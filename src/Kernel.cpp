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

#include "Kernel.h"

#include <cassert>
#include <sstream>

#include "Dispatch.h"
#include "Device.h"
#include "Program.h"
#include "LibraryPool.h"

namespace cml {

spirv_cross::MSLSamplerCoord ConvertToSamplerCoord(clspv::SamplerNormalizedCoords normalizedCoords) {
    switch (normalizedCoords) {
        case clspv::CLK_NORMALIZED_COORDS_FALSE:
            return spirv_cross::MSL_SAMPLER_COORD_PIXEL;
        case clspv::CLK_NORMALIZED_COORDS_TRUE:
        case clspv::CLK_NORMALIZED_COORDS_NOT_SET:
            return spirv_cross::MSL_SAMPLER_COORD_NORMALIZED;
        default:
            throw std::exception();
    }
}

spirv_cross::MSLSamplerAddress ConvertToSamplerAddress(clspv::SamplerAddressingMode addressingMode) {
    switch (addressingMode) {
        case clspv::CLK_ADDRESS_NONE:
        case clspv::CLK_ADDRESS_CLAMP_TO_EDGE:
            return spirv_cross::MSL_SAMPLER_ADDRESS_CLAMP_TO_EDGE;
        case clspv::CLK_ADDRESS_CLAMP:
            return spirv_cross::MSL_SAMPLER_ADDRESS_CLAMP_TO_BORDER;
        case clspv::CLK_ADDRESS_MIRRORED_REPEAT:
            return spirv_cross::MSL_SAMPLER_ADDRESS_MIRRORED_REPEAT;
        case clspv::CLK_ADDRESS_REPEAT:
            return spirv_cross::MSL_SAMPLER_ADDRESS_REPEAT;
        default:
            throw std::exception();
    }
}

spirv_cross::MSLSamplerFilter ConvertToSamplerFilter(clspv::SamplerFilterMode filterMode) {
    switch (filterMode) {
        case clspv::CLK_FILTER_NEAREST:
            return spirv_cross::MSL_SAMPLER_FILTER_NEAREST;
        case clspv::CLK_FILTER_LINEAR:
        case clspv::CLK_FILTER_NOT_SET:
            return spirv_cross::MSL_SAMPLER_FILTER_LINEAR;
        default:
            throw std::exception();
    }
}

spirv_cross::MSLConstexprSampler ConvertToConstexprSampler(const LiteralSampler &literalSampler) {
    spirv_cross::MSLConstexprSampler constexprSampler;

    constexprSampler.coord = ConvertToSamplerCoord(literalSampler.NormalizedCoords);
    constexprSampler.s_address = ConvertToSamplerAddress(literalSampler.AddressingMode);
    constexprSampler.r_address = ConvertToSamplerAddress(literalSampler.AddressingMode);
    constexprSampler.t_address = ConvertToSamplerAddress(literalSampler.AddressingMode);
    constexprSampler.min_filter = ConvertToSamplerFilter(literalSampler.FilterMode);
    constexprSampler.mag_filter = ConvertToSamplerFilter(literalSampler.FilterMode);

    return constexprSampler;
}

void KeepResourceBindings(spirv_cross::CompilerMSL &compiler) {
    const auto resources = compiler.get_shader_resources();
    const auto stage = compiler.get_execution_model();

    for (const auto &resource : resources.uniform_buffers) {
        auto descSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        compiler.add_msl_resource_binding({.stage = stage, .desc_set = descSet, .binding = binding,
                                           .msl_buffer = binding, .msl_texture = binding, .msl_sampler = binding});
    }

    for (const auto &resource : resources.storage_buffers) {
        auto descSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        compiler.add_msl_resource_binding({.stage = stage, .desc_set = descSet, .binding = binding,
                                           .msl_buffer = binding, .msl_texture = binding, .msl_sampler = binding});
    }

    for (const auto &resource : resources.storage_images) {
        auto descSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        compiler.add_msl_resource_binding({.stage = stage, .desc_set = descSet, .binding = binding,
                                           .msl_buffer = binding, .msl_texture = binding, .msl_sampler = binding});
    }

    for (auto &resource : resources.sampled_images) {
        auto descSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        compiler.add_msl_resource_binding({.stage = stage, .desc_set = descSet, .binding = binding,
                                           .msl_buffer = binding, .msl_texture = binding, .msl_sampler = binding});
    }

    for (auto &resource : resources.separate_images) {
        auto descSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        compiler.add_msl_resource_binding({.stage = stage, .desc_set = descSet, .binding = binding,
                                           .msl_buffer = binding, .msl_texture = binding, .msl_sampler = binding});
    }

    for (const auto &resource : resources.separate_samplers) {
        auto descSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

        compiler.add_msl_resource_binding({.stage = stage, .desc_set = descSet, .binding = binding,
                                           .msl_buffer = binding, .msl_texture = binding, .msl_sampler = binding});
    }
}

void RemapConstexprSamplers(spirv_cross::CompilerMSL &compiler, const std::vector<LiteralSampler> &literalSamplers) {
    for (auto &literalSampler : literalSamplers) {
        compiler.remap_constexpr_sampler_by_binding(literalSampler.DescSet, literalSampler.Binding,
                                                    ConvertToConstexprSampler(literalSampler));
    }
}

std::string ConvertToString(const std::unordered_map<uint32_t, std::string> &defines) {
    std::stringstream stream;

    for (auto &[ordinal, define] : defines) {
        stream << define;
    }

    return stream.str();
}

uint64_t GetHash(const Size &size) {
    return (size.w << 42) | (size.h << 21) | (size.d << 0);
}

uint32_t GetDimension(const Size &size) {
    uint32_t dimension = 1;

    if (size.h) {
        dimension += 1;
    }

    if (size.d) {
        dimension += 1;
    }

    return dimension;
}

MTL::FunctionConstantValues *CreateConstantValues(const Size &workGroupSize, uint32_t dimension) {
    auto h = std::max(workGroupSize.h, 1ul);
    auto d = std::max(workGroupSize.d, 1ul);
    auto values = MTL::FunctionConstantValues::alloc()->init();

    values->setConstantValue(&dimension, MTL::DataTypeUInt, 3ul);
    values->setConstantValue(&workGroupSize.w, MTL::DataTypeUInt, 0ul);
    values->setConstantValue(&h, MTL::DataTypeUInt, 1ul);
    values->setConstantValue(&d, MTL::DataTypeUInt, 2ul);

    return values;
}

Kernel *Kernel::DownCast(cl_kernel kernel) {
    return (Kernel *) kernel;
}

Kernel::Kernel(Program *program, const std::string &name)
    : _cl_kernel{Dispatch::GetTable()}, Object{}, mProgram{program}, mReflection{program->GetReflection()}, mName{name}
    , mPipelineStates{}, mArgTable{} {
    InitSource();
    InitPipelineState();
    InitArgTable();
}

Kernel::~Kernel() {
    for (auto &[hash, pipelineStates] : mPipelineStates) {
        for (auto &[defines, pipelineState] : pipelineStates) {
            pipelineState->release();
        }
    }
}

void Kernel::SetArg(size_t index, const void *data, size_t size) {
    if (mArgTable[index].Kind != clspv::ArgKind::Local) {
        if (data) {
            memcpy(mArgTable[index].Data, data, size);
        }

        mArgTable[index].Size = size;
    } else {
        std::stringstream stream;

        stream << "#define SPIRV_CROSS_CONSTANT_ID_" << mReflection.Arguments[mName][index].Spec << " "
               << size / mReflection.Arguments[mName][index].Size << "\n";
        mDefines[index] = stream.str();
    }
}

Context *Kernel::GetContext() const {
    return mProgram->GetContext();
}

Program *Kernel::GetProgram() const {
    return mProgram;
}

std::string Kernel::GetName() const {
    return mName;
}

MTL::ComputePipelineState *Kernel::GetPipelineState(const Size &workGroupSize) {
    auto hash = GetHash(workGroupSize);
    auto defines = ConvertToString(mDefines);

    if (!mPipelineStates.count(hash) || !mPipelineStates.at(hash).count(defines)) {
        AddPipelineState(hash, workGroupSize);
    }

    return mPipelineStates[hash][defines];
}

size_t Kernel::GetWorkGroupSize() const {
    return mPipelineStates.at(0).at("")->maxTotalThreadsPerThreadgroup();
}

Size Kernel::GetCompileWorkGroupSize() const {
    if (mReflection.RequiredWorkgroupSizes.contains(mName)) {
        return mReflection.RequiredWorkgroupSizes.at(mName).WorkgroupSize;
    } else {
        return {0, 0, 0};
    }
}

size_t Kernel::GetWorkItemExecutionWidth() const {
    return mPipelineStates.at(0).at("")->threadExecutionWidth();
}

std::unordered_map<uint32_t, Arg> Kernel::GetArgTable() const {
    return mArgTable;
}

void Kernel::InitSource() {
    spirv_cross::CompilerMSL::Options options;

    options.set_msl_version(2, 3);

    spirv_cross::CompilerMSL compiler(mProgram->GetBinary().data(), mProgram->GetBinary().size());

    compiler.set_msl_options(options);
    compiler.set_entry_point(mName, spv::ExecutionModelGLCompute);
    KeepResourceBindings(compiler);
    RemapConstexprSamplers(compiler, mReflection.LiteralSamplers);

    mSource = compiler.compile();
    assert(!mSource.empty());
}

void Kernel::InitPipelineState() {
    try {
        AddPipelineState(0, {1, 1, 1});
    } catch (std::exception &e) {
        Release();

        throw e;
    }
}

void Kernel::InitArgTable() {
    for (auto &argument : mReflection.Arguments[mName]) {
        mArgTable[argument.Ordinal] = {.Kind = argument.Kind, .Binding = argument.Binding};
    }
}

MTL::Function *Kernel::CreateFunction(const Size &workGroupSize) {
    auto name = NS::String::alloc()->init(mName.c_str(), NS::UTF8StringEncoding);
    auto constantValues = CreateConstantValues(workGroupSize, GetDimension(workGroupSize));
    NS::Error *error = nullptr;

    auto function = Device::GetSingleton()->GetLibraryPool()->
        At(mProgram, ConvertToString(mDefines) + mSource)->newFunction(name, constantValues, &error);
    assert(function);

    name->release();
    constantValues->release();

    if (error) {
        error->release();
    }

    return function;
}

void Kernel::AddPipelineState(uint64_t hash, const Size &workGroupSize) {
    auto function = CreateFunction(workGroupSize);
    NS::Error *error = nullptr;

    mPipelineStates[hash][ConvertToString(mDefines)] = Device::GetSingleton()->GetDevice()->
        newComputePipelineState(function, &error);

    function->release();

    if (error) {
        error->release();

        throw std::exception();
    }
}

} //namespace cml
