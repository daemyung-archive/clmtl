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

#include "CommandQueue.h"

#include "Dispatch.h"
#include "Context.h"
#include "Device.h"
#include "Buffer.h"
#include "Kernel.h"

_cl_command_queue::_cl_command_queue(cl_icd_dispatch *dispatch) :
        Dispatch{dispatch} {
    assert(Dispatch);
}

namespace cml {

MTL::Size ConvertToSize(const std::array<size_t, 3> &size) {
    return {size[0], size[1], size[2]};
}

void BindResources(MTL::ComputeCommandEncoder *commandEncoder, Kernel *kernel) {
    for (auto &[index, arg]: kernel->GetArgTable()) {
        switch (arg.Kind) {
            case clspv::ArgKind::Buffer:
                commandEncoder->setBuffer(arg.Buffer->GetBuffer(), 0, index);
                break;
            case clspv::ArgKind::Pod:
                commandEncoder->setBytes(arg.Data, arg.Size, index);
                break;
            default:
                break;
        }
    }
}

CommandQueue *CommandQueue::DownCast(cl_command_queue commandQueue) {
    return dynamic_cast<CommandQueue *>(commandQueue);
}

CommandQueue::CommandQueue(Context *context, Device *device) :
        _cl_command_queue{Dispatch::GetTable()}, Object{}, mContext{context}, mDevice{device},
        mCommandQueue{}, mCommandBuffer{}, mCommittedCommandBuffer{} {
    InitCommandQueue();
    InitCommandBuffer();
}

CommandQueue::~CommandQueue() {
    WaitIdle();
    mCommandBuffer->release();
    mCommandQueue->release();
}

void CommandQueue::EnqueueReadBuffer(Buffer *srcBuffer, void *dstData, size_t offset, size_t size) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto dstBuffer = new Buffer(mContext, CL_MEM_ALLOC_HOST_PTR, size);
    assert(dstBuffer);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), 0, dstBuffer->GetBuffer(), offset, size);
    commandEncoder->endEncoding();
    commandEncoder->release();
    mCommandBuffer->addCompletedHandler([dstData, dstBuffer](MTL::CommandBuffer *commandBuffer) {
        memcpy(dstData, dstBuffer->Map(), dstBuffer->GetSize());
        dstBuffer->Unmap();
        dstBuffer->Release();
        delete dstBuffer;
    });
}

void CommandQueue::EnqueueWriteBuffer(const void *srcData, Buffer *dstBuffer, size_t offset, size_t size) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto srcBuffer = new Buffer(mContext, CL_MEM_ALLOC_HOST_PTR, srcData, size);
    assert(srcBuffer);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), 0, dstBuffer->GetBuffer(), offset, size);
    commandEncoder->endEncoding();
    commandEncoder->release();
    mCommandBuffer->addCompletedHandler([srcBuffer](MTL::CommandBuffer *commandBuffer) {
        srcBuffer->Release();
        delete srcBuffer;
    });
}

void CommandQueue::EnqueueDispatch(Kernel *kernel, const std::array<size_t, 3> &globalWorkSize,
                                   const std::array<size_t, 3> &localWorkSize) {
    auto commandEncoder = mCommandBuffer->computeCommandEncoder();
    assert(commandEncoder);

    BindResources(commandEncoder, kernel);
    commandEncoder->setComputePipelineState(kernel->GetPipeline());
    commandEncoder->dispatchThreads(ConvertToSize(globalWorkSize), ConvertToSize(localWorkSize));
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::Flush() {
    mCommandBuffer->commit();
    mCommittedCommandBuffer.push_back(mCommandBuffer);

    InitCommandBuffer();
}

void CommandQueue::WaitIdle() {
    for (auto commandBuffer: mCommittedCommandBuffer) {
        commandBuffer->waitUntilCompleted();
        commandBuffer->release();
    }

    mCommittedCommandBuffer.clear();
}

Context *CommandQueue::GetContext() const {
    return mContext;
}

Device *CommandQueue::GetDevice() const {
    return mDevice;
}

void CommandQueue::InitCommandQueue() {
    mCommandQueue = mDevice->GetDevice()->newCommandQueue();
    assert(mCommandQueue);
}

void CommandQueue::InitCommandBuffer() {
    mCommandBuffer = mCommandQueue->commandBuffer();
    assert(mCommandBuffer);
}

} //namespace cml
