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

/*!
 * @file annotationsPubSubTypes.cpp
 * This header file contains the implementation of the serialization functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#include "annotationsPubSubTypes.hpp"

#include <fastdds/dds/log/Log.hpp>
#include <fastdds/rtps/common/CdrSerialization.hpp>

#include "annotationsCdrAux.hpp"
#include "annotationsTypeObjectSupport.hpp"

using SerializedPayload_t = eprosima::fastdds::rtps::SerializedPayload_t;
using InstanceHandle_t = eprosima::fastdds::rtps::InstanceHandle_t;
using DataRepresentationId_t = eprosima::fastdds::dds::DataRepresentationId_t;

AnnotatedStructPubSubType::AnnotatedStructPubSubType()
{
    setName("AnnotatedStruct");
    uint32_t type_size =
#if FASTCDR_VERSION_MAJOR == 1
        static_cast<uint32_t>(AnnotatedStruct::getMaxCdrSerializedSize());
#else
        AnnotatedStruct_max_cdr_typesize;
#endif
    type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
    m_typeSize = type_size + 4; /*encapsulation*/
    m_isGetKeyDefined = false;
    uint32_t keyLength = AnnotatedStruct_max_key_cdr_typesize > 16 ? AnnotatedStruct_max_key_cdr_typesize : 16;
    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
    memset(m_keyBuffer, 0, keyLength);
}

AnnotatedStructPubSubType::~AnnotatedStructPubSubType()
{
    if (m_keyBuffer != nullptr)
    {
        free(m_keyBuffer);
    }
}

bool AnnotatedStructPubSubType::serialize(
        const void* const data,
        SerializedPayload_t* payload,
        DataRepresentationId_t data_representation)
{
    const AnnotatedStruct* p_type = static_cast<const AnnotatedStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size);
    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
            eprosima::fastcdr::CdrVersion::XCDRv1 : eprosima::fastcdr::CdrVersion::XCDRv2);
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
#if FASTCDR_VERSION_MAJOR > 1
    ser.set_encoding_flag(
        data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
        eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR  :
        eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2);
#endif // FASTCDR_VERSION_MAJOR > 1

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        // Serialize the object.
        ser << *p_type;
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    // Get the serialized length
#if FASTCDR_VERSION_MAJOR == 1
    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength());
#else
    payload->length = static_cast<uint32_t>(ser.get_serialized_data_length());
#endif // FASTCDR_VERSION_MAJOR == 1
    return true;
}

bool AnnotatedStructPubSubType::deserialize(
        SerializedPayload_t* payload,
        void* data)
{
    try
    {
        // Convert DATA to pointer of your type
        AnnotatedStruct* p_type = static_cast<AnnotatedStruct*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length);

        // Object that deserializes the data.
        eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN
#if FASTCDR_VERSION_MAJOR == 1
                , eprosima::fastcdr::Cdr::CdrType::DDS_CDR
#endif // FASTCDR_VERSION_MAJOR == 1
                );

        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

        // Deserialize the object.
        deser >> *p_type;
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

std::function<uint32_t()> AnnotatedStructPubSubType::getSerializedSizeProvider(
        const void* const data,
        DataRepresentationId_t data_representation)
{
    return [data, data_representation]() -> uint32_t
           {
#if FASTCDR_VERSION_MAJOR == 1
               static_cast<void>(data_representation);
               return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<AnnotatedStruct*>(data))) +
                      4u /*encapsulation*/;
#else
               try
               {
                   eprosima::fastcdr::CdrSizeCalculator calculator(
                       data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                       eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
                   size_t current_alignment {0};
                   return static_cast<uint32_t>(calculator.calculate_serialized_size(
                               *static_cast<const AnnotatedStruct*>(data), current_alignment)) +
                           4u /*encapsulation*/;
               }
               catch (eprosima::fastcdr::exception::Exception& /*exception*/)
               {
                   return 0;
               }
#endif // FASTCDR_VERSION_MAJOR == 1
           };
}

