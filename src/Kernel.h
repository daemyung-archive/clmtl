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

#ifndef CLMTL_KERNEL_H
#define CLMTL_KERNEL_H

#include <string>
#include <array>
#include <CL/cl_icd.h>

#include "Metal.hpp"
#include "Size.h"
#include "Object.h"
#include "Reflector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cl_kernel {
    cl_icd_dispatch *Dispatch;
};

#ifdef __cplusplus
} //extern "C"
#endif

namespace cml {

class Context;
class Program;
class Buffer;

struct Arg {
    clspv::ArgKind Kind;
    uint32_t Binding;
    union {
        cl_mem Buffer;
        cl_mem Image;
        cl_sampler Sampler;
        uint8_t Data[64];
    };
    uint32_t Size;
};

class Kernel : public _cl_kernel, public Object {
public:
    static Kernel *DownCast(cl_kernel kernel);

public:
    Kernel(Program *program, const std::string &name);
    ~Kernel() override;
    void SetArg(size_t index, const void *value, size_t size);
    Context *GetContext() const;
    Program *GetProgram() const;
    std::string GetName() const;
    MTL::ComputePipelineState *GetPipelineState(const Size &workGroupSize);
    size_t GetWorkGroupSize() const;
    size_t GetWorkItemExecutionWidth() const;
    std::unordered_map<uint32_t, Arg> GetArgTable() const;

private:
    Program *mProgram;
    Reflection mReflection;
    std::string mName;
    std::string mSource;
    std::unordered_map<uint64_t, std::unordered_map<std::string, MTL::ComputePipelineState *>> mPipelineStates;
    std::unordered_map<uint32_t, std::string> mDefines;
    std::unordered_map<uint32_t, Arg> mArgTable;

    void InitSource();
    void InitPipelineState();
    void InitArgTable();
    MTL::Function *CreateFunction(const Size &workGroupSize);
    void AddPipelineState(uint64_t hash, const Size &workGroupSize);
};

} //namespace cml

#endif //CLMTL_KERNEL_H
