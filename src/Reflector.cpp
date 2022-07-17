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

#include <spirv-tools/libspirv.hpp>
#include <spirv_cross/spirv.hpp>

namespace cml {

enum class ExtInst : uint32_t {
    Kernel = 1,
    ArgumentInfo = 2,
    ArgumentStorageBuffer = 3,
    ArgumentUniform = 4,
    ArgumentPodStorageBuffer = 5,
    ArgumentPodUniform = 6,
    ArgumentPodPushConstant = 7,
    ArgumentSampledImage = 8,
    ArgumentStorageImage = 9,
    ArgumentSampler = 10,
    ArgumentWorkgroup = 11,
    SpecConstantWorkgroupSize = 12,
    SpecConstantGlobalOffset = 13,
    SpecConstantWorkDim = 14,
    PushConstantGlobalOffset = 15,
    PushConstantEnqueuedLocalSize = 16,
    PushConstantGlobalSize = 17,
    PushConstantRegionOffset = 18,
    PushConstantNumWorkgroups = 19,
    PushConstantRegionGroupOffset = 20,
    ConstantDataStorageBuffer = 21,
    ConstantDataUniform = 22,
    LiteralSampler = 23,
    PropertyRequiredWorkgroupSize = 24,
    SpecConstantSubgroupMaxSize = 25,
    Max = 0x7fffffffu
};

clspv::ArgKind ConvertToArgKind(uint32_t instruction) {
    switch (static_cast<ExtInst>(instruction)) {
        case ExtInst::ArgumentStorageBuffer:
        case ExtInst::ConstantDataStorageBuffer:
            return clspv::ArgKind::Buffer;
        case ExtInst::ArgumentUniform:
        case ExtInst::ConstantDataUniform:
            return clspv::ArgKind::BufferUBO;
        case ExtInst::ArgumentPodStorageBuffer:
            return clspv::ArgKind::Pod;
        case ExtInst::ArgumentPodUniform:
            return clspv::ArgKind::PodUBO;
        case ExtInst::ArgumentPodPushConstant:
            return clspv::ArgKind::PodPushConstant;
        case ExtInst::ArgumentSampledImage:
            return clspv::ArgKind::SampledImage;
        case ExtInst::ArgumentStorageImage:
            return clspv::ArgKind::StorageImage;
        case ExtInst::ArgumentSampler:
            return clspv::ArgKind::Sampler;
        case ExtInst::ArgumentWorkgroup:
            return clspv::ArgKind::Local;
        default:
            throw std::exception();
    }
}

clspv::PushConstant ConvertToPushConstant(uint32_t instruction) {
    switch (static_cast<ExtInst>(instruction)) {
        case ExtInst::PushConstantGlobalOffset:
            return clspv::PushConstant::GlobalOffset;
        case ExtInst::PushConstantEnqueuedLocalSize:
            return clspv::PushConstant::EnqueuedLocalSize;
        case ExtInst::PushConstantGlobalSize:
            return clspv::PushConstant::GlobalSize;
        case ExtInst::PushConstantRegionOffset:
            return clspv::PushConstant::RegionOffset;
        case ExtInst::PushConstantNumWorkgroups:
            return clspv::PushConstant::NumWorkgroups;
        case ExtInst::PushConstantRegionGroupOffset:
            return clspv::PushConstant::RegionGroupOffset;
        default:
            throw std::exception();
    }
}

clspv::SamplerNormalizedCoords ConvertToSamplerNormalizedCoords(uint32_t mask) {
    return static_cast<clspv::SamplerNormalizedCoords>(mask & clspv::kSamplerNormalizedCoordsMask);
}

clspv::SamplerAddressingMode ConvertToSamplerAddressingMode(uint32_t mask) {
    return static_cast<clspv::SamplerAddressingMode>(mask & clspv::kSamplerAddressMask);
}

clspv::SamplerFilterMode ConvertToSamplerFilteringMode(uint32_t mask) {
    return static_cast<clspv::SamplerFilterMode>(mask & clspv::kSamplerFilterMask);
}

bool IsNonSemanticClspvReflection(const spv_parsed_instruction_t *inst) {
    return inst->ext_inst_type == SPV_EXT_INST_TYPE_NONSEMANTIC_CLSPVREFLECTION;
}

class Parser {
public:
    explicit Parser(spv_target_env env)
        : mContext{env}, mIntId{0}, mConstants{}, mStrings{}, mReflection{} {
    }

    Reflection Parse(const std::vector<uint32_t> &binary) {
        if (spvBinaryParse(mContext.CContext(), this, binary.data(), binary.size(), nullptr, Parse,
                           nullptr) == SPV_SUCCESS) {
            return mReflection;
        } else {
            throw std::exception();
        }
    }

private:
    static spv_result_t Parse(void *userData, const spv_parsed_instruction_t *instruction) {
        return static_cast<Parser *>(userData)->Parse(instruction);
    }

private:
    spvtools::Context mContext;
    uint32_t mIntId;
    std::unordered_map<uint32_t, std::string> mStrings;
    std::unordered_map<uint32_t, uint32_t> mConstants;
    Reflection mReflection;

