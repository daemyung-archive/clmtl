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

namespace cml {

static MTL::ResourceOptions convertToResourceOptions(cl_mem_flags flags) {
    MTL::ResourceOptions options = MTL::ResourceHazardTrackingModeTracked;

    if (Util::TestAnyFlagSet(flags, CL_MEM_HOST_NO_ACCESS)) {
        options |= MTL::ResourceStorageModePrivate;
    } else {
        options |= MTL::ResourceStorageModeShared;
    }

    if (Util::TestAnyFlagSet(flags, CL_MEM_HOST_WRITE_ONLY)) {
        options |= MTL::ResourceCPUCacheModeWriteCombined;
    }

    return options;
}

Buffer *Buffer::DownCast(cl_mem buffer) {
    return (Buffer *) buffer;
}

Buffer::Buffer(Context *context, cl_mem_flags flags, size_t size)
    : Memory{context, flags}, mParent{nullptr}, mHeap{nullptr}, mBuffer{nullptr} {
    InitHeap(size);
    InitBuffer(size, 0);
}

Buffer::Buffer(Context *context, cl_mem_flags flags, const void *data, size_t size)
    : Memory{context, flags}, mParent{nullptr}, mHeap{nullptr}, mBuffer{nullptr} {
    InitHeap(size);
    InitBuffer(size, 0);
    InitData(data, size);
}

Buffer::Buffer(Buffer *parent, cl_mem_flags flags, const cl_buffer_region *region)
    : Memory{parent->GetContext(), flags}, mParent{parent}, mHeap{nullptr}, mBuffer{nullptr}{
    InitHeap();
    InitBuffer(region->size, region->origin);
}

Buffer::~Buffer() {
    mBuffer->release();
    mHeap->release();
}

void *Buffer::Map() {
    if (Util::TestAnyFlagSet(mFlags, CL_MEM_HOST_NO_ACCESS)) {
        return nullptr;
    } else {
        ++mMapCount;
        return mBuffer->contents();
    }
}

void Buffer::Unmap() {
    if (mMapCount) {
        --mMapCount;
    }
}

Buffer *Buffer::GetParent() const {
    return mParent;
}

MTL::Heap *Buffer::GetHeap() const {
    return mHeap;
}

MTL::Buffer *Buffer::GetBuffer() const {
    return mBuffer;
}

size_t Buffer::GetSize() const {
    return mBuffer->length();
}

void Buffer::InitHeap(size_t size) {
    auto descriptor = MTL::HeapDescriptor::alloc()->init();
    assert(descriptor);

    descriptor->setSize(size);
    descriptor->setResourceOptions(convertToResourceOptions(mFlags));
    descriptor->setType(MTL::HeapTypePlacement);

    auto device = mContext->GetDevice();
    assert(device);

    mHeap = device->GetDevice()->newHeap(descriptor);
    assert(mHeap);

    descriptor->release();
}

void Buffer::InitHeap() {
    mHeap = mParent->GetHeap();
    mHeap->retain();
}

void Buffer::InitBuffer(size_t size, size_t offset) {
    mBuffer = mHeap->newBuffer(size, mHeap->resourceOptions(), offset);
    assert(mBuffer);
}

void Buffer::InitData(const void *data, size_t size) {
    memcpy(mBuffer->contents(), data, size);
}

} //namespace cml