void* AnnotatedStructPubSubType::createData()
{
    return reinterpret_cast<void*>(new AnnotatedStruct());
}

void AnnotatedStructPubSubType::deleteData(
        void* data)
{
    delete(reinterpret_cast<AnnotatedStruct*>(data));
}

bool AnnotatedStructPubSubType::getKey(
        const void* const data,
        InstanceHandle_t* handle,
        bool force_md5)
{
    if (!m_isGetKeyDefined)
    {
        return false;
    }

    const AnnotatedStruct* p_type = static_cast<const AnnotatedStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),
            AnnotatedStruct_max_key_cdr_typesize);

    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv1);
#if FASTCDR_VERSION_MAJOR == 1
    p_type->serializeKey(ser);
#else
    eprosima::fastcdr::serialize_key(ser, *p_type);
#endif // FASTCDR_VERSION_MAJOR == 1
    if (force_md5 || AnnotatedStruct_max_key_cdr_typesize > 16)
    {
        m_md5.init();
#if FASTCDR_VERSION_MAJOR == 1
        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
#else
        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.get_serialized_data_length()));
#endif // FASTCDR_VERSION_MAJOR == 1
        m_md5.finalize();
        for (uint8_t i = 0; i < 16; ++i)
        {
            handle->value[i] = m_md5.digest[i];
        }
    }
    else
    {
        for (uint8_t i = 0; i < 16; ++i)
        {
            handle->value[i] = m_keyBuffer[i];
        }
    }
    return true;
}

void AnnotatedStructPubSubType::register_type_object_representation()
{
    register_AnnotatedStruct_type_identifier(type_identifiers_);
}

EmptyAnnotatedStructPubSubType::EmptyAnnotatedStructPubSubType()
{
    setName("EmptyAnnotatedStruct");
    uint32_t type_size =
#if FASTCDR_VERSION_MAJOR == 1
        static_cast<uint32_t>(EmptyAnnotatedStruct::getMaxCdrSerializedSize());
#else
        EmptyAnnotatedStruct_max_cdr_typesize;
#endif
    type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
    m_typeSize = type_size + 4; /*encapsulation*/
    m_isGetKeyDefined = false;
    uint32_t keyLength = EmptyAnnotatedStruct_max_key_cdr_typesize > 16 ? EmptyAnnotatedStruct_max_key_cdr_typesize : 16;
    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
    memset(m_keyBuffer, 0, keyLength);
}

EmptyAnnotatedStructPubSubType::~EmptyAnnotatedStructPubSubType()
{
    if (m_keyBuffer != nullptr)
    {
        free(m_keyBuffer);
    }
}

bool EmptyAnnotatedStructPubSubType::serialize(
        const void* const data,
        SerializedPayload_t* payload,
        DataRepresentationId_t data_representation)
{
    const EmptyAnnotatedStruct* p_type = static_cast<const EmptyAnnotatedStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size);
    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
            eprosima::fastcdr::CdrVersion::XCDRv1 : eprosima::fastcdr::CdrVersion::XCDRv2);
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
#if FASTCDR_VERSION_MAJOR > 1
    ser.set_encoding_flag(
        data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
        eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR  :
        eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2);
#endif // FASTCDR_VERSION_MAJOR > 1

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        // Serialize the object.
        ser << *p_type;
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    // Get the serialized length
#if FASTCDR_VERSION_MAJOR == 1
    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength());
#else
    payload->length = static_cast<uint32_t>(ser.get_serialized_data_length());
#endif // FASTCDR_VERSION_MAJOR == 1
    return true;
}

bool EmptyAnnotatedStructPubSubType::deserialize(
        SerializedPayload_t* payload,
        void* data)
{
    try
    {
        // Convert DATA to pointer of your type
        EmptyAnnotatedStruct* p_type = static_cast<EmptyAnnotatedStruct*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length);

        // Object that deserializes the data.
        eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN
#if FASTCDR_VERSION_MAJOR == 1
                , eprosima::fastcdr::Cdr::CdrType::DDS_CDR
#endif // FASTCDR_VERSION_MAJOR == 1
                );

        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

        // Deserialize the object.
        deser >> *p_type;
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