    void ParseTypeInt(const spv_parsed_instruction_t *inst) {
        if (inst->words[inst->operands[1].offset] == 32 &&
            inst->words[inst->operands[2].offset] == 0) {
            mIntId = inst->result_id;
        }
    }

    void ParseConstant(const spv_parsed_instruction_t *inst) {
        if (inst->words[inst->operands[0].offset] == mIntId) {
            mConstants[inst->result_id] = inst->words[inst->operands[2].offset];
        }
    }

    void ParseString(const spv_parsed_instruction_t *inst) {
        mStrings[inst->result_id] = reinterpret_cast<const char *>(inst->words + inst->operands[1].offset);
    }

    void ParseKernel(const spv_parsed_instruction_t *inst) {
        mStrings[inst->result_id] = mStrings[inst->words[inst->operands[5].offset]];
    }

    void ParseArgumentInfo(const spv_parsed_instruction_t *inst) {
        mStrings[inst->result_id] = mStrings[inst->words[inst->operands[4].offset]];
    }

    void ParseArgumentKindKernelOrdinalBinding(const spv_parsed_instruction_t *inst) {
        Argument binding{
            .Kernel = mStrings[inst->words[inst->operands[4].offset]],
            .Ordinal = mConstants[inst->words[inst->operands[5].offset]],
            .Kind = ConvertToArgKind(inst->words[inst->operands[3].offset]),
            .Binding = mConstants[inst->words[inst->operands[7].offset]],
        };

        mReflection.Arguments[binding.Kernel].push_back(binding);
    }

    void ParseArgumentKindKernelOrdinalBindingOffsetSize(const spv_parsed_instruction_t *inst) {
        Argument binding{
            .Kernel = mStrings[inst->words[inst->operands[4].offset]],
            .Ordinal = mConstants[inst->words[inst->operands[5].offset]],
            .Kind = ConvertToArgKind(inst->words[inst->operands[3].offset]),
            .Binding = mConstants[inst->words[inst->operands[7].offset]],
            .Size = mConstants[inst->words[inst->operands[9].offset]],
            .Offset = mConstants[inst->words[inst->operands[8].offset]],
        };

        mReflection.Arguments[binding.Kernel].push_back(binding);
    }

    void ParseArgumentKindKernelOrdinalOffsetSize(const spv_parsed_instruction_t *inst) {
        Argument binding{
            .Kernel = mStrings[inst->words[inst->operands[4].offset]],
            .Ordinal = mConstants[inst->words[inst->operands[5].offset]],
            .Kind = ConvertToArgKind(inst->words[inst->operands[3].offset]),
            .Size = mConstants[inst->words[inst->operands[6].offset]],
            .Offset = mConstants[inst->words[inst->operands[7].offset]],
        };

        mReflection.Arguments[binding.Kernel].push_back(binding);
    }

    void ParseArgumentKindKernelOrdinalSizeSpec(const spv_parsed_instruction_t *inst) {
        Argument binding{
            .Kernel = mStrings[inst->words[inst->operands[4].offset]],
            .Ordinal = mConstants[inst->words[inst->operands[5].offset]],
            .Kind = ConvertToArgKind(inst->words[inst->operands[3].offset]),
            .Size = mConstants[inst->words[inst->operands[7].offset]],
            .Spec = mConstants[inst->words[inst->operands[6].offset]]
        };

        mReflection.Arguments[binding.Kernel].push_back(binding);
    }

    void ParseLiteralDescSetBindingMask(const spv_parsed_instruction_t *inst) {
        LiteralSampler literalSampler {
            .DescSet = mConstants[inst->words[inst->operands[4].offset]],
            .Binding = mConstants[inst->words[inst->operands[5].offset]],
            .NormalizedCoords = ConvertToSamplerNormalizedCoords(mConstants[inst->words[inst->operands[6].offset]]),
            .AddressingMode = ConvertToSamplerAddressingMode(mConstants[inst->words[inst->operands[6].offset]]),
            .FilterMode = ConvertToSamplerFilteringMode(mConstants[inst->words[inst->operands[6].offset]])
        };

        mReflection.LiteralSamplers.push_back(literalSampler);
    }

    void ParseConstantDataDescSetBindingDataKind(const spv_parsed_instruction_t *inst) {
        ConstantData constantData {
            .Kind = ConvertToArgKind(inst->words[inst->operands[3].offset]),
            .DescSet = mConstants[inst->words[inst->operands[4].offset]],
            .Binding = mConstants[inst->words[inst->operands[5].offset]],
            .Data = mStrings[inst->words[inst->operands[6].offset]]
        };

        mReflection.ConstantData.push_back(constantData);
    }

