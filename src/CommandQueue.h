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

#ifndef CLMTL_COMMAND_QUEUE_H
#define CLMTL_COMMAND_QUEUE_H

#include <array>
#include <vector>
#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

#include "Origin.h"
#include "Size.h"
#include "Object.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cl_command_queue {
    cl_icd_dispatch *Dispatch;
};

#ifdef __cplusplus
} //extern "C"
#endif

namespace cml {

class Context;
class Device;
class Buffer;
class Image;
class Kernel;
class Event;

class CommandQueue : public _cl_command_queue, public Object {
public:
    static CommandQueue *DownCast(cl_command_queue commandQueue);

public:
    CommandQueue(Context *context, Device *device, cl_command_queue_properties properties);
    ~CommandQueue() override;
    void EnqueueReadBuffer(Buffer *srcBuffer, size_t srcOffset, void *dstData, size_t dstSize);
    void EnqueueWriteBuffer(const void *srcData, Buffer *dstBuffer, size_t offset, size_t size);
    void EnqueueCopyBuffer(Buffer *srcBuffer, size_t srcOffset, Buffer *dstBuffer, size_t dstOffset, size_t size);
    void EnqueueFillBuffer(const void *srcData, size_t srcSize, Buffer *dstBuffer, size_t dstOffset, size_t dstSize);
    void EnqueueReadImage(Image *srcImage, const Origin &srcOrigin, const Size &srcRegion, void *dstData,
                          size_t dstRowPitch, size_t dstSlicePitch);
    void EnqueueWriteImage(const void *srcData, size_t srcRowPitch, size_t srcSlicePitch, const Size &srcRegion,
                           Image *dstImage, const Origin &dstOrigin);
    void EnqueueCopyImage(Image *srcImage, const Origin &srcOrigin, const Size &srcRegion, Image *dstImage,
                          const Origin &dstOrigin);
    void EnqueueCopyImageToBuffer(Image *srcImage, const Origin &srcOrigin, const Size &srcRegion, Buffer *dstBuffer,
                                  size_t dstOffset);
    void EnqueueCopyBufferToImage(Buffer *srcBuffer, size_t srcOffset, const Size &srcRegion, Image *dstImage,
                                  const Origin &dstOrigin);
    void EnqueueDispatch(Kernel *kernel, const Size &globalWorkSize);
    void EnqueueDispatch(Kernel *kernel, const Size &globalWorkSize, const Size &localWorkSize);
    void EnqueueSignalEvent(Event *event);
    void EnqueueWaitEvent(Event *event);
    void Flush();
    void WaitIdle();
    Context *GetContext() const;
    Device *GetDevice() const;
    cl_command_queue_properties GetProperties() const;

private:
    Context *mContext;
    Device *mDevice;
    cl_command_queue_properties mProperties;
    MTL::CommandQueue *mCommandQueue;
    MTL::CommandBuffer *mCommandBuffer;
    std::vector<MTL::CommandBuffer *> mCommittedCommandBuffer;

    void InitCommandQueue();
    void InitCommandBuffer();
};

} //namespace cml

#endif //CLMTL_COMMAND_QUEUE_H