std::function<uint32_t()> EmptyAnnotatedStructPubSubType::getSerializedSizeProvider(
        const void* const data,
        DataRepresentationId_t data_representation)
{
    return [data, data_representation]() -> uint32_t
           {
#if FASTCDR_VERSION_MAJOR == 1
               static_cast<void>(data_representation);
               return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<EmptyAnnotatedStruct*>(data))) +
                      4u /*encapsulation*/;
#else
               try
               {
                   eprosima::fastcdr::CdrSizeCalculator calculator(
                       data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                       eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
                   size_t current_alignment {0};
                   return static_cast<uint32_t>(calculator.calculate_serialized_size(
                               *static_cast<const EmptyAnnotatedStruct*>(data), current_alignment)) +
                           4u /*encapsulation*/;
               }
               catch (eprosima::fastcdr::exception::Exception& /*exception*/)
               {
                   return 0;
               }
#endif // FASTCDR_VERSION_MAJOR == 1
           };
}

void* EmptyAnnotatedStructPubSubType::createData()
{
    return reinterpret_cast<void*>(new EmptyAnnotatedStruct());
}

void EmptyAnnotatedStructPubSubType::deleteData(
        void* data)
{
    delete(reinterpret_cast<EmptyAnnotatedStruct*>(data));
}

bool EmptyAnnotatedStructPubSubType::getKey(
        const void* const data,
        InstanceHandle_t* handle,
        bool force_md5)
{
    if (!m_isGetKeyDefined)
    {
        return false;
    }

    const EmptyAnnotatedStruct* p_type = static_cast<const EmptyAnnotatedStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),
            EmptyAnnotatedStruct_max_key_cdr_typesize);

    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv1);
#if FASTCDR_VERSION_MAJOR == 1
    p_type->serializeKey(ser);
#else
    eprosima::fastcdr::serialize_key(ser, *p_type);
#endif // FASTCDR_VERSION_MAJOR == 1
    if (force_md5 || EmptyAnnotatedStruct_max_key_cdr_typesize > 16)
    {
        m_md5.init();
#if FASTCDR_VERSION_MAJOR == 1
        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
#else
        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.get_serialized_data_length()));
#endif // FASTCDR_VERSION_MAJOR == 1
        m_md5.finalize();
        for (uint8_t i = 0; i < 16; ++i)
        {
            handle->value[i] = m_md5.digest[i];
        }
    }
    else
    {
        for (uint8_t i = 0; i < 16; ++i)
        {
            handle->value[i] = m_keyBuffer[i];
        }
    }
    return true;
}

void EmptyAnnotatedStructPubSubType::register_type_object_representation()
{
    register_EmptyAnnotatedStruct_type_identifier(type_identifiers_);
}


BasicAnnotationsStructPubSubType::BasicAnnotationsStructPubSubType()
{
    setName("BasicAnnotationsStruct");
    uint32_t type_size =
#if FASTCDR_VERSION_MAJOR == 1
        static_cast<uint32_t>(BasicAnnotationsStruct::getMaxCdrSerializedSize());
#else
        BasicAnnotationsStruct_max_cdr_typesize;
#endif
    type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
    m_typeSize = type_size + 4; /*encapsulation*/
    m_isGetKeyDefined = false;
    uint32_t keyLength = BasicAnnotationsStruct_max_key_cdr_typesize > 16 ? BasicAnnotationsStruct_max_key_cdr_typesize : 16;
    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
    memset(m_keyBuffer, 0, keyLength);
}

BasicAnnotationsStructPubSubType::~BasicAnnotationsStructPubSubType()
{
    if (m_keyBuffer != nullptr)
    {
        free(m_keyBuffer);
    }
}

bool BasicAnnotationsStructPubSubType::serialize(
        const void* const data,
        SerializedPayload_t* payload,
        DataRepresentationId_t data_representation)
{
    const BasicAnnotationsStruct* p_type = static_cast<const BasicAnnotationsStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size);
    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
            eprosima::fastcdr::CdrVersion::XCDRv1 : eprosima::fastcdr::CdrVersion::XCDRv2);
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
#if FASTCDR_VERSION_MAJOR > 1
    ser.set_encoding_flag(
        data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
        eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR  :
        eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2);
