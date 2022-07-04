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

#ifndef CLMTL_IMAGE_H
#define CLMTL_IMAGE_H

#include <Metal/Metal.hpp>

#include "Memory.h"

namespace cml {

class Image : public Memory {
public:
    static Image *DownCast(cl_mem image);

public:
    Image(Context *context, cl_mem_flags flags, const cl_image_format &format, cl_mem_object_type type, size_t width,
          size_t height, size_t depth);
    ~Image() override;
    void *Map() override;
    void Unmap() override;
    cl_image_format GetFormat() const;
    size_t GetWidth() const;
    size_t GetHeight() const;
    size_t GetDepth() const;
    MTL::Texture *GetTexture() const;

private:
    cl_image_format mFormat;
    size_t mWidth;
    size_t mHeight;
    size_t mDepth;
    MTL::Texture *mTexture;

    void InitTexture();
};

} //namespace cml

#endif //CLMTL_IMAGE_H
