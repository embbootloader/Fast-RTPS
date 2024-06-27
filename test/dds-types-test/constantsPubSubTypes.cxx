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
 * @file constantsPubSubTypes.cpp
 * This header file contains the implementation of the serialization functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#include "constantsPubSubTypes.hpp"

#include <fastdds/dds/log/Log.hpp>
#include <fastdds/rtps/common/CdrSerialization.hpp>

#include "constantsCdrAux.hpp"
#include "constantsTypeObjectSupport.hpp"

using SerializedPayload_t = eprosima::fastdds::rtps::SerializedPayload_t;
using InstanceHandle_t = eprosima::fastdds::rtps::InstanceHandle_t;
using DataRepresentationId_t = eprosima::fastdds::dds::DataRepresentationId_t;

namespace const_module1 {
    ModuleConstsLiteralsStructPubSubType::ModuleConstsLiteralsStructPubSubType()
    {
        setName("const_module1::ModuleConstsLiteralsStruct");
        uint32_t type_size =
    #if FASTCDR_VERSION_MAJOR == 1
            static_cast<uint32_t>(ModuleConstsLiteralsStruct::getMaxCdrSerializedSize());
    #else
            const_module1_ModuleConstsLiteralsStruct_max_cdr_typesize;
    #endif
        type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
        m_typeSize = type_size + 4; /*encapsulation*/
        m_isGetKeyDefined = false;
        uint32_t keyLength = const_module1_ModuleConstsLiteralsStruct_max_key_cdr_typesize > 16 ? const_module1_ModuleConstsLiteralsStruct_max_key_cdr_typesize : 16;
        m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
        memset(m_keyBuffer, 0, keyLength);
    }

    ModuleConstsLiteralsStructPubSubType::~ModuleConstsLiteralsStructPubSubType()
    {
        if (m_keyBuffer != nullptr)
        {
            free(m_keyBuffer);
        }
    }

    bool ModuleConstsLiteralsStructPubSubType::serialize(
            const void* const data,
            SerializedPayload_t* payload,
            DataRepresentationId_t data_representation)
    {
        const ModuleConstsLiteralsStruct* p_type = static_cast<const ModuleConstsLiteralsStruct*>(data);

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

    bool ModuleConstsLiteralsStructPubSubType::deserialize(
            SerializedPayload_t* payload,
            void* data)
    {
        try
        {
            // Convert DATA to pointer of your type
            ModuleConstsLiteralsStruct* p_type = static_cast<ModuleConstsLiteralsStruct*>(data);

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

    std::function<uint32_t()> ModuleConstsLiteralsStructPubSubType::getSerializedSizeProvider(
            const void* const data,
            DataRepresentationId_t data_representation)
    {
        return [data, data_representation]() -> uint32_t
               {
    #if FASTCDR_VERSION_MAJOR == 1
                   static_cast<void>(data_representation);
                   return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<ModuleConstsLiteralsStruct*>(data))) +
                          4u /*encapsulation*/;
    #else
                   try
                   {
                       eprosima::fastcdr::CdrSizeCalculator calculator(
                           data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                           eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
                       size_t current_alignment {0};
                       return static_cast<uint32_t>(calculator.calculate_serialized_size(
                                   *static_cast<const ModuleConstsLiteralsStruct*>(data), current_alignment)) +
                               4u /*encapsulation*/;
                   }
                   catch (eprosima::fastcdr::exception::Exception& /*exception*/)
                   {
                       return 0;
                   }
    #endif // FASTCDR_VERSION_MAJOR == 1
               };
    }

    void* ModuleConstsLiteralsStructPubSubType::createData()
    {
        return reinterpret_cast<void*>(new ModuleConstsLiteralsStruct());
    }

    void ModuleConstsLiteralsStructPubSubType::deleteData(
            void* data)
    {
        delete(reinterpret_cast<ModuleConstsLiteralsStruct*>(data));
    }

    bool ModuleConstsLiteralsStructPubSubType::getKey(
            const void* const data,
            InstanceHandle_t* handle,
            bool force_md5)
    {
        if (!m_isGetKeyDefined)
        {
            return false;
        }

        const ModuleConstsLiteralsStruct* p_type = static_cast<const ModuleConstsLiteralsStruct*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),
                const_module1_ModuleConstsLiteralsStruct_max_key_cdr_typesize);

        // Object that serializes the data.
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv1);
    #if FASTCDR_VERSION_MAJOR == 1
        p_type->serializeKey(ser);
    #else
        eprosima::fastcdr::serialize_key(ser, *p_type);
    #endif // FASTCDR_VERSION_MAJOR == 1
        if (force_md5 || const_module1_ModuleConstsLiteralsStruct_max_key_cdr_typesize > 16)
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

