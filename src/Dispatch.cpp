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

#include <CL/cl_icd.h>

#include "Dispatch.h"

namespace cml {

cl_icd_dispatch *Dispatch::GetTable() {
    static cl_icd_dispatch sDispatchTable{
            .clGetPlatformIDs = &clGetPlatformIDs,
            .clGetPlatformInfo = &clGetPlatformInfo,
            .clGetDeviceIDs = &clGetDeviceIDs,
            .clGetDeviceInfo = &clGetDeviceInfo,
            .clCreateContext = &clCreateContext,
            .clCreateContextFromType = &clCreateContextFromType,
            .clRetainContext = &clRetainContext,
            .clReleaseContext = &clReleaseContext,
            .clGetContextInfo = &clGetContextInfo,
            .clCreateCommandQueue = &clCreateCommandQueue,
            .clRetainCommandQueue = &clRetainCommandQueue,
            .clReleaseCommandQueue = &clReleaseCommandQueue,
            .clGetCommandQueueInfo = &clGetCommandQueueInfo,
            .clSetCommandQueueProperty = &clSetCommandQueueProperty,
            .clCreateBuffer = &clCreateBuffer,
            .clCreateImage2D = &clCreateImage2D,
            .clCreateImage3D = &clCreateImage3D,
            .clRetainMemObject = &clRetainMemObject,
            .clReleaseMemObject = &clReleaseMemObject,
            .clGetSupportedImageFormats = &clGetSupportedImageFormats,
            .clGetMemObjectInfo = &clGetMemObjectInfo,
            .clGetImageInfo = &clGetImageInfo,
            .clCreateSampler = &clCreateSampler,
            .clRetainSampler = &clRetainSampler,
            .clReleaseSampler = &clReleaseSampler,
            .clGetSamplerInfo = &clGetSamplerInfo,
            .clCreateProgramWithSource = &clCreateProgramWithSource,
            .clCreateProgramWithBinary = &clCreateProgramWithBinary,
            .clRetainProgram = &clRetainProgram,
            .clReleaseProgram = &clReleaseProgram,
            .clBuildProgram = &clBuildProgram,
            .clUnloadCompiler = &clUnloadCompiler,
            .clGetProgramInfo = &clGetProgramInfo,
            .clGetProgramBuildInfo = &clGetProgramBuildInfo,
            .clCreateKernel = &clCreateKernel,
            .clCreateKernelsInProgram = &clCreateKernelsInProgram,
            .clRetainKernel = &clRetainKernel,
            .clReleaseKernel = &clReleaseKernel,
            .clSetKernelArg = &clSetKernelArg,
            .clGetKernelInfo = &clGetKernelInfo,
            .clGetKernelWorkGroupInfo = &clGetKernelWorkGroupInfo,
            .clWaitForEvents = &clWaitForEvents,
            .clGetEventInfo = &clGetEventInfo,
            .clRetainEvent = &clRetainEvent,
            .clReleaseEvent = &clReleaseEvent,
            .clGetEventProfilingInfo = &clGetEventProfilingInfo,
            .clFlush = &clFlush,
            .clFinish = &clFinish,
            .clEnqueueReadBuffer = &clEnqueueReadBuffer,
            .clEnqueueWriteBuffer = &clEnqueueWriteBuffer,
            .clEnqueueCopyBuffer = &clEnqueueCopyBuffer,
            .clEnqueueReadImage = &clEnqueueReadImage,
            .clEnqueueWriteImage = &clEnqueueWriteImage,
            .clEnqueueCopyImage = &clEnqueueCopyImage,
            .clEnqueueCopyImageToBuffer = &clEnqueueCopyImageToBuffer,
            .clEnqueueCopyBufferToImage = &clEnqueueCopyBufferToImage,
            .clEnqueueMapBuffer = &clEnqueueMapBuffer,
            .clEnqueueMapImage = &clEnqueueMapImage,
            .clEnqueueUnmapMemObject = &clEnqueueUnmapMemObject,
            .clEnqueueNDRangeKernel = &clEnqueueNDRangeKernel,
            .clEnqueueTask = &clEnqueueTask,
            .clEnqueueNativeKernel = &clEnqueueNativeKernel,
            .clEnqueueMarker = &clEnqueueMarker,
            .clEnqueueWaitForEvents = &clEnqueueWaitForEvents,
            .clEnqueueBarrier = &clEnqueueBarrier,
            .clGetExtensionFunctionAddress = &clGetExtensionFunctionAddress,
            .clCreateFromGLBuffer = &clCreateFromGLBuffer,
            .clCreateFromGLTexture2D = &clCreateFromGLTexture2D,
            .clCreateFromGLTexture3D = &clCreateFromGLTexture3D,
            .clCreateFromGLRenderbuffer = &clCreateFromGLRenderbuffer,
            .clGetGLObjectInfo = &clGetGLObjectInfo,
            .clGetGLTextureInfo = &clGetGLTextureInfo,
            .clEnqueueAcquireGLObjects = &clEnqueueAcquireGLObjects,
            .clEnqueueReleaseGLObjects = &clEnqueueReleaseGLObjects,
            .clGetGLContextInfoKHR = &clGetGLContextInfoKHR
    };

    return &sDispatchTable;
}

void *Dispatch::GetExtensionSymbol(const std::string &symbolName) {
    if ("clIcdGetPlatformIDsKHR" == symbolName) {
        return reinterpret_cast<void *>(&clIcdGetPlatformIDsKHR);
    }

    return nullptr;
}

} //namespace cml