    void ParseSpecConstantWorkgroupSize(const spv_parsed_instruction_t *inst) {
        Size workgroupSize {
            .w = mConstants[inst->words[inst->operands[4].offset]],
            .h = mConstants[inst->words[inst->operands[5].offset]],
            .d = mConstants[inst->words[inst->operands[6].offset]]
        };

        mReflection.WorkgroupSize = workgroupSize;
    }

    void ParseSpecConstantGlobalOffset(const spv_parsed_instruction_t *inst) {
        Origin globalOffset {
            .x = mConstants[inst->words[inst->operands[4].offset]],
            .y = mConstants[inst->words[inst->operands[5].offset]],
            .z = mConstants[inst->words[inst->operands[5].offset]]
        };

        mReflection.GlobalOffset = globalOffset;
    }

    void ParseSpecConstantWorkDim(const spv_parsed_instruction_t *inst) {
        mReflection.WorkDim = mConstants[inst->words[inst->operands[4].offset]];
    }

    void ParsePushConstantOffsetSizeKind(const spv_parsed_instruction_t * inst) {
        PushConstant pushConstant {
            .Kind = ConvertToPushConstant(inst->operands[3].offset),
            .Offset = mConstants[inst->words[inst->operands[4].offset]],
            .Size = mConstants[inst->words[inst->operands[5].offset]]
        };

        mReflection.PushConstants.push_back(pushConstant);
    }

    void ParsePropertyRequiredWorkgroupSize(const spv_parsed_instruction_t * inst) {
        RequiredWorkgroupSize requiredWorkGroupSize {
            .Kernel = mStrings[inst->words[inst->operands[4].offset]],
            .WorkgroupSize {
                .w = mConstants[inst->words[inst->operands[5].offset]],
                .h = mConstants[inst->words[inst->operands[6].offset]],
                .d = mConstants[inst->words[inst->operands[7].offset]]
            }
        };

        mReflection.RequiredWorkgroupSizes[requiredWorkGroupSize.Kernel] = requiredWorkGroupSize;
    }

    void ParseExtInst(const spv_parsed_instruction_t *inst) {
        if (IsNonSemanticClspvReflection(inst)) {
            switch (static_cast<ExtInst>(inst->words[inst->operands[3].offset])) {
                case ExtInst::Kernel:
                    ParseKernel(inst);
                    break;
                case ExtInst::ArgumentInfo:
                    ParseArgumentInfo(inst);
                    break;
                case ExtInst::ArgumentStorageBuffer:
                case ExtInst::ArgumentUniform:
                case ExtInst::ArgumentSampledImage:
                case ExtInst::ArgumentStorageImage:
                case ExtInst::ArgumentSampler:
                    ParseArgumentKindKernelOrdinalBinding(inst);
                    break;
                case ExtInst::ArgumentPodStorageBuffer:
                case ExtInst::ArgumentPodUniform:
                    ParseArgumentKindKernelOrdinalBindingOffsetSize(inst);
                    break;
                case ExtInst::ArgumentPodPushConstant:
                    ParseArgumentKindKernelOrdinalOffsetSize(inst);
                    break;
                case ExtInst::ArgumentWorkgroup:
                    ParseArgumentKindKernelOrdinalSizeSpec(inst);
                    break;
                case ExtInst::ConstantDataStorageBuffer:
                case ExtInst::ConstantDataUniform:
                    ParseConstantDataDescSetBindingDataKind(inst);
                    break;
                case ExtInst::SpecConstantWorkgroupSize:
                    ParseSpecConstantWorkgroupSize(inst);
                    break;
                case ExtInst::SpecConstantGlobalOffset:
                    ParseSpecConstantGlobalOffset(inst);
                    break;
                case ExtInst::SpecConstantWorkDim:
                    ParseSpecConstantWorkDim(inst);
                    break;
                case ExtInst::PushConstantGlobalOffset:
                case ExtInst::PushConstantEnqueuedLocalSize:
                case ExtInst::PushConstantGlobalSize:
                case ExtInst::PushConstantRegionOffset:
                case ExtInst::PushConstantNumWorkgroups:
                case ExtInst::PushConstantRegionGroupOffset:
                    ParsePushConstantOffsetSizeKind(inst);
                    break;
                case ExtInst::LiteralSampler:
                    ParseLiteralDescSetBindingMask(inst);
                    break;
                case ExtInst::PropertyRequiredWorkgroupSize:
                    ParsePropertyRequiredWorkgroupSize(inst);
                    break;
                default:
                    break;
            }
        }
    }

    spv_result_t Parse(const spv_parsed_instruction_t *inst) {
        switch (inst->opcode) {
            case spv::OpTypeInt:
                ParseTypeInt(inst);
                break;
            case spv::OpConstant:
                ParseConstant(inst);
                break;
            case spv::OpString:
                ParseString(inst);
                break;
            case spv::OpExtInst:
                ParseExtInst(inst);
                break;
            default:
                break;
        }

        return SPV_SUCCESS;
    }
};

Reflection Reflector::Reflect(const std::vector<uint32_t> &binary) {
    return Parser(SPV_ENV_OPENCL_1_2).Parse(binary);
}

} //namespace cml
