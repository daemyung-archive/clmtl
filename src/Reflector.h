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

#ifndef CLMTL_ARG_TABLE_H
#define CLMTL_ARG_TABLE_H

#include <vector>
#include <unordered_map>
#include <clspv/ArgKind.h>

namespace cml {

class Buffer;

struct Binding {
    clspv::ArgKind Kind;
};

class Reflector {
public:
    Reflector() = default;
    explicit Reflector(const std::vector<uint32_t> &binary);
    std::unordered_map<uint32_t, Binding> GetBindingTable() const;

private:
    std::string mReflection;
    std::unordered_map<uint32_t, Binding> mBindingTable;

    void InitBindingTable();
};

} //namespace cml

#endif //CLMTL_ARG_TABLE_H
