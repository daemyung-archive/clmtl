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

#include "Buffer.h"
#include "Object.h"

namespace cml {

class Context;

class Image : public _cl_mem, public Object {
public:
    static Image *DownCast(cl_mem image);

public:
    Image(Context *context, cl_mem_flags memFlags, const cl_image_format &format, cl_mem_object_type type, size_t width,
          size_t height, size_t depth);
    ~Image() override;
    Context *GetContext() const;
    cl_mem_flags GetMemFlags() const;
    cl_image_format GetFormat() const;
    cl_mem_object_type GetType() const;
    size_t GetWidth() const;
    size_t GetHeight() const;
    size_t GetDepth() const;
    MTL::Texture *GetTexture() const;

private:
    Context *mContext;
    cl_mem_flags mMemFlags;
    cl_image_format mFormat;
    cl_mem_object_type mType;
    size_t mWidth;
    size_t mHeight;
    size_t mDepth;
    MTL::Texture *mTexture;

    void InitTexture();
};

} //namespace cml

#endif //CLMTL_IMAGE_H
