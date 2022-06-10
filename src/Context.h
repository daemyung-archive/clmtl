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

#ifndef CLMTL_CONTEXT_H
#define CLMTL_CONTEXT_H

#include <CL/cl_icd.h>

#include "Object.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cl_context {
    cl_icd_dispatch *Dispatch;
};

#ifdef __cplusplus
} //extern "C"
#endif

namespace cml {

class Device;

class Context : public _cl_context, public Object {
public:
    static Context *DownCast(cl_context context);

public:
    Context();
    Device *GetDevice() const;

private:
    Device *mDevice;
};

} //namespace cml

#endif //CLMTL_CONTEXT_H
