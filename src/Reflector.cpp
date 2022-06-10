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

#include "Reflector.h"

#include <spirv_cross/spirv_reflect.hpp>
#include <json/json.hpp>

namespace cml {

auto GetBuffers(const nlohmann::json &json) {
    return json.at("ssbos");
}

auto GetPods(const nlohmann::json &json) {
    return json.at("ubos");
}

size_t GetIndex(nlohmann::json::reference resource) {
    return resource.at("binding").get<uint32_t>();
}

Reflector::Reflector(const std::vector<uint32_t> &binary) :
        mReflection{spirv_cross::CompilerReflection(binary).compile()}, mBindingTable{} {
    InitBindingTable();
}

std::unordered_map<uint32_t, Binding> Reflector::GetBindingTable() const {
    return mBindingTable;
}

void Reflector::InitBindingTable() {
    auto json = nlohmann::json::parse(mReflection);

    try {
        for (auto buffer: GetBuffers(json)) {
            mBindingTable[GetIndex(buffer)].Kind = clspv::ArgKind::Buffer;
        }
    } catch (std::exception &e) {
    }

    try {
        for (auto pod : GetPods(json)) {
            mBindingTable[GetIndex(pod)].Kind = clspv::ArgKind::Pod;
        }
    } catch (std::exception &e) {
    }
}

} //namespace cml
