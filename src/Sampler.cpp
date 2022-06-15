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

#include "Sampler.h"

#include "Dispatch.h"
#include "Device.h"

namespace cml {

Sampler *Sampler::DownCast(cl_sampler sampler) {
    return (Sampler *) sampler;
}

Sampler::Sampler(Context *context, cl_bool normalizedCoords, cl_addressing_mode addressingMode,
                 cl_filter_mode filterMode) :
        _cl_sampler{Dispatch::GetTable()}, Object{}, mContext{context}, mNormalizedCoords{normalizedCoords},
        mAddressingMode{addressingMode}, mFilterMode{filterMode}, mSamplerState{nullptr} {
    InitSamplerState();
}

Sampler::~Sampler() {
    mSamplerState->release();
}

Context *Sampler::GetContext() const {
    return mContext;
}

cl_bool Sampler::GetNormalizedCoords() const {
    return mNormalizedCoords;
}

cl_addressing_mode Sampler::GetAddressingMode() const {
    return mAddressingMode;
}

cl_filter_mode Sampler::GetFilterMode() const {
    return mFilterMode;
}

MTL::SamplerState *Sampler::GetSamplerState() const {
    return mSamplerState;
}

void Sampler::InitSamplerState() {
    auto descriptor = MTL::SamplerDescriptor::alloc()->init();
    assert(descriptor);

    descriptor->setNormalizedCoordinates(mNormalizedCoords);

    switch (mAddressingMode) {
        case CL_ADDRESS_NONE:
            descriptor->setRAddressMode(MTL::SamplerAddressModeClampToZero);
            break;
        case CL_ADDRESS_CLAMP_TO_EDGE:
            descriptor->setRAddressMode(MTL::SamplerAddressModeClampToEdge);
            break;
        case CL_ADDRESS_CLAMP:
            descriptor->setRAddressMode(MTL::SamplerAddressModeClampToZero);
            break;
        case CL_ADDRESS_REPEAT:
            descriptor->setRAddressMode(MTL::SamplerAddressModeRepeat);
            break;
    }

    switch (mFilterMode) {
        case CL_FILTER_NEAREST:
            descriptor->setMinFilter(MTL::SamplerMinMagFilterNearest);
            descriptor->setMagFilter(MTL::SamplerMinMagFilterNearest);
            break;
        case CL_FILTER_LINEAR:
            descriptor->setMinFilter(MTL::SamplerMinMagFilterLinear);
            descriptor->setMagFilter(MTL::SamplerMinMagFilterLinear);
            break;
    }

    mSamplerState = Device::GetSingleton()->GetDevice()->newSamplerState(descriptor);
    assert(mSamplerState);

    descriptor->release();
}

} //namespace cml
