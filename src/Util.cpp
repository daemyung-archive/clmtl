/***********************************************************************************************************************
* Copyright (c) 2022-2022 Daemyung Jang.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***********************************************************************************************************************/

#include "Util.h"

namespace cml {

bool Util::TestAnyFlagSet(uint64_t bitset, uint64_t test) {
    return (bitset & test) != 0;
}

intptr_t Util::ReadProperty(const cl_context_properties *properties, uint64_t key) {
    for (auto iter = properties; *iter != 0; iter += 2) {
        if (*iter == key) {
            return *(iter + 1);
        }
    }
    return 0;
}

size_t Util::GetChannelSize(cl_channel_order order) {
    switch (order) {
        case CL_R:
        case CL_A:
        case CL_INTENSITY:
        case CL_LUMINANCE:
            return 1;
        case CL_RG:
        case CL_RA:
            return 2;
        case CL_RGB:
            return 3;
        case CL_RGBA:
        case CL_BGRA:
        case CL_ARGB:
            return 4;
        default:
            return 0;
    }
}

std::array<size_t, 3> Util::ConvertToSize(cl_uint dim, const size_t *size) {
    return {size[0], dim > 1 ? size[1] : 1, dim > 3 ? size[2] : 1};
}

} //namespace cml
