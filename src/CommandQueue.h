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

#ifndef CLMTL_COMMAND_QUEUE_H
#define CLMTL_COMMAND_QUEUE_H

#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

#include "Object.h"

struct _cl_command_queue {
    cl_icd_dispatch *Dispatch;
};

namespace clmtl {

class Context;
class Device;

class CommandQueue : public _cl_command_queue, public Object {
public:
    static CommandQueue *DownCast(cl_command_queue commandQueue);

public:
    CommandQueue(Context *context, Device *device);
    ~CommandQueue() override;
    Context *GetContext() const;
    Device *GetDevice() const;

private:
    Context *mContext;
    Device *mDevice;
    MTL::CommandQueue *mCommandQueue;

    void InitCommandQueue();
};

} //namespace clmtl

#endif //CLMTL_COMMAND_QUEUE_H
