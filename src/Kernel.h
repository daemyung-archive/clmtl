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
#include <vector>
#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

#include "Object.h"

class _cl_kernel {
public:
    cl_icd_dispatch *Dispatch;

    explicit _cl_kernel(cl_icd_dispatch *dispatch);
    virtual ~_cl_kernel() = default;
};

namespace cml {

class Context;
class Program;

struct Argument {
    MTL::ArgumentType Type;
};

class Kernel : public _cl_kernel, public Object {
public:
    static Kernel *DownCast(cl_kernel kernel);

public:
    explicit Kernel(Program *program, std::string name);
    ~Kernel() override;
    Context *GetContext() const;
    Program *GetProgram() const;
    std::string GetName() const;
    std::vector<Argument> GetArgumentTable() const;
    size_t GetWorkGroupSize() const;
    size_t GetWorkItemExecutionWidth() const;

private:
    Program *mProgram;
    std::string mName;
    MTL::Function *mFunction;
    MTL::ComputePipelineState *mPipeline;
    std::vector<Argument> mArgumentTable;

    void InitFunction();
    void InitPipeline();
};

} //namespace cml

#endif //CLMTL_KERNEL_H
