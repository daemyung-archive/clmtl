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

#ifndef CLMTL_BUFFER_H
#define CLMTL_BUFFER_H

#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

#include "Object.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cl_mem {
    cl_icd_dispatch *Dispatch;
};

#ifdef __cplusplus
} //extern "C"
#endif

namespace cml {

class Context;

class Buffer : public _cl_mem, public Object {
public:
    static Buffer *DownCast(cl_mem buffer);

public:
    Buffer(Context *context, cl_mem_flags flags, size_t size);
    Buffer(Context *context, cl_mem_flags flags, const void *data, size_t size);
    ~Buffer() override;
    void *Map();
    void Unmap();
    Context *GetContext() const;
    cl_mem_flags GetMemFlags() const;
    MTL::Buffer *GetBuffer() const;
    size_t GetSize() const;
    cl_uint GetMapCount() const;

private:
    Context *mContext;
    cl_mem_flags mMemFlags;
    MTL::Buffer *mBuffer;
    cl_uint mMapCount;

    void InitBuffer(size_t size);
    void InitBuffer(const void *data, size_t size);
};

} //namespace cml

#endif //CLMTL_BUFFER_H
