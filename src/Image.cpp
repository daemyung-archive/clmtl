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

#include "Image.h"

#include "Dispatch.h"
#include "Util.h"
#include "Device.h"

namespace cml {

auto ConvertToTextureType(cl_mem_object_type type) {
    switch (type) {
        case CL_MEM_OBJECT_IMAGE2D:
            return MTL::TextureType2D;
        case CL_MEM_OBJECT_IMAGE3D:
            return MTL::TextureType3D;
        default:
            throw std::exception();
    }
}

auto ConvertToPixelFormat(const cl_image_format &format) {
    switch (format.image_channel_order) {
        case CL_R:
        case CL_A:
            switch (format.image_channel_data_type) {
                case CL_SNORM_INT8:
                    return MTL::PixelFormatR8Snorm;
                case CL_SNORM_INT16:
                    return MTL::PixelFormatR16Snorm;
                case CL_UNORM_INT8:
                    return MTL::PixelFormatR8Unorm;
                case CL_UNORM_INT16:
                    return MTL::PixelFormatR16Unorm;
                case CL_SIGNED_INT8:
                    return MTL::PixelFormatR8Sint;
                case CL_SIGNED_INT16:
                    return MTL::PixelFormatR16Sint;
                case CL_SIGNED_INT32:
                    return MTL::PixelFormatR32Sint;
                case CL_UNSIGNED_INT8:
                    return MTL::PixelFormatR8Uint;
                case CL_UNSIGNED_INT16:
                    return MTL::PixelFormatR16Uint;
                case CL_UNSIGNED_INT32:
                    return MTL::PixelFormatR32Uint;
                case CL_HALF_FLOAT:
                    return MTL::PixelFormatR16Float;
                case CL_FLOAT:
                    return MTL::PixelFormatR32Float;
            }
            break;
        case CL_RGBA:
            switch (format.image_channel_data_type) {
                case CL_SNORM_INT8:
                    return MTL::PixelFormatRGBA8Snorm;
                case CL_SNORM_INT16:
                    return MTL::PixelFormatRGBA16Snorm;
                case CL_UNORM_INT8:
                    return MTL::PixelFormatRGBA8Unorm;
                case CL_UNORM_INT16:
                    return MTL::PixelFormatRGBA16Unorm;
                case CL_SIGNED_INT8:
                    return MTL::PixelFormatRGBA8Sint;
                case CL_SIGNED_INT16:
                    return MTL::PixelFormatRGBA16Sint;
                case CL_SIGNED_INT32:
                    return MTL::PixelFormatRGBA32Sint;
                case CL_UNSIGNED_INT8:
                    return MTL::PixelFormatRGBA8Uint;
                case CL_UNSIGNED_INT16:
                    return MTL::PixelFormatRGBA16Uint;
                case CL_UNSIGNED_INT32:
                    return MTL::PixelFormatRGBA32Uint;
                case CL_HALF_FLOAT:
                    return MTL::PixelFormatRGBA16Float;
                case CL_FLOAT:
                    return MTL::PixelFormatRGBA32Float;
            }
            break;
        case CL_BGRA:
            switch (format.image_channel_data_type) {
                case CL_UNORM_INT8:
                    return MTL::PixelFormatBGRA8Unorm;
            }
            break;
    }

    throw std::exception();
}

auto ConvertToTextureUsage(cl_mem_flags flags) {
    if (Util::TestAnyFlagSet(flags, CL_MEM_WRITE_ONLY)) {
        return MTL::TextureUsageShaderWrite;
    }

    if (Util::TestAnyFlagSet(flags, CL_MEM_READ_ONLY)) {
        return MTL::TextureUsageShaderRead;
    }

    return MTL::TextureUsageUnknown;
}

Image *Image::DownCast(cl_mem image) {
    return (Image *) image;
}

Image::Image(Context *context, cl_mem_flags flags, const cl_image_format &format, cl_mem_object_type type,
             size_t width, size_t height, size_t depth)
    : Memory{context, flags, type}, mFormat{format}, mWidth{width}, mHeight{height}, mDepth{depth}, mTexture{nullptr} {
    InitTexture();
}

Image::~Image() {
    mTexture->release();
}

void *Image::Map() {
    return nullptr;
}

void Image::Unmap() {
}

cl_image_format Image::GetFormat() const {
    return mFormat;
}

size_t Image::GetWidth() const {
    return mWidth;
}

size_t Image::GetHeight() const {
    return mHeight;
}

size_t Image::GetDepth() const {
    return mDepth;
}

MTL::Texture *Image::GetTexture() const {
    return mTexture;
}

void Image::InitTexture() {
    auto descriptor = MTL::TextureDescriptor::alloc()->init();
    assert(descriptor);

    descriptor->setTextureType(ConvertToTextureType(mType));
    descriptor->setPixelFormat(ConvertToPixelFormat(mFormat));
    descriptor->setWidth(mWidth);
    descriptor->setHeight(mHeight);
    descriptor->setDepth(mDepth);
    descriptor->setResourceOptions(MTL::ResourceStorageModePrivate);
    descriptor->setUsage(ConvertToTextureUsage(mFlags));

    mTexture = Device::GetSingleton()->GetDevice()->newTexture(descriptor);
    assert(mTexture);
    mSize = mTexture->allocatedSize();

    descriptor->release();
}

} //namespace cml
