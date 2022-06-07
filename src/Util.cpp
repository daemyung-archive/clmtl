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

#include "Util.h"

namespace cml {

bool Util::TestAnyFlagSet(uint64_t bitset, uint64_t test) {
    return (bitset & test) != 0;
}

intptr_t Util::ReadProperty(const cl_context_properties *properties, uint64_t key) {
    for (auto iter = properties; *iter != 0; iter += 2) {
        if (*iter == key) {
            return *(iter + 1);
        }
    }
    return 0;
}

} //namespace cml
