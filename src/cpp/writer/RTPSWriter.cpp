/*************************************************************************
 * Copyright (c) 2013 eProsima. All rights reserved.
 *
 * This copy of FastCdr is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/*
 * RTPSWriter.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: Gonzalo Rodriguez Canosa
 *      email:  gonzalorodriguez@eprosima.com
 *      		grcanosa@gmail.com
 */

#include "eprosimartps/writer/RTPSWriter.h"
#include "eprosimartps/HistoryCache.h"
#include "eprosimartps/CDRMessage.h"
#include "eprosimartps/dds/Publisher.h"

#include "eprosimartps/dds/ParameterList.h"

namespace eprosima {
namespace rtps {

RTPSWriter::RTPSWriter():rtpsw_header(RTPSMESSAGE_HEADER_SIZE) {
	// TODO Auto-generated constructor stub
		RTPSMessageCreator::createHeader(&rtpsw_header,participant->guid.guidPrefix);
}

RTPSWriter::~RTPSWriter() {
	// TODO Auto-generated destructor stub
}

bool RTPSWriter::new_change(ChangeKind_t changeKind,
		SerializedPayload_t* data, InstanceHandle_t handle,CacheChange_t* change)
{
	change->kind = changeKind;
	//change->sequenceNumber = lastChangeSequenceNumber;
	change->writerGUID = guid;
	change->instanceHandle = handle;
	if(change->kind == ALIVE)
		change->serializedPayload.copy(data);
	return true;
}

void RTPSWriter::DataSubM(CDRMessage_t* submsg,bool expectsInlineQos,CacheChange_t* change,EntityId_t ReaderId)
{
	ParameterList_t* inlineQos;
	if(expectsInlineQos)
		inlineQos = &Pub->ParamList;
	else
		inlineQos = NULL;
	CDRMessage::initCDRMsg(submsg);
	RTPSMessageCreator::createSubmessageData(submsg,change,topicKind,ReaderId,inlineQos);
}


void RTPSWriter::sendChangesList(std::vector<CacheChange_t*>* changes,
		std::vector<Locator_t>* unicast,std::vector<Locator_t>* multicast,
		bool expectsInlineQos,EntityId_t ReaderId)
{
	pDebugInfo("Sending relevant changes as data messages" << endl);
	std::vector<Locator_t>::iterator lit;

	std::vector<CacheChange_t*>::iterator cit;
	cit = changes->begin();

	uint16_t data_msg_size = 0;
	uint16_t change_n = 1;
	//FIRST SUBMESSAGE
	DataSubM(&rtpsw_submessage, expectsInlineQos,*cit,ReaderId);
	data_msg_size = rtpsw_submessage.length;
	if(data_msg_size+RTPSMESSAGE_HEADER_SIZE > RTPSMESSAGE_MAX_SIZE)
	{
		pError("The Data messages are larger than max size, fragmentation needed" << endl);
	}
	bool first = true;
	do
	{
		CDRMessage::initCDRMsg(&rtpsw_fullmsg);
		CDRMessage::appendMsg(&rtpsw_fullmsg,&rtpsw_header);
		RTPSMessageCreator::createSubmessageInfoTS_Now(&rtpsw_fullmsg,false);
		if(first)
		{
			CDRMessage::appendMsg(&rtpsw_fullmsg,&rtpsw_submessage);
			first = false;
		}
		while(rtpsw_fullmsg.length + data_msg_size < rtpsw_fullmsg.max_size
				&& (change_n + 1) <= changes->size()) //another one fits in the full message
		{
			change_n++;
			cit++;
			DataSubM(&rtpsw_fullmsg, expectsInlineQos,*cit,ReaderId);
			CDRMessage::appendMsg(&rtpsw_fullmsg,&rtpsw_fullmsg);
		}
		if(unicast!=NULL)
		{
			for(lit = unicast->begin();lit!=unicast->end();lit++)
				participant->threadSend.sendSync(&rtpsw_fullmsg,&(*lit));
		}

		if(multicast!=NULL)
		{
			for(lit = multicast->begin();lit!=multicast->end();lit++)
				participant->threadSend.sendSync(&rtpsw_fullmsg,&(*lit));
		}

	}while(change_n < changes->size()); //There is still a message to add
}

void RTPSWriter::sendChangesList(std::vector<CacheChange_t*>* changes,Locator_t* loc,
		bool expectsInlineQos,EntityId_t ReaderId)
{
	pDebugInfo("Sending relevant changes as data messages" << endl);
	std::vector<Locator_t>::iterator lit;

	std::vector<CacheChange_t*>::iterator cit;
	cit = changes->begin();

	uint16_t data_msg_size = 0;
	uint16_t change_n = 1;
	//FIRST SUBMESSAGE
	DataSubM(&rtpsw_submessage, expectsInlineQos,*cit,ReaderId);
	data_msg_size = rtpsw_submessage.length;
	if(data_msg_size+RTPSMESSAGE_HEADER_SIZE > RTPSMESSAGE_MAX_SIZE)
	{
		pError("The Data messages are larger than max size, fragmentation needed" << endl);
	}
	bool first = true;
	do
	{
		CDRMessage::initCDRMsg(&rtpsw_fullmsg);
		CDRMessage::appendMsg(&rtpsw_fullmsg,&rtpsw_header);
		RTPSMessageCreator::createSubmessageInfoTS_Now(&rtpsw_fullmsg,false);
		if(first)
		{
			CDRMessage::appendMsg(&rtpsw_fullmsg,&rtpsw_submessage);
			first = false;
		}
		while(rtpsw_fullmsg.length + data_msg_size < rtpsw_fullmsg.max_size
				&& (change_n + 1) <= changes->size()) //another one fits in the full message
		{
			change_n++;
			cit++;
			DataSubM(&rtpsw_fullmsg, expectsInlineQos,*cit,ReaderId);
			CDRMessage::appendMsg(&rtpsw_fullmsg,&rtpsw_fullmsg);
		}
		participant->threadSend.sendSync(&rtpsw_fullmsg,loc);
	}while(change_n < changes->size()); //There is still a message to add
}













} /* namespace rtps */
} /* namespace eprosima */

