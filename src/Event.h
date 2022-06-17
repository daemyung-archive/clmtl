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

#ifndef CLMTL_EVENT_H
#define CLMTL_EVENT_H

#include <functional>
#include <unordered_map>
#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

#include "Object.h"

struct _cl_event {
    cl_icd_dispatch *Dispatch;
};

namespace cml {

class Context;

class Event : public _cl_event, public Object {
public:
    static Event *DownCast(cl_event event);

public:
    explicit Event(Context *context);
    ~Event() override;
    void SetStatus(cl_int status);
    void SetCallback(cl_int type, std::function<void (cl_int)> callback);
    MTL::Event *GetEvent() const;

private:
    Context *mContext;
    cl_int mStatus;
    MTL::Event *mEvent;
    std::unordered_map<cl_int, std::function<void (cl_int)>> mCallbacks;

    void InitEvent();
};

} //namespace cml

#endif //CLMTL_EVENT_H