#endif // FASTCDR_VERSION_MAJOR > 1

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        // Serialize the object.
        ser << *p_type;
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    // Get the serialized length
#if FASTCDR_VERSION_MAJOR == 1
    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength());
#else
    payload->length = static_cast<uint32_t>(ser.get_serialized_data_length());
#endif // FASTCDR_VERSION_MAJOR == 1
    return true;
}

bool BasicAnnotationsStructPubSubType::deserialize(
        SerializedPayload_t* payload,
        void* data)
{
    try
    {
        // Convert DATA to pointer of your type
        BasicAnnotationsStruct* p_type = static_cast<BasicAnnotationsStruct*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length);

        // Object that deserializes the data.
        eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN
#if FASTCDR_VERSION_MAJOR == 1
                , eprosima::fastcdr::Cdr::CdrType::DDS_CDR
#endif // FASTCDR_VERSION_MAJOR == 1
                );

        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

        // Deserialize the object.
        deser >> *p_type;
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

std::function<uint32_t()> BasicAnnotationsStructPubSubType::getSerializedSizeProvider(
        const void* const data,
        DataRepresentationId_t data_representation)
{
    return [data, data_representation]() -> uint32_t
           {
#if FASTCDR_VERSION_MAJOR == 1
               static_cast<void>(data_representation);
               return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<BasicAnnotationsStruct*>(data))) +
                      4u /*encapsulation*/;
#else
               try
               {
                   eprosima::fastcdr::CdrSizeCalculator calculator(
                       data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                       eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
                   size_t current_alignment {0};
                   return static_cast<uint32_t>(calculator.calculate_serialized_size(
                               *static_cast<const BasicAnnotationsStruct*>(data), current_alignment)) +
                           4u /*encapsulation*/;
               }
               catch (eprosima::fastcdr::exception::Exception& /*exception*/)
               {
                   return 0;
               }
#endif // FASTCDR_VERSION_MAJOR == 1
           };
}

void* BasicAnnotationsStructPubSubType::createData()
{
    return reinterpret_cast<void*>(new BasicAnnotationsStruct());
}

void BasicAnnotationsStructPubSubType::deleteData(
        void* data)
{
    delete(reinterpret_cast<BasicAnnotationsStruct*>(data));
}

bool BasicAnnotationsStructPubSubType::getKey(
        const void* const data,
        InstanceHandle_t* handle,
        bool force_md5)
{
    if (!m_isGetKeyDefined)
    {
        return false;
    }

    const BasicAnnotationsStruct* p_type = static_cast<const BasicAnnotationsStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),
            BasicAnnotationsStruct_max_key_cdr_typesize);

    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv1);
#if FASTCDR_VERSION_MAJOR == 1
    p_type->serializeKey(ser);
#else
    eprosima::fastcdr::serialize_key(ser, *p_type);
#endif // FASTCDR_VERSION_MAJOR == 1
    if (force_md5 || BasicAnnotationsStruct_max_key_cdr_typesize > 16)
    {
        m_md5.init();
#if FASTCDR_VERSION_MAJOR == 1
        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
#else
        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.get_serialized_data_length()));
#endif // FASTCDR_VERSION_MAJOR == 1
        m_md5.finalize();
        for (uint8_t i = 0; i < 16; ++i)
        {
            handle->value[i] = m_md5.digest[i];
        }
    }
    else
    {
        for (uint8_t i = 0; i < 16; ++i)
        {
            handle->value[i] = m_keyBuffer[i];
        }
    }
    return true;
}

void BasicAnnotationsStructPubSubType::register_type_object_representation()
{
    register_BasicAnnotationsStruct_type_identifier(type_identifiers_);
}


// Include auxiliary functions like for serializing/deserializing.
#include "annotationsCdrAux.ipp"
