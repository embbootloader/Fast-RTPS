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
 * @file inheritanceCdrAux.hpp
 * This source file contains some definitions of CDR related functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__INHERITANCECDRAUX_HPP
#define FAST_DDS_GENERATED__INHERITANCECDRAUX_HPP

#include "inheritance.hpp"

constexpr uint32_t InnerStructureHelperChild_max_cdr_typesize {32UL};
constexpr uint32_t InnerStructureHelperChild_max_key_cdr_typesize {0UL};

constexpr uint32_t InnerStructureHelperEmptyChild_max_cdr_typesize {12UL};
constexpr uint32_t InnerStructureHelperEmptyChild_max_key_cdr_typesize {0UL};

constexpr uint32_t InnerEmptyStructureHelperChild_max_cdr_typesize {24UL};
constexpr uint32_t InnerEmptyStructureHelperChild_max_key_cdr_typesize {0UL};


constexpr uint32_t InnerStructureHelperChildChild_max_cdr_typesize {48UL};
constexpr uint32_t InnerStructureHelperChildChild_max_key_cdr_typesize {0UL};


constexpr uint32_t InnerStructureHelperEmptyChildChild_max_cdr_typesize {13UL};
constexpr uint32_t InnerStructureHelperEmptyChildChild_max_key_cdr_typesize {0UL};





constexpr uint32_t StructuresInheritanceStruct_max_cdr_typesize {142UL};
constexpr uint32_t StructuresInheritanceStruct_max_key_cdr_typesize {0UL};




constexpr uint32_t BitsetsChildInheritanceStruct_max_cdr_typesize {32UL};
constexpr uint32_t BitsetsChildInheritanceStruct_max_key_cdr_typesize {0UL};









constexpr uint32_t StructAliasInheritanceStruct_max_cdr_typesize {14UL};
constexpr uint32_t StructAliasInheritanceStruct_max_key_cdr_typesize {0UL};


namespace eprosima {
namespace fastcdr {

class Cdr;
class CdrSizeCalculator;

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const InnerStructureHelperChild& data);

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const InnerStructureHelperChildChild& data);

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const InnerStructureHelperEmptyChild& data);

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const InnerStructureHelperEmptyChildChild& data);

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const InnerEmptyStructureHelperChild& data);

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const StructAliasInheritanceStruct& data);

eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const StructuresInheritanceStruct& data);




eProsima_user_DllExport void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const BitsetsChildInheritanceStruct& data);


} // namespace fastcdr
} // namespace eprosima

#endif // FAST_DDS_GENERATED__INHERITANCECDRAUX_HPP

