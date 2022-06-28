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

size_t Util::GetPixelSize(cl_channel_type type) {
    switch (type) {
        case CL_SNORM_INT8:
        case CL_UNORM_INT8:
        case CL_SIGNED_INT8:
        case CL_UNSIGNED_INT8:
            return 1;
        case CL_SNORM_INT16:
        case CL_UNORM_INT16:
        case CL_UNORM_SHORT_565:
        case CL_UNORM_SHORT_555:
        case CL_SIGNED_INT16:
        case CL_UNSIGNED_INT16:
        case CL_HALF_FLOAT:
            return 2;
        case CL_UNORM_INT24:
            return 3;
        case CL_UNORM_INT_101010:
        case CL_SIGNED_INT32:
        case CL_UNSIGNED_INT32:
        case CL_FLOAT:
        case CL_UNORM_INT_101010_2:
            return 4;
        default:
            throw std::exception();
    }
}

size_t Util::GetFormatSize(const cl_image_format &format) {
    return GetChannelSize(format.image_channel_order) * GetPixelSize(format.image_channel_data_type);
}

Size Util::ConvertToSize(cl_uint dim, const size_t *size) {
    return {size[0], dim > 1 ? size[1] : 0, dim > 2 ? size[2] : 0};
}

cl_channel_order Util::ConvertToChannelOrder(MTL::PixelFormat format) {
    switch (format) {
        case MTL::PixelFormatA8Unorm:
            return CL_A;
        case MTL::PixelFormatR8Unorm:
        case MTL::PixelFormatR8Snorm:
        case MTL::PixelFormatR8Uint:
        case MTL::PixelFormatR8Sint:
        case MTL::PixelFormatR16Unorm:
        case MTL::PixelFormatR16Snorm:
        case MTL::PixelFormatR16Uint:
        case MTL::PixelFormatR16Sint:
        case MTL::PixelFormatR16Float:
        case MTL::PixelFormatR32Uint:
        case MTL::PixelFormatR32Sint:
        case MTL::PixelFormatR32Float:
            return CL_R;
        case MTL::PixelFormatRGBA8Unorm:
        case MTL::PixelFormatRGBA8Snorm:
        case MTL::PixelFormatRGBA8Uint:
        case MTL::PixelFormatRGBA8Sint:
        case MTL::PixelFormatRGBA16Unorm:
        case MTL::PixelFormatRGBA16Snorm:
        case MTL::PixelFormatRGBA16Uint:
        case MTL::PixelFormatRGBA16Sint:
        case MTL::PixelFormatRGBA16Float:
        case MTL::PixelFormatRGBA32Uint:
        case MTL::PixelFormatRGBA32Sint:
        case MTL::PixelFormatRGBA32Float:
            return CL_RGBA;
        case MTL::PixelFormatBGRA8Unorm:
            return CL_BGRA;
        default:
            throw std::exception();
    }
}

cl_channel_type Util::ConvertToChannelType(MTL::PixelFormat format) {
    switch (format) {
        case MTL::PixelFormatA8Unorm:
        case MTL::PixelFormatR8Unorm:
        case MTL::PixelFormatRGBA8Unorm:
        case MTL::PixelFormatBGRA8Unorm:
            return CL_UNORM_INT8;
        case MTL::PixelFormatR8Snorm:
        case MTL::PixelFormatRGBA8Snorm:
            return CL_SNORM_INT8;
        case MTL::PixelFormatR8Uint:
        case MTL::PixelFormatRGBA8Uint:
            return CL_UNSIGNED_INT8;
        case MTL::PixelFormatR8Sint:
        case MTL::PixelFormatRGBA8Sint:
            return CL_SIGNED_INT8;
        case MTL::PixelFormatR16Unorm:
        case MTL::PixelFormatRGBA16Unorm:
            return CL_UNORM_INT16;
        case MTL::PixelFormatR16Snorm:
        case MTL::PixelFormatRGBA16Snorm:
            return CL_SNORM_INT16;
        case MTL::PixelFormatR16Uint:
        case MTL::PixelFormatRGBA16Uint:
            return CL_UNSIGNED_INT16;
        case MTL::PixelFormatR16Sint:
        case MTL::PixelFormatRGBA16Sint:
            return CL_SIGNED_INT16;
        case MTL::PixelFormatR16Float:
        case MTL::PixelFormatRGBA16Float:
            return CL_HALF_FLOAT;
        case MTL::PixelFormatR32Uint:
        case MTL::PixelFormatRGBA32Uint:
            return CL_UNSIGNED_INT32;
        case MTL::PixelFormatR32Sint:
        case MTL::PixelFormatRGBA32Sint:
            return CL_SIGNED_INT32;
        case MTL::PixelFormatR32Float:
        case MTL::PixelFormatRGBA32Float:
            return CL_FLOAT;
        default:
            throw std::exception();
    }
}

} //namespace cml