    void ModuleConstsLiteralsStructPubSubType::register_type_object_representation()
    {
        register_ModuleConstsLiteralsStruct_type_identifier(type_identifiers_);
    }

} // namespace const_module1

namespace const_module2 {
    Module2ConstsLiteralsStructPubSubType::Module2ConstsLiteralsStructPubSubType()
    {
        setName("const_module2::Module2ConstsLiteralsStruct");
        uint32_t type_size =
    #if FASTCDR_VERSION_MAJOR == 1
            static_cast<uint32_t>(Module2ConstsLiteralsStruct::getMaxCdrSerializedSize());
    #else
            const_module2_Module2ConstsLiteralsStruct_max_cdr_typesize;
    #endif
        type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
        m_typeSize = type_size + 4; /*encapsulation*/
        m_isGetKeyDefined = false;
        uint32_t keyLength = const_module2_Module2ConstsLiteralsStruct_max_key_cdr_typesize > 16 ? const_module2_Module2ConstsLiteralsStruct_max_key_cdr_typesize : 16;
        m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
        memset(m_keyBuffer, 0, keyLength);
    }

    Module2ConstsLiteralsStructPubSubType::~Module2ConstsLiteralsStructPubSubType()
    {
        if (m_keyBuffer != nullptr)
        {
            free(m_keyBuffer);
        }
    }

    bool Module2ConstsLiteralsStructPubSubType::serialize(
            const void* const data,
            SerializedPayload_t* payload,
            DataRepresentationId_t data_representation)
    {
        const Module2ConstsLiteralsStruct* p_type = static_cast<const Module2ConstsLiteralsStruct*>(data);

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

    bool Module2ConstsLiteralsStructPubSubType::deserialize(
            SerializedPayload_t* payload,
            void* data)
    {
        try
        {
            // Convert DATA to pointer of your type
            Module2ConstsLiteralsStruct* p_type = static_cast<Module2ConstsLiteralsStruct*>(data);

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

    std::function<uint32_t()> Module2ConstsLiteralsStructPubSubType::getSerializedSizeProvider(
            const void* const data,
            DataRepresentationId_t data_representation)
    {
        return [data, data_representation]() -> uint32_t
               {
    #if FASTCDR_VERSION_MAJOR == 1
                   static_cast<void>(data_representation);
                   return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<Module2ConstsLiteralsStruct*>(data))) +
                          4u /*encapsulation*/;
    #else
                   try
                   {
                       eprosima::fastcdr::CdrSizeCalculator calculator(
                           data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                           eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
                       size_t current_alignment {0};
                       return static_cast<uint32_t>(calculator.calculate_serialized_size(
                                   *static_cast<const Module2ConstsLiteralsStruct*>(data), current_alignment)) +
                               4u /*encapsulation*/;
                   }
                   catch (eprosima::fastcdr::exception::Exception& /*exception*/)
                   {
                       return 0;
                   }
    #endif // FASTCDR_VERSION_MAJOR == 1
               };
    }

    void* Module2ConstsLiteralsStructPubSubType::createData()
    {
        return reinterpret_cast<void*>(new Module2ConstsLiteralsStruct());
    }

    void Module2ConstsLiteralsStructPubSubType::deleteData(
            void* data)
    {
        delete(reinterpret_cast<Module2ConstsLiteralsStruct*>(data));
    }

    bool Module2ConstsLiteralsStructPubSubType::getKey(
            const void* const data,
            InstanceHandle_t* handle,
            bool force_md5)
    {
        if (!m_isGetKeyDefined)
        {
            return false;
        }

        const Module2ConstsLiteralsStruct* p_type = static_cast<const Module2ConstsLiteralsStruct*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),
                const_module2_Module2ConstsLiteralsStruct_max_key_cdr_typesize);

        // Object that serializes the data.
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv1);
    #if FASTCDR_VERSION_MAJOR == 1
        p_type->serializeKey(ser);
    #else
        eprosima::fastcdr::serialize_key(ser, *p_type);
    #endif // FASTCDR_VERSION_MAJOR == 1
        if (force_md5 || const_module2_Module2ConstsLiteralsStruct_max_key_cdr_typesize > 16)
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

