// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file EDP.cpp
 *
 */

#include <fastrtps/rtps/builtin/discovery/endpoint/EDP.h>

#include <fastrtps/rtps/builtin/discovery/participant/PDPSimple.h>

#include "../../../participant/RTPSParticipantImpl.h"


#include <fastrtps/rtps/writer/RTPSWriter.h>
#include <fastrtps/rtps/reader/RTPSReader.h>
#include <fastrtps/rtps/writer/WriterListener.h>
#include <fastrtps/rtps/reader/ReaderListener.h>

#include <fastrtps/rtps/builtin/data/WriterProxyData.h>
#include <fastrtps/rtps/builtin/data/ReaderProxyData.h>
#include <fastrtps/rtps/builtin/data/ParticipantProxyData.h>

#include <fastrtps/attributes/TopicAttributes.h>
#include <fastrtps/rtps/common/MatchingInfo.h>

#include <fastrtps/utils/StringMatching.h>
#include <fastrtps/log/Log.h>

#include <fastrtps/types/TypeObjectFactory.h>

#include <mutex>

using namespace eprosima::fastrtps;

namespace eprosima {
namespace fastrtps{
namespace rtps {


EDP::EDP(PDPSimple* p,RTPSParticipantImpl* part): mp_PDP(p),
    mp_RTPSParticipant(part) { }

EDP::~EDP()
{
    // TODO Auto-generated destructor stub
}

bool EDP::newLocalReaderProxyData(RTPSReader* reader, const TopicAttributes& att, const ReaderQos& rqos)
{
    logInfo(RTPS_EDP,"Adding " << reader->getGuid().entityId << " in topic "<<att.topicName);
    ReaderProxyData rpd;
    rpd.isAlive(true);
    rpd.m_expectsInlineQos = reader->expectsInlineQos();
    rpd.guid(reader->getGuid());
    rpd.key() = rpd.guid();
    rpd.multicastLocatorList(reader->getAttributes().multicastLocatorList);
    rpd.unicastLocatorList(reader->getAttributes().unicastLocatorList);
    rpd.RTPSParticipantKey() = mp_RTPSParticipant->getGuid();
    rpd.topicName(att.getTopicName());
    rpd.typeName(att.getTopicDataType());
    rpd.topicKind(att.getTopicKind());
    rpd.m_qos = rqos;
    rpd.userDefinedId(reader->getAttributes().getUserDefinedID());
#if HAVE_SECURITY
    if (mp_RTPSParticipant->is_secure())
    {
        rpd.security_attributes_ = reader->getAttributes().security_attributes().mask();
        rpd.plugin_security_attributes_ = reader->getAttributes().security_attributes().plugin_endpoint_attributes;
    }
    else
    {
        rpd.security_attributes_ = 0UL;
        rpd.plugin_security_attributes_ = 0UL;
    }
#endif
    reader->m_acceptMessagesFromUnkownWriters = false;

    // Data representation guessings if the user didn't change it.
    if (rpd.m_qos.representation.m_value.empty() && !rpd.m_qos.representation.hasChanged())
    {
        if (att.type_information.assigned())
        {
            rpd.m_qos.representation.m_value.push_back(DataRepresentationId_t::XCDR2_DATA_REPRESENTATION);
            logInfo(EDP, "Added XCDR2_DATA_REPRESENTATION due to existence of type_information");
        }
        if (att.type_id.m_type_identifier._d() != 0)
        {
            rpd.m_qos.representation.m_value.push_back(DataRepresentationId_t::XCDR_DATA_REPRESENTATION);
            logInfo(EDP, "Added XCDR_DATA_REPRESENTATION due to existence of type_identifier");
        }
    }

    // DataRepresentation checkings
    for (DataRepresentationId_t representation : rpd.m_qos.representation.m_value)
    {
        switch (representation)
        {
            case DataRepresentationId_t::XCDR_DATA_REPRESENTATION:
                {
                    if (att.type_id.m_type_identifier._d() == 0) // Not set
                    {
                        const types::TypeIdentifier* type_id =
                                types::TypeObjectFactory::get_instance()->get_type_identifier_trying_complete(
                                    rpd.typeName().c_str());
                        if (type_id == nullptr)
                        {
                            logError(EDP, "Type identifier " << rpd.typeName() << " isn't registered using "
                                     << "XCDR_DATA_REPRESENTATION.");
                        }
                        else
                        {
                            rpd.type_id().m_type_identifier = *type_id;
                        }
                    }
                    else
                    {
                        rpd.type_id(att.type_id);
                    }

                    if (att.type.m_type_object._d() == 0
                        && (att.type_id.m_type_identifier._d() == types::EK_MINIMAL
                        || att.type_id.m_type_identifier._d() == types::EK_COMPLETE)) // Not set
                    {
                        const types::TypeObject *type_obj = types::TypeObjectFactory::get_instance()->get_type_object(
                                rpd.typeName().c_str(), att.type_id.m_type_identifier._d() == types::EK_COMPLETE);
                        if (type_obj == nullptr)
                        {
                            logError(EDP, "Type object " << rpd.typeName() << " isn't registered using "
                                     << "XCDR_DATA_REPRESENTATION.");
                        }
                        else
                        {
                            rpd.type().m_type_object = *type_obj;
                        }
                    }
                    else
                    {
                        rpd.type(att.type);
                    }
                }
                break;
            case DataRepresentationId_t::XML_DATA_REPRESENTATION:
                // Not supported
                logError(EDP, "Unsupported DataRepresentation: XML_DATA_REPRESENTATION: " << rpd.typeName().c_str());
                break;
            case DataRepresentationId_t::XCDR2_DATA_REPRESENTATION:
                {
                    if (att.type_information.assigned())
                    {
                        rpd.type_information(att.type_information);
                    }
                    else
                    {
                        const types::TypeInformation* type_info =
                            types::TypeObjectFactory::get_instance()->get_type_information(rpd.typeName().c_str());
                        if (type_info == nullptr)
                        {
                            logError(EDP, "Type information " << rpd.typeName() << " isn't registered using "
                                     << "XCDR2_DATA_REPRESENTATION.");
                        }
                        else
                        {
                            rpd.type_information() = *type_info;
                        }
                    }
                }
                break;
            //default: break;
        }
    }

    //ADD IT TO THE LIST OF READERPROXYDATA
    ParticipantProxyData pdata;
    if(!this->mp_PDP->addReaderProxyData(&rpd, pdata))
    {
        return false;
    }

    //PAIRING
    pairing_reader_proxy_with_any_local_writer(&pdata, &rpd);
    pairingReader(reader, pdata, rpd);
    //DO SOME PROCESSING DEPENDING ON THE IMPLEMENTATION (SIMPLE OR STATIC)
    processLocalReaderProxyData(reader, &rpd);
    return true;
}

bool EDP::newLocalWriterProxyData(RTPSWriter* writer, const TopicAttributes& att, const WriterQos& wqos)
{
    logInfo(RTPS_EDP,"Adding " << writer->getGuid().entityId << " in topic "<<att.topicName);
    WriterProxyData wpd;
    wpd.isAlive(true);
    wpd.guid(writer->getGuid());
    wpd.key() = wpd.guid();
    wpd.multicastLocatorList(writer->getAttributes().multicastLocatorList);
    wpd.unicastLocatorList(writer->getAttributes().unicastLocatorList);
    wpd.RTPSParticipantKey() = mp_RTPSParticipant->getGuid();
    wpd.topicName(att.getTopicName());
    wpd.typeName(att.getTopicDataType());
    wpd.topicKind(att.getTopicKind());
    wpd.typeMaxSerialized(writer->getTypeMaxSerialized());
    wpd.m_qos = wqos;
    wpd.userDefinedId(writer->getAttributes().getUserDefinedID());
    wpd.persistence_guid(writer->getAttributes().persistence_guid);
#if HAVE_SECURITY
    if (mp_RTPSParticipant->is_secure())
    {
        wpd.security_attributes_ = writer->getAttributes().security_attributes().mask();
        wpd.plugin_security_attributes_ = writer->getAttributes().security_attributes().plugin_endpoint_attributes;
    }
    else
    {
        wpd.security_attributes_ = 0UL;
        wpd.plugin_security_attributes_ = 0UL;
    }
#endif

    // Data representation guessings if the user didn't change it.
    if (wpd.m_qos.representation.m_value.empty() && !wpd.m_qos.representation.hasChanged())
    {
        if (att.type_information.assigned())
        {
            wpd.m_qos.representation.m_value.push_back(DataRepresentationId_t::XCDR2_DATA_REPRESENTATION);
            logInfo(EDP, "Added XCDR2_DATA_REPRESENTATION due to existence of type_information");
        }
        if (att.type_id.m_type_identifier._d() != 0)
        {
            wpd.m_qos.representation.m_value.push_back(DataRepresentationId_t::XCDR_DATA_REPRESENTATION);
            logInfo(EDP, "Added XCDR_DATA_REPRESENTATION due to existence of type_identifier");
        }
    }

    // DataRepresentation checkings
    for (DataRepresentationId_t representation : wpd.m_qos.representation.m_value)
    {
        switch (representation)
        {
            case DataRepresentationId_t::XCDR_DATA_REPRESENTATION:
                {
                    if (att.type_id.m_type_identifier._d() == 0) // Not set
                    {
                        const types::TypeIdentifier* type_id =
                                types::TypeObjectFactory::get_instance()->get_type_identifier_trying_complete(
                                    wpd.typeName().c_str());
                        if (type_id == nullptr)
                        {
                            logError(EDP, "Type identifier " << wpd.typeName() << " isn't registered using "
                                     << " XCDR_DATA_REPRESENTATION.");
                        }
                        else
                        {
                            wpd.type_id().m_type_identifier = *type_id;
                        }
                    }
                    else
                    {
                        wpd.type_id(att.type_id);
                    }

                    if (att.type.m_type_object._d() == 0
                        && (att.type_id.m_type_identifier._d() == types::EK_MINIMAL
                        || att.type_id.m_type_identifier._d() == types::EK_COMPLETE)) // Not set
                    {
                        const types::TypeObject *type_obj = types::TypeObjectFactory::get_instance()->get_type_object(
                                wpd.typeName().c_str(), att.type_id.m_type_identifier._d() == types::EK_COMPLETE);
                        if (type_obj == nullptr)
                        {
                            logError(EDP, "Type object " << wpd.typeName() << " isn't registered using "
                                     << " XCDR_DATA_REPRESENTATION.");
                        }
                        else
                        {
                            wpd.type().m_type_object = *type_obj;
                        }
                    }
                    else
                    {
                        wpd.type(att.type);
                    }
                }
                break;
            case DataRepresentationId_t::XML_DATA_REPRESENTATION:
                logError(EDP, "Unsupported DataRepresentation: XML_DATA_REPRESENTATION: " << wpd.typeName().c_str());
                // Not supported
                break;
            case DataRepresentationId_t::XCDR2_DATA_REPRESENTATION:
                {
                    if (att.type_information.assigned())
                    {
                        wpd.type_information(att.type_information);
                    }
                    else
                    {
                        const types::TypeInformation* type_info =
                            types::TypeObjectFactory::get_instance()->get_type_information(wpd.typeName().c_str());
                        if (type_info == nullptr)
                        {
                            logError(EDP, "Type information " << wpd.typeName() << " isn't registered using "
                                     << " XCDR2_DATA_REPRESENTATION.");
                        }
                        else
                        {
                            wpd.type_information() = *type_info;
                        }
                    }
                }
                break;
            //default: break;
        }
    }

    //ADD IT TO THE LIST OF READERPROXYDATA
    ParticipantProxyData pdata;
    if(!this->mp_PDP->addWriterProxyData(&wpd, pdata))
    {
        return false;
    }

    //PAIRING
    pairing_writer_proxy_with_any_local_reader(&pdata, &wpd);
    pairingWriter(writer, pdata, wpd);
    //DO SOME PROCESSING DEPENDING ON THE IMPLEMENTATION (SIMPLE OR STATIC)
    processLocalWriterProxyData(writer, &wpd);
    return true;
}

bool EDP::updatedLocalReader(RTPSReader* reader, const TopicAttributes& att, const ReaderQos& rqos)
{
    ParticipantProxyData pdata;
    ReaderProxyData rdata;
    rdata.isAlive(true);
    rdata.m_expectsInlineQos = reader->expectsInlineQos();
    rdata.guid(reader->getGuid());
    rdata.key() = rdata.guid();
    rdata.multicastLocatorList(reader->getAttributes().multicastLocatorList);
    rdata.unicastLocatorList(reader->getAttributes().unicastLocatorList);
    rdata.RTPSParticipantKey() = mp_RTPSParticipant->getGuid();
    rdata.topicName(att.getTopicName());
    rdata.typeName(att.getTopicDataType());
    rdata.topicKind(att.getTopicKind());
    rdata.m_qos.setQos(rqos, true);
    rdata.userDefinedId(reader->getAttributes().getUserDefinedID());
#if HAVE_SECURITY
    if (mp_RTPSParticipant->is_secure())
    {
        rdata.security_attributes_ = reader->getAttributes().security_attributes().mask();
        rdata.plugin_security_attributes_ = reader->getAttributes().security_attributes().plugin_endpoint_attributes;
    }
    else
    {
        rdata.security_attributes_ = 0UL;
        rdata.plugin_security_attributes_ = 0UL;
    }
#endif

    if(this->mp_PDP->addReaderProxyData(&rdata, pdata))
    {
        processLocalReaderProxyData(reader, &rdata);
        //this->updatedReaderProxy(rdata);
        pairing_reader_proxy_with_any_local_writer(&pdata, &rdata);
        pairingReader(reader, pdata, rdata);
        return true;
    }
    return false;
}

bool EDP::updatedLocalWriter(RTPSWriter* writer, const TopicAttributes& att, const WriterQos& wqos)
{
    ParticipantProxyData pdata;
    WriterProxyData wdata;
    wdata.isAlive(true);
    wdata.guid(writer->getGuid());
    wdata.key() = wdata.guid();
    wdata.multicastLocatorList(writer->getAttributes().multicastLocatorList);
    wdata.unicastLocatorList(writer->getAttributes().unicastLocatorList);
    wdata.RTPSParticipantKey() = mp_RTPSParticipant->getGuid();
    wdata.topicName(att.getTopicName());
    wdata.typeName(att.getTopicDataType());
    wdata.topicKind(att.getTopicKind());
    wdata.typeMaxSerialized(writer->getTypeMaxSerialized());
    wdata.m_qos.setQos(wqos,true);
    wdata.userDefinedId(writer->getAttributes().getUserDefinedID());
    wdata.persistence_guid(writer->getAttributes().persistence_guid);
#if HAVE_SECURITY
    if (mp_RTPSParticipant->is_secure())
    {
        wdata.security_attributes_ = writer->getAttributes().security_attributes().mask();
        wdata.plugin_security_attributes_ = writer->getAttributes().security_attributes().plugin_endpoint_attributes;
    }
    else
    {
        wdata.security_attributes_ = 0UL;
        wdata.plugin_security_attributes_ = 0UL;
    }
#endif

    if(this->mp_PDP->addWriterProxyData(&wdata, pdata))
    {
        processLocalWriterProxyData(writer, &wdata);
        //this->updatedWriterProxy(wdata);
        pairing_writer_proxy_with_any_local_reader(&pdata, &wdata);
        pairingWriter(writer, pdata, wdata);
        return true;
    }
    return false;
}

bool EDP::unpairWriterProxy(const GUID_t& participant_guid, const GUID_t& writer_guid)
{
    (void)participant_guid;

    logInfo(RTPS_EDP, writer_guid);

    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSReader*>::iterator rit = mp_RTPSParticipant->userReadersListBegin();
            rit!=mp_RTPSParticipant->userReadersListEnd();++rit)
    {
        RemoteWriterAttributes watt;
        watt.guid = writer_guid;
        if((*rit)->matched_writer_remove(watt))
        {
#if HAVE_SECURITY
            mp_RTPSParticipant->security_manager().remove_writer((*rit)->getGuid(), participant_guid, writer_guid);
#endif

            //MATCHED AND ADDED CORRECTLY:
            if((*rit)->getListener()!=nullptr)
            {
                MatchingInfo info;
                info.status = REMOVED_MATCHING;
                info.remoteEndpointGuid = writer_guid;
                (*rit)->getListener()->onReaderMatched((*rit),info);
            }
        }
    }
    return true;
}

bool EDP::unpairReaderProxy(const GUID_t& participant_guid, const GUID_t& reader_guid)
{
    (void)participant_guid;

    logInfo(RTPS_EDP, reader_guid);

    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSWriter*>::iterator wit = mp_RTPSParticipant->userWritersListBegin();
            wit!=mp_RTPSParticipant->userWritersListEnd();++wit)
    {
        RemoteReaderAttributes ratt;
        ratt.guid = reader_guid;
        if((*wit)->matched_reader_remove(ratt))
        {
#if HAVE_SECURITY
            mp_RTPSParticipant->security_manager().remove_reader((*wit)->getGuid(), participant_guid, reader_guid);
#endif
            //MATCHED AND ADDED CORRECTLY:
            if((*wit)->getListener()!=nullptr)
            {
                MatchingInfo info;
                info.status = REMOVED_MATCHING;
                info.remoteEndpointGuid = reader_guid;
                (*wit)->getListener()->onWriterMatched((*wit),info);
            }
        }
    }
    return true;
}


bool EDP::validMatching(const WriterProxyData* wdata, const ReaderProxyData* rdata)
{
    if (wdata->topicName() != rdata->topicName())
    {
        return false;
    }

    // Type Consistency Enforcement QosPolicy
    if (!checkTypeValidation(wdata, rdata))
    {
        // TODO Trigger INCONSISTENT_TOPIC status change
        return false;
    }

    if(wdata->topicKind() != rdata->topicKind())
    {
        logWarning(RTPS_EDP, "INCOMPATIBLE QOS:Remote Reader " << rdata->guid() << " is publishing in topic "
                << rdata->topicName() << "(keyed:"<< rdata->topicKind() <<
                "), local writer publishes as keyed: "<< wdata->topicKind())
            return false;
    }
    /*
    if(!checkTypeIdentifier(wdata, rdata))
    {
        logInfo(RTPS_EDP, "Matching failed on checkTypeIdentifier.");
        return false;
    }
    */
    if(!rdata->isAlive()) //Matching
    {
        logWarning(RTPS_EDP,"ReaderProxyData object is NOT alive");
        return false;
    }
    if( wdata->m_qos.m_reliability.kind == BEST_EFFORT_RELIABILITY_QOS
            && rdata->m_qos.m_reliability.kind == RELIABLE_RELIABILITY_QOS) //Means our writer is BE but the reader wants RE
    {
        logWarning(RTPS_EDP,"INCOMPATIBLE QOS (topic: "<< rdata->topicName() <<"):Remote Reader "
                << rdata->guid() << " is Reliable and local writer is BE ");
        return false;
    }
    if(wdata->m_qos.m_durability.kind < rdata->m_qos.m_durability.kind)
    {
        // TODO (MCC) Change log message
        logWarning(RTPS_EDP,"INCOMPATIBLE QOS (topic: "<< rdata->topicName() <<"):RemoteReader "
                << rdata->guid() << " has TRANSIENT_LOCAL DURABILITY and we offer VOLATILE");
        return false;
    }
    if(wdata->m_qos.m_ownership.kind != rdata->m_qos.m_ownership.kind)
    {
        logWarning(RTPS_EDP,"INCOMPATIBLE QOS (topic: "<< rdata->topicName() <<"):Remote reader "
                << rdata->guid() << " has different Ownership Kind");
        return false;
    }
    if(wdata->m_qos.m_deadline.period > rdata->m_qos.m_deadline.period)
    {
        logWarning(RTPS_EDP,"INCOMPATIBLE QOS (topic: "<< rdata->topicName() <<"):Remote reader "
                << rdata->guid() << " has smaller DEADLINE period");
        return false;
    }
    if (!wdata->m_qos.m_disablePositiveACKs.enabled && rdata->m_qos.m_disablePositiveACKs.enabled)
    {
        logWarning(RTPS_EDP, "Incompatible Disable Positive Acks QoS: writer is enabled but reader is not");
        return false;
    }

    if (!checkDataRepresentationQos(wdata, rdata))
    {
        logWarning(RTPS_EDP, "INCOMPATIBLE Data Representation QOS (topic: " << rdata->topicName()
            << "):Remote reader " << rdata->guid() << " hasn't any representation compatible");
        return false;
    }

#if HAVE_SECURITY
    // TODO: Check EndpointSecurityInfo
#endif

    //Partition check:
    bool matched = false;
    if(wdata->m_qos.m_partition.names.empty() && rdata->m_qos.m_partition.names.empty())
    {
        matched = true;
    }
    else if(wdata->m_qos.m_partition.names.empty() && rdata->m_qos.m_partition.names.size()>0)
    {
        for(std::vector<std::string>::const_iterator rnameit = rdata->m_qos.m_partition.names.begin();
                rnameit!=rdata->m_qos.m_partition.names.end();++rnameit)
        {
            if(rnameit->size()==0)
            {
                matched = true;
                break;
            }
        }
    }
    else if(wdata->m_qos.m_partition.names.size()>0 && rdata->m_qos.m_partition.names.empty() )
    {
        for(std::vector<std::string>::const_iterator wnameit = wdata->m_qos.m_partition.names.begin();
                wnameit !=  wdata->m_qos.m_partition.names.end();++wnameit)
        {
            if(wnameit->size()==0)
            {
                matched = true;
                break;
            }
        }
    }
    else
    {
        for(std::vector<std::string>::const_iterator wnameit = wdata->m_qos.m_partition.names.begin();
                wnameit !=  wdata->m_qos.m_partition.names.end();++wnameit)
        {
            for(std::vector<std::string>::const_iterator rnameit = rdata->m_qos.m_partition.names.begin();
                    rnameit!=rdata->m_qos.m_partition.names.end();++rnameit)
            {
                if(StringMatching::matchString(wnameit->c_str(),rnameit->c_str()))
                {
                    matched = true;
                    break;
                }
            }
            if(matched)
                break;
        }
    }
    if(!matched) //Different partitions
        logWarning(RTPS_EDP,"INCOMPATIBLE QOS (topic: "<< rdata->topicName() <<"): Different Partitions");
    return matched;
}

/**
 * @brief EDP::checkDataRepresentationQos
 * Table 7.57 XTypes document 1.2
 * Writer   Reader  Compatible
 * XCDR     XCDR    true
 * XCDR     XCDR2   true
 * XCDR2    XCDR    false
 * XCDR2    XCDR2   true
 * @param wdata
 * @param rdata
 * @return
 */
bool EDP::checkDataRepresentationQos(const WriterProxyData* wdata, const ReaderProxyData* rdata) const
{
    bool compatible = wdata->m_qos.representation.m_value.empty() || rdata->m_qos.representation.m_value.empty();
    if (!compatible)
    {
        const std::vector<DataRepresentationId_t>& rr = rdata->m_qos.representation.m_value;
        for (DataRepresentationId writerRepresentation : wdata->m_qos.representation.m_value)
        {
            if (writerRepresentation == XCDR2_DATA_REPRESENTATION)
            {
                compatible |= std::find(rr.begin(), rr.end(), XCDR2_DATA_REPRESENTATION) != rr.end();
            }
            else if (writerRepresentation == XCDR_DATA_REPRESENTATION)
            {
                compatible |= std::find(rr.begin(), rr.end(), XCDR2_DATA_REPRESENTATION) != rr.end();
                compatible |= std::find(rr.begin(), rr.end(), XCDR_DATA_REPRESENTATION) != rr.end();
            }
            else // XML_DATA_REPRESENTATION
            {
                logInfo(EDP, "DataRepresentationQosPolicy XML_DATA_REPRESENTATION isn't supported.");
            }
        }
    }
    return compatible;
}

bool EDP::checkTypeValidation(const WriterProxyData* wdata, const ReaderProxyData* rdata) const
{
    if (rdata->m_qos.type_consistency.m_kind == DISALLOW_TYPE_COERCION)
    {
        if (!checkTypeIdentifier(wdata, rdata))
        {
            logWarning(RTPS_EDP, "Type Validation (DISALLOW_TYPE_COERCION) FAILED (typeName: " << rdata->typeName()
                << "): Local reader " << rdata->guid() << " didn't match with remote writer ("
                << wdata->guid() << ") (typeName: " << wdata->typeName() << ")");
            return false;
        }
        return true;
    }

    const std::vector<DataRepresentationId_t>& wr = wdata->m_qos.representation.m_value;
    const std::vector<DataRepresentationId_t>& rr = rdata->m_qos.representation.m_value;

    if (wr.empty() || rr.empty())
    {
        return wdata->typeName() == rdata->typeName();
    }

    if (std::find(wr.begin(), wr.end(), XCDR2_DATA_REPRESENTATION) != wr.end())
    {
        if (std::find(rr.begin(), rr.end(), XCDR2_DATA_REPRESENTATION) != rr.end())
        {
            if (wdata->type_information().assigned() && rdata->type_information().assigned())
            {
                std::cout << "Both has XCDR2" << std::endl;
                return wdata->type_information().type_information.complete().typeid_with_size().type_id().consistent(
                    rdata->type_information().type_information.complete().typeid_with_size().type_id(),
                    rdata->m_qos.type_consistency);
            }
            std::cout << "Both has XCDR2 but at least one of them haven't assigned it." << std::endl;
            return false;
        }
        // Not compatible (p. 170, XTypes 1.2)
        /*
        else if (std::find(rr.begin(), rr.end(), XCDR_DATA_REPRESENTATION) != rr.end())
        {
            return wdata->type_information().type_information.complete().typeid_with_size().type_id().consistent(
                rdata->type_id().m_type_identifier,
                rdata->m_qos.type_consistency);
        }
        */
    }

    if (std::find(wr.begin(), wr.end(), XCDR_DATA_REPRESENTATION) != wr.end())
    {
        if (std::find(rr.begin(), rr.end(), XCDR2_DATA_REPRESENTATION) != rr.end())
        {
            if (wdata->type_id().m_type_identifier._d() != static_cast<uint8_t>(0x00))
            {
                std::cout << "XCDR vs XCDR2" << std::endl;
                return wdata->type_id().m_type_identifier.consistent(
                            rdata->type_information().type_information.complete().typeid_with_size().type_id(),
                            rdata->m_qos.type_consistency);
            }
            std::cout << "XCDR vs XCDR2 writer has no type identifier." << std::endl;
            return false;
        }
        else if (std::find(rr.begin(), rr.end(), XCDR_DATA_REPRESENTATION) != rr.end())
        {
            if (wdata->type_id().m_type_identifier._d() != static_cast<uint8_t>(0x00))
            {
                std::cout << "Both XCDR" << std::endl;
                return wdata->type_id().m_type_identifier.consistent(
                            rdata->type_id().m_type_identifier,
                            rdata->m_qos.type_consistency);
            }
            std::cout << "Both XCDR, but writer has no type identifier." << std::endl;
            return false;
        }
    }

    std::cout << "Checks ended... FAILED" << std::endl;
    return false;
}

bool EDP::validMatching(const ReaderProxyData* rdata, const WriterProxyData* wdata)
{
    if (rdata->topicName() != wdata->topicName())
    {
        return false;
    }

    // Type Consistency Enforcement QosPolicy
    if (!checkTypeValidation(wdata, rdata))
    {
        // TODO Trigger INCONSISTENT_TOPIC status change
        return false;
    }

    if(rdata->topicKind() != wdata->topicKind())
    {
        logWarning(RTPS_EDP, "INCOMPATIBLE QOS:Remote Writer " << wdata->guid() << " is publishing in topic "
            << wdata->topicName() << "(keyed:" << wdata->topicKind() << "), local reader subscribes as keyed: "
            << rdata->topicKind());
        return false;
    }
    /* Now it is done in checkTypeValidation
    if(!checkTypeIdentifier(wdata, rdata))
    {
        logInfo(RTPS_EDP, "Matching failed on checkTypeIdentifier.");
        std::cout << "--- TypeIdentifier failed ---" << std::endl;
        return false;
    }
    */
    if(!wdata->isAlive()) //Matching
    {
        logWarning(RTPS_EDP, "WriterProxyData " << wdata->guid() << " is NOT alive");
        return false;
    }
    if(rdata->m_qos.m_reliability.kind == RELIABLE_RELIABILITY_QOS
            && wdata->m_qos.m_reliability.kind == BEST_EFFORT_RELIABILITY_QOS) //Means our reader is reliable but hte writer is not
    {
        logWarning(RTPS_EDP,"INCOMPATIBLE QOS (topic: "<< wdata->topicName() << "): Remote Writer " << wdata->guid() << " is Best Effort and local reader is RELIABLE " << endl;);
        return false;
    }
    if(rdata->m_qos.m_durability.kind > wdata->m_qos.m_durability.kind)
    {
        // TODO (MCC) Change log message
        logWarning(RTPS_EDP, "INCOMPATIBLE QOS (topic: " << wdata->topicName() << "):RemoteWriter " << wdata->guid() << " has VOLATILE DURABILITY and we want TRANSIENT_LOCAL" << endl;);
        return false;
    }
    if(rdata->m_qos.m_ownership.kind != wdata->m_qos.m_ownership.kind)
    {
        logWarning(RTPS_EDP, "INCOMPATIBLE QOS (topic: " << wdata->topicName() << "):Remote Writer " << wdata->guid() << " has different Ownership Kind" << endl;);
        return false;
    }
    if(rdata->m_qos.m_deadline.period < wdata->m_qos.m_deadline.period)
    {
        logWarning(RTPS_EDP, "INCOMPATIBLE QOS (topic: "
                   << wdata->topicName() << "):RemoteWriter "
                   << wdata->guid() << "has smaller DEADLINE period");
        return false;
    }
    if (rdata->m_qos.m_disablePositiveACKs.enabled && !wdata->m_qos.m_disablePositiveACKs.enabled)
    {
        logWarning(RTPS_EDP, "Incompatible Disable Positive Acks QoS: writer is enabled but reader is not");
        return false;
    }

#if HAVE_SECURITY
    // TODO: Check EndpointSecurityInfo
#endif

    if (!checkDataRepresentationQos(wdata, rdata))
    {
        logWarning(RTPS_EDP, "INCOMPATIBLE Data Representation QOS (topic: " << rdata->topicName()
            << "):Local reader " << rdata->guid() << " hasn't any representation compatible");
        std::cout << "Data representation failed :D" << std::endl;
        return false;
    }

    //Partition check:
    bool matched = false;
    if(rdata->m_qos.m_partition.names.empty() && wdata->m_qos.m_partition.names.empty())
    {
        matched = true;
    }
    else if(rdata->m_qos.m_partition.names.empty() && wdata->m_qos.m_partition.names.size()>0)
    {
        for(std::vector<std::string>::const_iterator rnameit = wdata->m_qos.m_partition.names.begin();
                rnameit!=wdata->m_qos.m_partition.names.end();++rnameit)
        {
            if(rnameit->size()==0)
            {
                matched = true;
                break;
            }
        }
    }
    else if(rdata->m_qos.m_partition.names.size()>0 && wdata->m_qos.m_partition.names.empty() )
    {
        for(std::vector<std::string>::const_iterator wnameit = rdata->m_qos.m_partition.names.begin();
                wnameit !=  rdata->m_qos.m_partition.names.end();++wnameit)
        {
            if(wnameit->size()==0)
            {
                matched = true;
                break;
            }
        }
    }
    else
    {
        for(std::vector<std::string>::const_iterator wnameit = rdata->m_qos.m_partition.names.begin();
                wnameit !=  rdata->m_qos.m_partition.names.end();++wnameit)
        {
            for(std::vector<std::string>::const_iterator rnameit = wdata->m_qos.m_partition.names.begin();
                    rnameit!=wdata->m_qos.m_partition.names.end();++rnameit)
            {
                if(StringMatching::matchString(wnameit->c_str(),rnameit->c_str()))
                {
                    matched = true;
                    break;
                }
            }
            if(matched)
                break;
        }
    }
    if(!matched) //Different partitions
        logWarning(RTPS_EDP, "INCOMPATIBLE QOS (topic: " <<  wdata->topicName() << "): Different Partitions");

    return matched;

}

//TODO Estas cuatro funciones comparten codigo comun (2 a 2) y se podrían seguramente combinar.

bool EDP::pairingReader(RTPSReader* R, const ParticipantProxyData& pdata, const ReaderProxyData& rdata)
{
    (void)pdata;

    logInfo(RTPS_EDP, rdata.guid() <<" in topic: \"" << rdata.topicName() <<"\"");
    std::lock_guard<std::recursive_mutex> pguard(*mp_PDP->getMutex());

    for(std::vector<ParticipantProxyData*>::const_iterator pit = mp_PDP->ParticipantProxiesBegin();
            pit!=mp_PDP->ParticipantProxiesEnd(); ++pit)
    {
        for(std::vector<WriterProxyData*>::iterator wdatait = (*pit)->m_writers.begin();
                wdatait != (*pit)->m_writers.end(); ++wdatait)
        {
            bool valid = validMatching(&rdata, *wdatait);

            if(valid)
            {
#if HAVE_SECURITY
                if(!mp_RTPSParticipant->security_manager().discovered_writer(R->m_guid, (*pit)->m_guid,
                            **wdatait, R->getAttributes().security_attributes()))
                {
                    logError(RTPS_EDP, "Security manager returns an error for reader " << R->getGuid());
                }
#else
                RemoteWriterAttributes rwatt = (*wdatait)->toRemoteWriterAttributes();
                if(R->matched_writer_add(rwatt))
                {
                    logInfo(RTPS_EDP, "Valid Matching to writerProxy: " << (*wdatait)->guid());
                    //MATCHED AND ADDED CORRECTLY:
                    if(R->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = MATCHED_MATCHING;
                        info.remoteEndpointGuid = (*wdatait)->guid();
                        R->getListener()->onReaderMatched(R,info);
                    }
                }
#endif
            }
            else
            {
                //logInfo(RTPS_EDP,RTPS_CYAN<<"Valid Matching to writerProxy: "<<(*wdatait)->m_guid<<RTPS_DEF<<endl);
                if(R->matched_writer_is_matched((*wdatait)->toRemoteWriterAttributes())
                        && R->matched_writer_remove((*wdatait)->toRemoteWriterAttributes()))
                {
#if HAVE_SECURITY
                    mp_RTPSParticipant->security_manager().remove_writer(R->getGuid(), pdata.m_guid, (*wdatait)->guid());
#endif

                    //MATCHED AND ADDED CORRECTLY:
                    if(R->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = REMOVED_MATCHING;
                        info.remoteEndpointGuid = (*wdatait)->guid();
                        R->getListener()->onReaderMatched(R,info);
                    }
                }
            }
        }
    }
    return true;
}

//TODO Añadir WriterProxyData como argumento con nullptr como valor por defecto.
bool EDP::pairingWriter(RTPSWriter* W, const ParticipantProxyData& pdata, const WriterProxyData& wdata)
{
    (void) pdata;

    logInfo(RTPS_EDP, W->getGuid() << " in topic: \"" << wdata.topicName() <<"\"");
    std::lock_guard<std::recursive_mutex> pguard(*mp_PDP->getMutex());

    for(std::vector<ParticipantProxyData*>::const_iterator pit = mp_PDP->ParticipantProxiesBegin();
            pit!=mp_PDP->ParticipantProxiesEnd(); ++pit)
    {
        for(std::vector<ReaderProxyData*>::iterator rdatait = (*pit)->m_readers.begin();
                rdatait!=(*pit)->m_readers.end(); ++rdatait)
        {
            bool valid = validMatching(&wdata, *rdatait);

            if(valid)
            {
#if HAVE_SECURITY
                if(!mp_RTPSParticipant->security_manager().discovered_reader(W->getGuid(), (*pit)->m_guid,
                            **rdatait, W->getAttributes().security_attributes()))
                {
                    logError(RTPS_EDP, "Security manager returns an error for writer " << W->getGuid());
                }
#else
                RemoteReaderAttributes rratt = (*rdatait)->toRemoteReaderAttributes();
                if(W->matched_reader_add(rratt))
                {
                    logInfo(RTPS_EDP,"Valid Matching to readerProxy: " << (*rdatait)->guid());
                    //MATCHED AND ADDED CORRECTLY:
                    if(W->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = MATCHED_MATCHING;
                        info.remoteEndpointGuid = (*rdatait)->guid();
                        W->getListener()->onWriterMatched(W,info);
                    }
                }
#endif
            }
            else
            {
                //logInfo(RTPS_EDP,RTPS_CYAN<<"Valid Matching to writerProxy: "<<(*wdatait)->m_guid<<RTPS_DEF<<endl);
                if(W->matched_reader_is_matched((*rdatait)->toRemoteReaderAttributes()) &&
                        W->matched_reader_remove((*rdatait)->toRemoteReaderAttributes()))
                {
#if HAVE_SECURITY
                    mp_RTPSParticipant->security_manager().remove_reader(W->getGuid(), pdata.m_guid, (*rdatait)->guid());
#endif
                    //MATCHED AND ADDED CORRECTLY:
                    if(W->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = REMOVED_MATCHING;
                        info.remoteEndpointGuid = (*rdatait)->guid();
                        W->getListener()->onWriterMatched(W,info);
                    }
                }
            }
        }
    }
    return true;
}

bool EDP::pairing_reader_proxy_with_any_local_writer(ParticipantProxyData* pdata, ReaderProxyData* rdata)
{
    (void)pdata;

    logInfo(RTPS_EDP, rdata->guid() <<" in topic: \"" << rdata->topicName() <<"\"");
    std::lock_guard<std::recursive_mutex> pguard(*mp_PDP->getMutex());
    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSWriter*>::iterator wit = mp_RTPSParticipant->userWritersListBegin();
            wit!=mp_RTPSParticipant->userWritersListEnd();++wit)
    {
        (*wit)->getMutex().lock();
        GUID_t writerGUID = (*wit)->getGuid();
        (*wit)->getMutex().unlock();
        ParticipantProxyData wpdata;
        WriterProxyData wdata;
        if(mp_PDP->lookupWriterProxyData(writerGUID, wdata, wpdata))
        {
            bool valid = validMatching(&wdata, rdata);

            if(valid)
            {
#if HAVE_SECURITY
                if(!mp_RTPSParticipant->security_manager().discovered_reader(writerGUID, pdata->m_guid,
                            *rdata, (*wit)->getAttributes().security_attributes()))
                {
                    logError(RTPS_EDP, "Security manager returns an error for writer " << writerGUID);
                }
#else
                RemoteReaderAttributes rratt = rdata->toRemoteReaderAttributes();
                if((*wit)->matched_reader_add(rratt))
                {
                    logInfo(RTPS_EDP, "Valid Matching to local writer: " << writerGUID.entityId);
                    //MATCHED AND ADDED CORRECTLY:
                    if((*wit)->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = MATCHED_MATCHING;
                        info.remoteEndpointGuid = rdata->guid();
                        (*wit)->getListener()->onWriterMatched((*wit),info);
                    }
                }
#endif
            }
            else
            {
                if((*wit)->matched_reader_is_matched(rdata->toRemoteReaderAttributes())
                        && (*wit)->matched_reader_remove(rdata->toRemoteReaderAttributes()))
                {
#if HAVE_SECURITY
                    mp_RTPSParticipant->security_manager().remove_reader((*wit)->getGuid(), pdata->m_guid, rdata->guid());
#endif
                    //MATCHED AND ADDED CORRECTLY:
                    if((*wit)->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = REMOVED_MATCHING;
                        info.remoteEndpointGuid = rdata->guid();
                        (*wit)->getListener()->onWriterMatched((*wit),info);
                    }
                }
            }
        }
    }

