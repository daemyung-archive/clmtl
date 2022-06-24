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

#include "Context.h"

#include "Dispatch.h"
#include "Util.h"
#include "Device.h"

namespace cml {

Context *Context::DownCast(cl_context context) {
    return (Context *) context;
}

Context::Context()
    : _cl_context{Dispatch::GetTable()}, Object{}, mDevice{Device::GetSingleton()}, mSupportedImageFormats{} {
    InitSupportedImageFormats();
}

Device *Context::GetDevice() const {
    return mDevice;
}

std::vector<cl_image_format> Context::GetSupportedImageFormats() const {
    return mSupportedImageFormats;
}

void Context::InitSupportedImageFormats() {
    for (auto format : mDevice->GetSupportedPixelFormats()) {
        mSupportedImageFormats.push_back({Util::ConvertToChannelOrder(format), Util::ConvertToChannelType(format)});
    }
}

} //namespace cml
