// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef FASTDDS_RTPS_WRITER_DELIVERYRETCODE_HPP
#define FASTDDS_RTPS_WRITER_DELIVERYRETCODE_HPP

#include <cstdint>

namespace eprosima {
namespace fastdds {
namespace rtps {

enum class DeliveryRetCode : uint32_t
{
    DELIVERED,
    NOT_DELIVERED,
    EXCEEDED_LIMIT
};

} // namespace rtps
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_RTPS_WRITER_DELIVERYRETCODE_HPP
