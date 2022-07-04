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

#include "Memory.h"

#include "Dispatch.h"

namespace cml {

Memory *Memory::DownCast(cl_mem memory) {
    return (Memory *) memory;
}

Memory::Memory(Context *context, cl_mem_flags flags)
    : _cl_mem{Dispatch::GetTable()}, Object{}, mContext{context}, mFlags{flags}, mMapCount{0} {
}

Context *Memory::GetContext() const {
    return mContext;
}

cl_mem_flags Memory::GetFlags() const {
    return mFlags;
}

cl_uint Memory::GetMapCount() const {
    return mMapCount;
}

} //namespace cml