    return true;
}

#if HAVE_SECURITY
bool EDP::pairing_reader_proxy_with_local_writer(const GUID_t& local_writer, const GUID_t& remote_participant_guid,
        ReaderProxyData& rdata)
{
    logInfo(RTPS_EDP, rdata.guid() <<" in topic: \"" << rdata.topicName() <<"\"");
    std::lock_guard<std::recursive_mutex> pguard(*mp_PDP->getMutex());
    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSWriter*>::iterator wit = mp_RTPSParticipant->userWritersListBegin();
            wit!=mp_RTPSParticipant->userWritersListEnd();++wit)
    {
        (*wit)->getMutex().lock();
        GUID_t writerGUID = (*wit)->getGuid();
        (*wit)->getMutex().unlock();

        if(local_writer == writerGUID)
        {
            ParticipantProxyData wpdata;
            WriterProxyData wdata;
            if(mp_PDP->lookupWriterProxyData(writerGUID, wdata, wpdata))
            {
                bool valid = validMatching(&wdata, &rdata);

                if(valid)
                {
                    if(!mp_RTPSParticipant->security_manager().discovered_reader(writerGUID,
                                remote_participant_guid, rdata, (*wit)->getAttributes().security_attributes()))
                    {
                        logError(RTPS_EDP, "Security manager returns an error for writer " << writerGUID);
                    }
                }
                else
                {
                    if((*wit)->matched_reader_is_matched(rdata.toRemoteReaderAttributes())
                            && (*wit)->matched_reader_remove(rdata.toRemoteReaderAttributes()))
                    {
                        mp_RTPSParticipant->security_manager().remove_reader((*wit)->getGuid(),
                                remote_participant_guid, rdata.guid());
                        //MATCHED AND ADDED CORRECTLY:
                        if((*wit)->getListener()!=nullptr)
                        {
                            MatchingInfo info;
                            info.status = REMOVED_MATCHING;
                            info.remoteEndpointGuid = rdata.guid();
                            (*wit)->getListener()->onWriterMatched((*wit),info);
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool EDP::pairing_remote_reader_with_local_writer_after_security(const GUID_t& local_writer,
        const ReaderProxyData& remote_reader_data)
{
    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSWriter*>::iterator wit = mp_RTPSParticipant->userWritersListBegin();
            wit!=mp_RTPSParticipant->userWritersListEnd();++wit)
    {
        (*wit)->getMutex().lock();
        GUID_t writerGUID = (*wit)->getGuid();
        (*wit)->getMutex().unlock();

        if(local_writer == writerGUID)
        {
            RemoteReaderAttributes rratt = remote_reader_data.toRemoteReaderAttributes();
            if((*wit)->matched_reader_add(rratt))
            {
                logInfo(RTPS_EDP, "Valid Matching to local writer: " << writerGUID.entityId);
                //MATCHED AND ADDED CORRECTLY:
                if((*wit)->getListener()!=nullptr)
                {
                    MatchingInfo info;
                    info.status = MATCHED_MATCHING;
                    info.remoteEndpointGuid = remote_reader_data.guid();
                    (*wit)->getListener()->onWriterMatched((*wit),info);
                }

                return true;
            }

            return false;
        }
    }

    return pairing_remote_reader_with_local_builtin_writer_after_security(local_writer, remote_reader_data);
}
#endif

bool EDP::pairing_writer_proxy_with_any_local_reader(ParticipantProxyData *pdata, WriterProxyData* wdata)
{
    (void)pdata;

    logInfo(RTPS_EDP, wdata->guid() <<" in topic: \"" << wdata->topicName() <<"\"");
    std::lock_guard<std::recursive_mutex> pguard(*mp_PDP->getMutex());
    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSReader*>::iterator rit = mp_RTPSParticipant->userReadersListBegin();
            rit!=mp_RTPSParticipant->userReadersListEnd();++rit)
    {
        GUID_t readerGUID;
        (*rit)->getMutex().lock();
        readerGUID = (*rit)->getGuid();
        (*rit)->getMutex().unlock();
        ParticipantProxyData rpdata;
        ReaderProxyData rdata;
        if(mp_PDP->lookupReaderProxyData(readerGUID, rdata, rpdata))
        {
            bool valid = validMatching(&rdata, wdata);

            if(valid)
            {
#if HAVE_SECURITY
                if(!mp_RTPSParticipant->security_manager().discovered_writer(readerGUID, pdata->m_guid,
                            *wdata, (*rit)->getAttributes().security_attributes()))
                {
                    logError(RTPS_EDP, "Security manager returns an error for reader " << readerGUID);
                }
#else
                RemoteWriterAttributes rwatt = wdata->toRemoteWriterAttributes();
                if((*rit)->matched_writer_add(rwatt))
                {
                    logInfo(RTPS_EDP, "Valid Matching to local reader: " << readerGUID.entityId);
                    //MATCHED AND ADDED CORRECTLY:
                    if((*rit)->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = MATCHED_MATCHING;
                        info.remoteEndpointGuid = wdata->guid();
                        (*rit)->getListener()->onReaderMatched((*rit),info);
                    }
                }
#endif
            }
            else
            {
                if((*rit)->matched_writer_is_matched(wdata->toRemoteWriterAttributes())
                        && (*rit)->matched_writer_remove(wdata->toRemoteWriterAttributes()))
                {
#if HAVE_SECURITY
                    mp_RTPSParticipant->security_manager().remove_writer((*rit)->getGuid(), pdata->m_guid, wdata->guid());
#endif
                    //MATCHED AND ADDED CORRECTLY:
                    if((*rit)->getListener()!=nullptr)
                    {
                        MatchingInfo info;
                        info.status = REMOVED_MATCHING;
                        info.remoteEndpointGuid = wdata->guid();
                        (*rit)->getListener()->onReaderMatched((*rit),info);
                    }
                }
            }
        }
    }
    return true;
}

#if HAVE_SECURITY
bool EDP::pairing_writer_proxy_with_local_reader(const GUID_t& local_reader, const GUID_t& remote_participant_guid,
        WriterProxyData& wdata)
{
    logInfo(RTPS_EDP, wdata.guid() <<" in topic: \"" << wdata.topicName() <<"\"");
    std::lock_guard<std::recursive_mutex> pguard(*mp_PDP->getMutex());
    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSReader*>::iterator rit = mp_RTPSParticipant->userReadersListBegin();
            rit!=mp_RTPSParticipant->userReadersListEnd();++rit)
    {
        GUID_t readerGUID;
        (*rit)->getMutex().lock();
        readerGUID = (*rit)->getGuid();
        (*rit)->getMutex().unlock();

        if(local_reader == readerGUID)
        {
            ParticipantProxyData rpdata;
            ReaderProxyData rdata;
            if(mp_PDP->lookupReaderProxyData(readerGUID, rdata, rpdata))
            {
                bool valid = validMatching(&rdata, &wdata);

                if(valid)
                {
                    if(!mp_RTPSParticipant->security_manager().discovered_writer(readerGUID,
                                remote_participant_guid, wdata, (*rit)->getAttributes().security_attributes()))
                    {
                        logError(RTPS_EDP, "Security manager returns an error for reader " << readerGUID);
                    }
                }
                else
                {
                    if((*rit)->matched_writer_is_matched(wdata.toRemoteWriterAttributes())
                            && (*rit)->matched_writer_remove(wdata.toRemoteWriterAttributes()))
                    {
                        mp_RTPSParticipant->security_manager().remove_writer((*rit)->getGuid(),
                                remote_participant_guid, wdata.guid());
                        //MATCHED AND ADDED CORRECTLY:
                        if((*rit)->getListener()!=nullptr)
                        {
                            MatchingInfo info;
                            info.status = REMOVED_MATCHING;
                            info.remoteEndpointGuid = wdata.guid();
                            (*rit)->getListener()->onReaderMatched((*rit),info);
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool EDP::pairing_remote_writer_with_local_reader_after_security(const GUID_t& local_reader,
                const WriterProxyData& remote_writer_data)
{
    std::lock_guard<std::recursive_mutex> guard(*mp_RTPSParticipant->getParticipantMutex());
    for(std::vector<RTPSReader*>::iterator rit = mp_RTPSParticipant->userReadersListBegin();
            rit!=mp_RTPSParticipant->userReadersListEnd();++rit)
    {
        GUID_t readerGUID;
        (*rit)->getMutex().lock();
        readerGUID = (*rit)->getGuid();
        (*rit)->getMutex().unlock();

        if(local_reader == readerGUID)
        {
            // TODO(richiware) Implement and use move with attributes
            RemoteWriterAttributes rwatt = remote_writer_data.toRemoteWriterAttributes();
            if((*rit)->matched_writer_add(rwatt))
            {
                logInfo(RTPS_EDP, "Valid Matching to local reader: " << readerGUID.entityId);
                //MATCHED AND ADDED CORRECTLY:
                if((*rit)->getListener()!=nullptr)
                {
                    MatchingInfo info;
                    info.status = MATCHED_MATCHING;
                    info.remoteEndpointGuid = remote_writer_data.guid();
                    (*rit)->getListener()->onReaderMatched((*rit),info);
                }

                return true;
            }

            return false;
        }
    }

    return pairing_remote_writer_with_local_builtin_reader_after_security(local_reader, remote_writer_data);
}
#endif

bool EDP::checkTypeIdentifier(const WriterProxyData* wdata, const ReaderProxyData* rdata) const
{
    const std::vector<DataRepresentationId_t>& wr = wdata->m_qos.representation.m_value;
    const std::vector<DataRepresentationId_t>& rr = rdata->m_qos.representation.m_value;

    if (wr.empty() || rr.empty())
    {
        return wdata->typeName() == rdata->typeName();
    }

    if (std::find(wr.begin(), wr.end(), XCDR2_DATA_REPRESENTATION) != wr.end())
    {
        if (std::find(rr.begin(), rr.end(), XCDR2_DATA_REPRESENTATION) != rr.end())
        {
            return wdata->type_information().type_information.minimal().typeid_with_size().type_id()
                    == rdata->type_information().type_information.minimal().typeid_with_size().type_id();
        }
        // Not compatible (p. 170, XTypes 1.2)
        /*
        else if (std::find(rr.begin(), rr.end(), XCDR_DATA_REPRESENTATION) != rr.end())
        {
            return wdata->type_information().type_information.minimal().typeid_with_size().type_id()
                    == rdata->type_id().m_type_identifier;
        }
        */
    }

    if (std::find(wr.begin(), wr.end(), XCDR_DATA_REPRESENTATION) != wr.end())
    {
        if (std::find(rr.begin(), rr.end(), XCDR2_DATA_REPRESENTATION) != rr.end())
        {
            return wdata->type_id().m_type_identifier == rdata->type_id().m_type_identifier
                    || wdata->type_id().m_type_identifier
                    == rdata->type_information().type_information.minimal().typeid_with_size().type_id();
        }
        else if (std::find(rr.begin(), rr.end(), XCDR_DATA_REPRESENTATION) != rr.end())
        {
            return wdata->type_id().m_type_identifier == rdata->type_id().m_type_identifier;
        }

    }

    return false;
}

}
} /* namespace rtps */
} /* namespace eprosima */