    void Module2ConstsLiteralsStructPubSubType::register_type_object_representation()
    {
        register_Module2ConstsLiteralsStruct_type_identifier(type_identifiers_);
    }

} // namespace const_module2

ConstsLiteralsStructPubSubType::ConstsLiteralsStructPubSubType()
{
    setName("ConstsLiteralsStruct");
    uint32_t type_size =
#if FASTCDR_VERSION_MAJOR == 1
        static_cast<uint32_t>(ConstsLiteralsStruct::getMaxCdrSerializedSize());
#else
        ConstsLiteralsStruct_max_cdr_typesize;
#endif
    type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
    m_typeSize = type_size + 4; /*encapsulation*/
    m_isGetKeyDefined = false;
    uint32_t keyLength = ConstsLiteralsStruct_max_key_cdr_typesize > 16 ? ConstsLiteralsStruct_max_key_cdr_typesize : 16;
    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
    memset(m_keyBuffer, 0, keyLength);
}

ConstsLiteralsStructPubSubType::~ConstsLiteralsStructPubSubType()
{
    if (m_keyBuffer != nullptr)
    {
        free(m_keyBuffer);
    }
}

bool ConstsLiteralsStructPubSubType::serialize(
        const void* const data,
        SerializedPayload_t* payload,
        DataRepresentationId_t data_representation)
{
    const ConstsLiteralsStruct* p_type = static_cast<const ConstsLiteralsStruct*>(data);

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

bool ConstsLiteralsStructPubSubType::deserialize(
        SerializedPayload_t* payload,
        void* data)
{
    try
    {
        // Convert DATA to pointer of your type
        ConstsLiteralsStruct* p_type = static_cast<ConstsLiteralsStruct*>(data);

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

std::function<uint32_t()> ConstsLiteralsStructPubSubType::getSerializedSizeProvider(
        const void* const data,
        DataRepresentationId_t data_representation)
{
    return [data, data_representation]() -> uint32_t
           {
#if FASTCDR_VERSION_MAJOR == 1
               static_cast<void>(data_representation);
               return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<ConstsLiteralsStruct*>(data))) +
                      4u /*encapsulation*/;
#else
               try
               {
                   eprosima::fastcdr::CdrSizeCalculator calculator(
                       data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                       eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
                   size_t current_alignment {0};
                   return static_cast<uint32_t>(calculator.calculate_serialized_size(
                               *static_cast<const ConstsLiteralsStruct*>(data), current_alignment)) +
                           4u /*encapsulation*/;
               }
               catch (eprosima::fastcdr::exception::Exception& /*exception*/)
               {
                   return 0;
               }
#endif // FASTCDR_VERSION_MAJOR == 1
           };
}

void* ConstsLiteralsStructPubSubType::createData()
{
    return reinterpret_cast<void*>(new ConstsLiteralsStruct());
}

void ConstsLiteralsStructPubSubType::deleteData(
        void* data)
{
    delete(reinterpret_cast<ConstsLiteralsStruct*>(data));
}

bool ConstsLiteralsStructPubSubType::getKey(
        const void* const data,
        InstanceHandle_t* handle,
        bool force_md5)
{
    if (!m_isGetKeyDefined)
    {
        return false;
    }

    const ConstsLiteralsStruct* p_type = static_cast<const ConstsLiteralsStruct*>(data);

    // Object that manages the raw buffer.
    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),
            ConstsLiteralsStruct_max_key_cdr_typesize);

    // Object that serializes the data.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv1);
#if FASTCDR_VERSION_MAJOR == 1
    p_type->serializeKey(ser);
#else
    eprosima::fastcdr::serialize_key(ser, *p_type);
#endif // FASTCDR_VERSION_MAJOR == 1
    if (force_md5 || ConstsLiteralsStruct_max_key_cdr_typesize > 16)
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

void ConstsLiteralsStructPubSubType::register_type_object_representation()
{
    register_ConstsLiteralsStruct_type_identifier(type_identifiers_);
}


// Include auxiliary functions like for serializing/deserializing.
#include "constantsCdrAux.ipp"
