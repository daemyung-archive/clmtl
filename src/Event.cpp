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

#include "Event.h"

#include "Dispatch.h"
#include "Context.h"
#include "Device.h"

namespace cml {

Event *Event::DownCast(cl_event event) {
    return (Event *) event;
}

Event::Event(Context *context)
    : _cl_event{Dispatch::GetTable()}, Object{}, mContext{context}, mStatus{-1}, mEvent{nullptr} {
    InitEvent();
}

Event::~Event() {
    mEvent->release();
}

void Event::SetStatus(cl_int status) {
    if (mCallbacks.count(status)) {
        mCallbacks[status](status);
    }

    mStatus = status;
}

void Event::SetCallback(cl_int type, std::function<void (cl_int)> callback) {
    mCallbacks[type] = std::move(callback);
}

MTL::Event *Event::GetEvent() const {
    return mEvent;
}

void Event::InitEvent() {
    auto device = mContext->GetDevice();
    assert(device);

    mEvent = device->GetDevice()->newEvent();
    assert(mEvent);
}

} //namespace cml