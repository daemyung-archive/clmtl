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
#include "Util.h"
#include "Context.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"
#include "Kernel.h"
#include "Event.h"
#include "Sampler.h"

namespace cml {

MTL::Origin ConvertToOrigin(const Origin &origin) {
    return MTL::Origin::Make(origin.x, origin.y, origin.z);
}

MTL::Size ConvertToSize(const Size &size) {
    return MTL::Size::Make(size.w, std::max(size.h, 1lu), std::max(size.d, 1lu));
}

void BindResources(MTL::ComputeCommandEncoder *commandEncoder, Kernel *kernel) {
    for (auto &[index, arg]: kernel->GetArgTable()) {
        switch (arg.Kind) {
            case clspv::ArgKind::Buffer:
                commandEncoder->setBuffer(Buffer::DownCast(arg.Buffer)->GetBuffer(), 0, arg.Binding);
                break;
            case clspv::ArgKind::PodUBO:
                commandEncoder->setBytes(arg.Data, arg.Size, arg.Binding);
                break;
            case clspv::ArgKind::SampledImage:
            case clspv::ArgKind::StorageImage:
                commandEncoder->setTexture(Image::DownCast(arg.Image)->GetTexture(), arg.Binding);
                break;
            case clspv::ArgKind::Sampler:
                commandEncoder->setSamplerState(Sampler::DownCast(arg.Sampler)->GetSamplerState(), arg.Binding);
                break;
            default:
                break;
        }
    }
}

CommandQueue *CommandQueue::DownCast(cl_command_queue commandQueue) {
    return (CommandQueue *) commandQueue;
}

CommandQueue::CommandQueue(Context *context, Device *device)
    : _cl_command_queue{Dispatch::GetTable()}, Object{}, mContext{context}, mDevice{device}, mCommandQueue{}
    , mCommandBuffer{}, mCommittedCommandBuffer{} {
    InitCommandQueue();
    InitCommandBuffer();
}

CommandQueue::~CommandQueue() {
    WaitIdle();
    mCommandBuffer->release();
    mCommandQueue->release();
}

void CommandQueue::EnqueueReadBuffer(Buffer *srcBuffer, size_t srcOffset, void *dstData, size_t dstSize) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto dstBuffer = new Buffer(mContext, CL_MEM_ALLOC_HOST_PTR, dstSize);
    assert(dstBuffer);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), srcOffset, dstBuffer->GetBuffer(), 0, dstSize);
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

    auto srcBuffer = new Buffer(mContext, CL_MEM_ALLOC_HOST_PTR | CL_MEM_COPY_HOST_PTR, srcData, size);
    assert(srcBuffer);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), 0, dstBuffer->GetBuffer(), offset, size);
    commandEncoder->endEncoding();
    commandEncoder->release();
    mCommandBuffer->addCompletedHandler([srcBuffer](MTL::CommandBuffer *commandBuffer) {
        srcBuffer->Release();
        delete srcBuffer;
    });
}

void CommandQueue::EnqueueCopyBuffer(Buffer *srcBuffer, size_t srcOffset, Buffer *dstBuffer, size_t dstOffset,
                                     size_t size) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), srcOffset, dstBuffer->GetBuffer(), dstOffset, size);
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::EnqueueFillBuffer(const void *srcData, size_t srcSize, Buffer *dstBuffer, size_t dstOffset,
                                     size_t dstSize) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto srcBuffer = new Buffer(mContext, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, srcData, srcSize);
    assert(srcBuffer);

    for (size_t i = 0; i < dstSize; i += srcSize) {
        commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), 0, dstBuffer->GetBuffer(), dstOffset + i, srcSize);
    }

    commandEncoder->endEncoding();
    commandEncoder->release();
    mCommandBuffer->addCompletedHandler([srcBuffer](MTL::CommandBuffer *commandBuffer) {
        srcBuffer->Release();
        delete srcBuffer;
    });
}

void CommandQueue::EnqueueReadImage(Image *srcImage, const Origin &srcOrigin, const Size &srcRegion, void *dstData,
                                    size_t dstRowPitch, size_t dstSlicePitch) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto dstBuffer = new Buffer(mContext, CL_MEM_ALLOC_HOST_PTR, dstSlicePitch * srcRegion.d);
    assert(dstBuffer);

    commandEncoder->copyFromTexture(srcImage->GetTexture(), 0, 0, ConvertToOrigin(srcOrigin), ConvertToSize(srcRegion),
                                    dstBuffer->GetBuffer(), 0, dstRowPitch, dstSlicePitch);
    commandEncoder->endEncoding();
    commandEncoder->release();
    mCommandBuffer->addCompletedHandler([dstData, dstBuffer](MTL::CommandBuffer *commandBuffer) {
        memcpy(dstData, dstBuffer->Map(), dstBuffer->GetSize());
        dstBuffer->Unmap();
        dstBuffer->Release();
        delete dstBuffer;
    });
}

