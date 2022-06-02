########################################################################################################################
# Copyright (c) 2022-2022 Daemyung Jang.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
########################################################################################################################

cmake_minimum_required(VERSION 3.18)
project(apple CXX)

add_library(apple STATIC
        include/Foundation/Foundation.hpp
        include/Foundation/NSArray.hpp
        include/Foundation/NSAutoreleasePool.hpp
        include/Foundation/NSBundle.hpp
        include/Foundation/NSData.hpp
        include/Foundation/NSDate.hpp
        include/Foundation/NSDefines.hpp
        include/Foundation/NSDictionary.hpp
        include/Foundation/NSEnumerator.hpp
        include/Foundation/NSError.hpp
        include/Foundation/NSLock.hpp
        include/Foundation/NSNotification.hpp
        include/Foundation/NSNumber.hpp
        include/Foundation/NSObjCRuntime.hpp
        include/Foundation/NSObject.hpp
        include/Foundation/NSPrivate.hpp
        include/Foundation/NSProcessInfo.hpp
        include/Foundation/NSRange.hpp
        include/Foundation/NSString.hpp
        include/Foundation/NSTypes.hpp
        include/Foundation/NSURL.hpp
        include/Metal/Metal.hpp
        include/Metal/MTLAccelerationStructure.hpp
        include/Metal/MTLAccelerationStructureCommandEncoder.hpp
        include/Metal/MTLAccelerationStructureTypes.hpp
        include/Metal/MTLArgument.hpp
        include/Metal/MTLArgumentEncoder.hpp
        include/Metal/MTLBinaryArchive.hpp
        include/Metal/MTLBlitCommandEncoder.hpp
        include/Metal/MTLBlitPass.hpp
        include/Metal/MTLBuffer.hpp
        include/Metal/MTLCaptureManager.hpp
        include/Metal/MTLCaptureScope.hpp
        include/Metal/MTLCommandBuffer.hpp
        include/Metal/MTLCommandEncoder.hpp
        include/Metal/MTLCommandQueue.hpp
        include/Metal/MTLComputeCommandEncoder.hpp
        include/Metal/MTLComputePass.hpp
        include/Metal/MTLComputePipeline.hpp
        include/Metal/MTLCounters.hpp
        include/Metal/MTLDefines.hpp
        include/Metal/MTLDepthStencil.hpp
        include/Metal/MTLDevice.hpp
        include/Metal/MTLDrawable.hpp
        include/Metal/MTLDynamicLibrary.hpp
        include/Metal/MTLEvent.hpp
        include/Metal/MTLFence.hpp
        include/Metal/MTLFunctionConstantValues.hpp
        include/Metal/MTLFunctionDescriptor.hpp
        include/Metal/MTLFunctionHandle.hpp
        include/Metal/MTLFunctionLog.hpp
        include/Metal/MTLFunctionStitching.hpp
        include/Metal/MTLHeaderBridge.hpp
        include/Metal/MTLHeap.hpp
        include/Metal/MTLIndirectCommandBuffer.hpp
        include/Metal/MTLIndirectCommandEncoder.hpp
        include/Metal/MTLIntersectionFunctionTable.hpp
        include/Metal/MTLLibrary.hpp
        include/Metal/MTLLinkedFunctions.hpp
        include/Metal/MTLParallelRenderCommandEncoder.hpp
        include/Metal/MTLPipeline.hpp
        include/Metal/MTLPixelFormat.hpp
        include/Metal/MTLPrivate.hpp
        include/Metal/MTLRasterizationRate.hpp
        include/Metal/MTLRenderCommandEncoder.hpp
        include/Metal/MTLRenderPass.hpp
        include/Metal/MTLRenderPipeline.hpp
        include/Metal/MTLResource.hpp
        include/Metal/MTLResourceStateCommandEncoder.hpp
        include/Metal/MTLResourceStatePass.hpp
        include/Metal/MTLSampler.hpp
        include/Metal/MTLStageInputOutputDescriptor.hpp
        include/Metal/MTLTexture.hpp
        include/Metal/MTLTypes.hpp
        include/Metal/MTLVertexDescriptor.hpp
        include/Metal/MTLVisibleFunctionTable.hpp
        include/QuartzCore/CADefines.hpp
        include/QuartzCore/CAMetalDrawable.hpp
        include/QuartzCore/CAPrivate.hpp
        include/QuartzCore/QuartzCore.hpp
        src/Foundation.cpp
        src/Metal.cpp
        src/QuartzCore.cpp
)

target_include_directories(apple
    PUBLIC
        include
)

target_compile_features(apple
    PRIVATE
        cxx_std_17
)

target_link_libraries(apple
    PUBLIC
        "-framework Foundation"
        "-framework QuartzCore"
        "-framework Metal"
)
