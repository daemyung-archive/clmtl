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

#include "Buffer.h"

#include "Dispatch.h"
#include "Util.h"
#include "Context.h"
#include "Device.h"

_cl_mem::_cl_mem(cl_icd_dispatch *dispatch) :
        Dispatch{dispatch} {
    assert(Dispatch);
}

namespace cml {

MTL::ResourceOptions convertToResourceOptions(cl_mem_flags flags) {
    MTL::ResourceOptions options = 0;

    if (Util::TestAnyFlagSet(flags, CL_MEM_READ_WRITE | CL_MEM_WRITE_ONLY | CL_MEM_READ_ONLY)) {
        options |= MTL::ResourceStorageModePrivate;
    }

    if (Util::TestAnyFlagSet(flags, CL_MEM_ALLOC_HOST_PTR)) {
        options |= MTL::ResourceStorageModeShared;
        options |= MTL::ResourceCPUCacheModeWriteCombined;
    }

    return options;
}

Buffer *Buffer::DownCast(cl_mem buffer) {
    return dynamic_cast<Buffer *>(buffer);
}

Buffer::Buffer(Context *context, cl_mem_flags flags, size_t size) :
        _cl_mem{Dispatch::GetTable()}, mContext{context}, mMemFlags{flags}, mBuffer{nullptr}, mMapCount{0} {
    InitBuffer(size);
}

Buffer::Buffer(Context *context, cl_mem_flags flags, const void *data, size_t size) :
        _cl_mem{Dispatch::GetTable()}, mContext{context}, mMemFlags{flags}, mBuffer{nullptr}, mMapCount{0} {
    InitBuffer(data, size);
}

Buffer::~Buffer() {
    mBuffer->release();
}

void *Buffer::Map() {
    if (Util::TestAnyFlagSet(mMemFlags, CL_MEM_ALLOC_HOST_PTR)) {
        ++mMapCount;
        return mBuffer->contents();
    } else {
        return nullptr;
    }
}

void Buffer::Unmap() {
    if (mMapCount) {
        --mMapCount;
    }
}

Context *Buffer::GetContext() const {
    return mContext;
}

cl_mem_flags Buffer::GetMemFlags() const {
    return mMemFlags;
}

MTL::Buffer *Buffer::GetBuffer() const {
    return mBuffer;
}

size_t Buffer::GetSize() const {
    return mBuffer->length();
}

cl_uint Buffer::GetMapCount() const {
    return mMapCount;
}

void Buffer::InitBuffer(size_t size) {
    auto device = mContext->GetDevice();
    assert(device);

    mBuffer = device->GetDevice()->newBuffer(size, convertToResourceOptions(mMemFlags));
    assert(mBuffer);
}

void Buffer::InitBuffer(const void *data, size_t size) {
    auto device = mContext->GetDevice();
    assert(device);

    mBuffer = device->GetDevice()->newBuffer(data, size, convertToResourceOptions(mMemFlags));
    assert(mBuffer);
}

} //namespace cml