void CommandQueue::EnqueueWriteImage(const void *srcData, size_t srcRowPitch, size_t srcSlicePitch,
                                     const Size &srcRegion, Image *dstImage, const Origin &dstOrigin) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto srcBuffer = new Buffer(mContext, CL_MEM_ALLOC_HOST_PTR | CL_MEM_COPY_HOST_PTR, srcData,
                                srcSlicePitch * srcRegion.d);
    assert(srcBuffer);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), 0, srcRowPitch, srcSlicePitch, ConvertToSize(srcRegion),
                                   dstImage->GetTexture(), 0, 0, ConvertToOrigin(dstOrigin));
    commandEncoder->endEncoding();
    commandEncoder->release();
    mCommandBuffer->addCompletedHandler([srcBuffer](MTL::CommandBuffer *commandBuffer) {
        srcBuffer->Release();
        delete srcBuffer;
    });
}

void CommandQueue::EnqueueCopyImage(Image *srcImage, const Origin &srcOrigin, const Size &srcRegion, Image *dstImage,
                                    const Origin &dstOrigin) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    commandEncoder->copyFromTexture(srcImage->GetTexture(), 0, 0, ConvertToOrigin(srcOrigin), ConvertToSize(srcRegion),
                                    dstImage->GetTexture(), 0, 0, ConvertToOrigin(dstOrigin));
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::EnqueueCopyImageToBuffer(Image *srcImage, const Origin &srcOrigin, const Size &srcRegion,
                                            Buffer *dstBuffer, size_t dstOffset) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto dstRowPitch = srcRegion.w * cml::Util::GetFormatSize(srcImage->GetFormat());
    assert(!(dstRowPitch % 32) || !(dstRowPitch % 767));

    auto dstSlicePitch = dstRowPitch * srcRegion.h;
    assert(dstSlicePitch);

    commandEncoder->copyFromTexture(srcImage->GetTexture(), 0, 0, ConvertToOrigin(srcOrigin), ConvertToSize(srcRegion),
                                    dstBuffer->GetBuffer(), dstOffset, dstRowPitch, dstSlicePitch);
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::EnqueueCopyBufferToImage(Buffer *srcBuffer, size_t srcOffset, const Size &srcRegion, Image *dstImage,
                                            const Origin &dstOrigin) {
    auto commandEncoder = mCommandBuffer->blitCommandEncoder();
    assert(commandEncoder);

    auto srcRowPitch = srcRegion.w * cml::Util::GetFormatSize(dstImage->GetFormat());
    assert(!(srcRowPitch % 32) || !(srcRowPitch % 767));

    auto srcSlicePitch = srcRowPitch * srcRegion.h;
    assert(srcSlicePitch);

    commandEncoder->copyFromBuffer(srcBuffer->GetBuffer(), srcOffset, srcRowPitch, srcSlicePitch,
                                   ConvertToSize(srcRegion), dstImage->GetTexture(), 0, 0, ConvertToOrigin(dstOrigin));
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::EnqueueDispatch(Kernel *kernel, const Size &globalWorkSize) {
    auto commandEncoder = mCommandBuffer->computeCommandEncoder();
    assert(commandEncoder);

    Size workGroupSize{kernel->GetWorkItemExecutionWidth(),
                       kernel->GetWorkGroupSize() / kernel->GetWorkItemExecutionWidth(), 1};
    assert(workGroupSize.w && workGroupSize.h && workGroupSize.d);

    BindResources(commandEncoder, kernel);
    commandEncoder->setComputePipelineState(kernel->GetPipelineState(workGroupSize));
    commandEncoder->dispatchThreads(ConvertToSize(globalWorkSize), ConvertToSize(workGroupSize));
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::EnqueueDispatch(Kernel *kernel, const Size &globalWorkSize, const Size &localWorkSize) {
    auto commandEncoder = mCommandBuffer->computeCommandEncoder();
    assert(commandEncoder);

    BindResources(commandEncoder, kernel);
    commandEncoder->setComputePipelineState(kernel->GetPipelineState(localWorkSize));
    commandEncoder->dispatchThreads(ConvertToSize(globalWorkSize), ConvertToSize(localWorkSize));
    commandEncoder->endEncoding();
    commandEncoder->release();
}

void CommandQueue::EnqueueSignalEvent(Event *event) {
    mCommandBuffer->encodeSignalEvent(event->GetEvent(), 1);
    mCommandBuffer->addScheduledHandler([event](MTL::CommandBuffer *commandBuffer) {
        event->SetStatus(CL_QUEUED);
        event->SetStatus(CL_RUNNING);
    });
    mCommandBuffer->addCompletedHandler([event](MTL::CommandBuffer *commandBuffer) {
        event->SetStatus(CL_COMPLETE);
    });
}

void CommandQueue::EnqueueWaitEvent(Event *event) {
    mCommandBuffer->encodeWait(event->GetEvent(), 1);
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
