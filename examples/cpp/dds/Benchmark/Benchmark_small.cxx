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
 * @file Benchmark_small.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "Benchmark_small.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

#define BenchMarkSmall_max_cdr_typesize 16388ULL;
#define BenchMarkSmall_max_key_cdr_typesize 0ULL;

BenchMarkSmall::BenchMarkSmall()
{
    // char m_array
    memset(&m_array, 0, (16384) * 1);
    // unsigned long m_index
    m_index = 0;

}

BenchMarkSmall::~BenchMarkSmall()
{


}

BenchMarkSmall::BenchMarkSmall(
        const BenchMarkSmall& x)
{
    m_array = x.m_array;
    m_index = x.m_index;
}

BenchMarkSmall::BenchMarkSmall(
        BenchMarkSmall&& x) noexcept 
{
    m_array = std::move(x.m_array);
    m_index = x.m_index;
}

BenchMarkSmall& BenchMarkSmall::operator =(
        const BenchMarkSmall& x)
{

    m_array = x.m_array;
    m_index = x.m_index;

    return *this;
}

BenchMarkSmall& BenchMarkSmall::operator =(
        BenchMarkSmall&& x) noexcept
{

    m_array = std::move(x.m_array);
    m_index = x.m_index;

    return *this;
}

bool BenchMarkSmall::operator ==(
        const BenchMarkSmall& x) const
{

    return (m_array == x.m_array && m_index == x.m_index);
}

bool BenchMarkSmall::operator !=(
        const BenchMarkSmall& x) const
{
    return !(*this == x);
}

size_t BenchMarkSmall::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    static_cast<void>(current_alignment);
    return BenchMarkSmall_max_cdr_typesize;
}

size_t BenchMarkSmall::getCdrSerializedSize(
        const BenchMarkSmall& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += ((16384) * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void BenchMarkSmall::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_array;

    scdr << m_index;

}

void BenchMarkSmall::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_array;

    dcdr >> m_index;
}

/*!
 * @brief This function copies the value in member array
 * @param _array New value to be copied in member array
 */
void BenchMarkSmall::array(
        const std::array<char, 16384>& _array)
{
    m_array = _array;
}

/*!
 * @brief This function moves the value in member array
 * @param _array New value to be moved in member array
 */
void BenchMarkSmall::array(
        std::array<char, 16384>&& _array)
{
    m_array = std::move(_array);
}

/*!
 * @brief This function returns a constant reference to member array
 * @return Constant reference to member array
 */
const std::array<char, 16384>& BenchMarkSmall::array() const
{
    return m_array;
}

/*!
 * @brief This function returns a reference to member array
 * @return Reference to member array
 */
std::array<char, 16384>& BenchMarkSmall::array()
{
    return m_array;
}
/*!
 * @brief This function sets a value in member index
 * @param _index New value for member index
 */
void BenchMarkSmall::index(
        uint32_t _index)
{
    m_index = _index;
}

/*!
 * @brief This function returns the value of member index
 * @return Value of member index
 */
uint32_t BenchMarkSmall::index() const
{
    return m_index;
}

/*!
 * @brief This function returns a reference to member index
 * @return Reference to member index
 */
uint32_t& BenchMarkSmall::index()
{
    return m_index;
}



size_t BenchMarkSmall::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    static_cast<void>(current_alignment);
    return BenchMarkSmall_max_key_cdr_typesize;
}

bool BenchMarkSmall::isKeyDefined()
{
    return false;
}

void BenchMarkSmall::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
}

