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

#ifndef CLMTL_PROGRAM_H
#define CLMTL_PROGRAM_H

#include <vector>
#include <string>
#include <CL/cl_icd.h>

#include "Object.h"
#include "Reflector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cl_program {
    cl_icd_dispatch *Dispatch;
};

#ifdef __cplusplus
} //extern "C"
#endif

namespace cml {

class Context;

class Program : public _cl_program, public Object {
public:
    static Program *DownCast(cl_program program);

public:
    explicit Program(Context *context);
    void AddSource(const std::string &source);
    void Compile();
    void Link(const std::vector<std::vector<uint32_t>> &binaries);
    void Reflect();
    void SetOptions(const std::string &options);
    void SetBinary(const std::vector<uint32_t> &binary);
    Context *GetContext() const;
    std::string GetSource() const;
    std::string GetOptions() const;
    cl_build_status GetBuildStatus() const;
    std::string GetLog() const;
    std::vector<uint32_t> GetBinary() const;
    std::unordered_map<std::string, std::vector<Binding>> GetReflection() const;

private:
    Context *mContext;
    std::string mSource;
    std::string mOptions;
    std::vector<uint32_t> mBinary;
    cl_build_status mBuildStatus;
    std::string mLog;
    std::unordered_map<std::string, std::vector<Binding>> mReflection;
};

} //namespace cml

#endif //CLMTL_PROGRAM_H
