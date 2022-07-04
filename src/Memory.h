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

#ifndef CLMTL_MEMORY_H
#define CLMTL_MEMORY_H

#include <CL/cl_icd.h>

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

class Memory : public _cl_mem, public Object {
public:
    static Memory *DownCast(cl_mem memory);

public:
    explicit Memory(Context *context, cl_mem_flags flags);
    virtual void *Map() = 0;
    virtual void Unmap() = 0;
    Context *GetContext() const;
    cl_mem_flags GetFlags() const;
    cl_uint GetMapCount() const;

protected:
    Context *mContext;
    cl_mem_flags mFlags;
    cl_uint mMapCount;
};

} //namespace cml

#endif //CLMTL_MEMORY_H

