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

#ifndef CLMTL_UTIL_H
#define CLMTL_UTIL_H

#include <cstdint>
#include <CL/cl.h>

namespace cml {

class Util {
public:
    static bool TestAnyFlagSet(uint64_t bitset, uint64_t test);
    static intptr_t ReadProperty(const cl_context_properties *properties, uint64_t key);
};

} //namespace cml

#endif //CLMTL_UTIL_H
