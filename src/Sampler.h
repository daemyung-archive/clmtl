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

#ifndef CLMTL_SAMPLER_H
#define CLMTL_SAMPLER_H

#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

#include "Object.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cl_sampler {
    cl_icd_dispatch *Dispatch;
};

#ifdef __cplusplus
} //extern "C"
#endif

namespace cml {

class Context;

class Sampler : public _cl_sampler, public Object {
public:
    static Sampler *DownCast(cl_sampler sampler);

public:
    Sampler(Context *context, cl_bool normalizedCoords, cl_addressing_mode addressingMode, cl_filter_mode filterMode);
    ~Sampler() override;
    Context *GetContext() const;
    cl_bool GetNormalizedCoords() const;
    cl_addressing_mode GetAddressingMode() const;
    cl_filter_mode GetFilterMode() const;

private:
    Context *mContext;
    cl_bool mNormalizedCoords;
    cl_addressing_mode mAddressingMode;
    cl_filter_mode mFilterMode;
    MTL::SamplerState *mSamplerState;

    void InitSamplerState();
};

} //namespace cml

#endif //CLMTL_SAMPLER_H
