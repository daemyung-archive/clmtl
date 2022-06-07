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

#include "CommandQueue.h"

#include "Dispatch.h"
#include "Context.h"
#include "Device.h"

_cl_command_queue::_cl_command_queue(cl_icd_dispatch *dispatch) :
        Dispatch{dispatch} {
    assert(Dispatch);
}

namespace clmtl {

CommandQueue *CommandQueue::DownCast(cl_command_queue commandQueue) {
    return dynamic_cast<CommandQueue *>(commandQueue);
}

CommandQueue::CommandQueue(Context *context, Device *device)
        : _cl_command_queue{Dispatch::GetTable()}, Object{}, mContext{context}, mDevice{device},
          mCommandQueue{nullptr} {
    InitCommandQueue();
}

CommandQueue::~CommandQueue() {
    mCommandQueue->release();
}

Context *CommandQueue::GetContext() const {
    return mContext;
}

Device *CommandQueue::GetDevice() const {
    return mDevice;
}

void CommandQueue::InitCommandQueue() {
    mCommandQueue = mDevice->GetDevice()->newCommandQueue();
    assert(mCommandQueue);
}

} //namespace clmtl
