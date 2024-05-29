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
 * @file Calculator.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_CALCULATOR_HPP_
#define _FAST_DDS_GENERATED_CALCULATOR_HPP_

#include <cstdint>
#include <utility>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(CALCULATOR_SOURCE)
#define CALCULATOR_DllAPI __declspec( dllexport )
#else
#define CALCULATOR_DllAPI __declspec( dllimport )
#endif // CALCULATOR_SOURCE
#else
#define CALCULATOR_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define CALCULATOR_DllAPI
#endif // _WIN32

/*!
 * @brief This class represents the enumeration OperationType defined by the user in the IDL file.
 * @ingroup Calculator
 */
enum class OperationType : uint32_t
{
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION
};
/*!
 * @brief This class represents the structure RequestType defined by the user in the IDL file.
 * @ingroup Calculator
 */
class RequestType
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport RequestType()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~RequestType()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object RequestType that will be copied.
     */
    eProsima_user_DllExport RequestType(
            const RequestType& x)
    {
                    m_operation = x.m_operation;

                    m_x = x.m_x;

                    m_y = x.m_y;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object RequestType that will be copied.
     */
    eProsima_user_DllExport RequestType(
            RequestType&& x) noexcept
    {
        m_operation = x.m_operation;
        m_x = x.m_x;
        m_y = x.m_y;
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object RequestType that will be copied.
     */
    eProsima_user_DllExport RequestType& operator =(
            const RequestType& x)
    {

                    m_operation = x.m_operation;

                    m_x = x.m_x;

                    m_y = x.m_y;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object RequestType that will be copied.
     */
    eProsima_user_DllExport RequestType& operator =(
            RequestType&& x) noexcept
    {

        m_operation = x.m_operation;
        m_x = x.m_x;
        m_y = x.m_y;
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x RequestType object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const RequestType& x) const
    {
        return (m_operation == x.m_operation &&
           m_x == x.m_x &&
           m_y == x.m_y);
    }

    /*!
     * @brief Comparison operator.
     * @param x RequestType object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const RequestType& x) const
    {
        return !(*this == x);
    }

    /*!
     * @brief This function sets a value in member operation
     * @param _operation New value for member operation
     */
    eProsima_user_DllExport void operation(
            OperationType _operation)
    {
        m_operation = _operation;
    }

    /*!
     * @brief This function returns the value of member operation
     * @return Value of member operation
     */
    eProsima_user_DllExport OperationType operation() const
    {
        return m_operation;
    }

    /*!
     * @brief This function returns a reference to member operation
     * @return Reference to member operation
     */
    eProsima_user_DllExport OperationType& operation()
    {
        return m_operation;
    }


    /*!
     * @brief This function sets a value in member x
     * @param _x New value for member x
     */
    eProsima_user_DllExport void x(
            int32_t _x)
    {
        m_x = _x;
    }

    /*!
     * @brief This function returns the value of member x
     * @return Value of member x
     */
    eProsima_user_DllExport int32_t x() const
    {
        return m_x;
    }

    /*!
     * @brief This function returns a reference to member x
     * @return Reference to member x
     */
    eProsima_user_DllExport int32_t& x()
    {
        return m_x;
    }


    /*!
     * @brief This function sets a value in member y
     * @param _y New value for member y
     */
    eProsima_user_DllExport void y(
            int32_t _y)
    {
        m_y = _y;
    }

    /*!
     * @brief This function returns the value of member y
     * @return Value of member y
     */
    eProsima_user_DllExport int32_t y() const
    {
        return m_y;
    }

    /*!
     * @brief This function returns a reference to member y
     * @return Reference to member y
     */
    eProsima_user_DllExport int32_t& y()
    {
        return m_y;
    }



private:

    OperationType m_operation{OperationType::ADDITION};
    int32_t m_x{0};
    int32_t m_y{0};

};
/*!
 * @brief This class represents the structure ReplyType defined by the user in the IDL file.
 * @ingroup Calculator
 */
class ReplyType
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport ReplyType()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~ReplyType()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object ReplyType that will be copied.
     */
    eProsima_user_DllExport ReplyType(
            const ReplyType& x)
    {
                    m_z = x.m_z;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object ReplyType that will be copied.
     */
    eProsima_user_DllExport ReplyType(
            ReplyType&& x) noexcept
    {
        m_z = x.m_z;
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object ReplyType that will be copied.
     */
    eProsima_user_DllExport ReplyType& operator =(
            const ReplyType& x)
    {

                    m_z = x.m_z;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object ReplyType that will be copied.
     */
    eProsima_user_DllExport ReplyType& operator =(
            ReplyType&& x) noexcept
    {

        m_z = x.m_z;
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x ReplyType object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const ReplyType& x) const
    {
        return (m_z == x.m_z);
    }

    /*!
     * @brief Comparison operator.
     * @param x ReplyType object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const ReplyType& x) const
    {
        return !(*this == x);
    }

    /*!
     * @brief This function sets a value in member z
     * @param _z New value for member z
     */
    eProsima_user_DllExport void z(
            int64_t _z)
    {
        m_z = _z;
    }

    /*!
     * @brief This function returns the value of member z
     * @return Value of member z
     */
    eProsima_user_DllExport int64_t z() const
    {
        return m_z;
    }

    /*!
     * @brief This function returns a reference to member z
     * @return Reference to member z
     */
    eProsima_user_DllExport int64_t& z()
    {
        return m_z;
    }



private:

    int64_t m_z{0};

};

#endif // _FAST_DDS_GENERATED_CALCULATOR_HPP_

