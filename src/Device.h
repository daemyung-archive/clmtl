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

#ifndef CLMTL_DEVICE_H
#define CLMTL_DEVICE_H

#include <string>
#include <CL/cl_icd.h>
#include <Metal/Metal.hpp>

struct _cl_device_id {
    cl_icd_dispatch *Dispatch;
};

namespace clmtl {

class Platform;

struct DeviceLimits {
    cl_device_type Type;
    cl_uint VendorId;
    cl_uint MaxComputeUnits;
    cl_uint MaxWorkItemDimensions;
    size_t MaxWorkGroupSize;
    size_t MaxWorkItemSizes[3];
    cl_uint PreferredVectorWidthChar;
    cl_uint PreferredVectorWidthShort;
    cl_uint PreferredVectorWidthInt;
    cl_uint PreferredVectorWidthLong;
    cl_uint PreferredVectorWidthFloat;
    cl_uint PreferredVectorWidthDouble;
    cl_uint MaxClockFrequency;
    uint AddressBits;
    cl_uint MaxReadImageArgs;
    cl_uint MaxWriteImageArgs;
    cl_ulong MaxMemAllocSize;
    size_t Image2DMaxWidth;
    size_t Image2DMaxHeight;
    size_t Image3DMaxWidth;
    size_t Image3DMaxHeight;
    size_t Image3DMaxDepth;
    cl_bool ImageSupport;
    size_t MaxParameterSize;
    cl_uint MaxSamplers;
    cl_uint MemBaseAddrAlign;
    cl_uint MinDataTypeAlignSize;
    cl_device_fp_config SingleFpConfig;
    cl_device_mem_cache_type GlobalMemCacheType;
    cl_uint GlobalMemCachelineSize;
    cl_ulong GlobalMemCacheSize;
    cl_ulong GlobalMemSize;
    cl_ulong MaxConstantBufferSize;
    cl_uint MaxConstantArgs;
    cl_ulong LocalMemType;
    cl_ulong LocalMemSize;
    cl_bool ErrorCorrectionSupport;
    size_t ProfilingTimeResolution;
    cl_bool EndianLittle;
    cl_bool Available;
    cl_bool CompilerAvailable;
    cl_device_exec_capabilities ExecCapabilities;
    cl_command_queue_properties QueueOnHostProperties;
    std::string Name;
    std::string Vendor;
    std::string DriverVersion;
    std::string Profile;
    std::string Version;
    std::string Extensions;
    cl_platform_id Platform;
    cl_device_fp_config DoubleFpConfig;
    cl_uint PreferredVectorWidthHalf;
    cl_bool HostUnifiedMemory;
    cl_uint NativeVectorWidthChar;
    cl_uint NativeVectorWidthShort;
    cl_uint NativeVectorWidthInt;
    cl_uint NativeVectorWidthLong;
    cl_uint NativeVectorWidthFloat;
    cl_uint NativeVectorWidthDouble;
    cl_uint NativeVectorWidthHalf;
    std::string CVersion;
};

class Device : public _cl_device_id {
public:
    static Device *GetSingleton();
    static Device *DownCast(_cl_device_id *device);

public:
    ~Device();
    MTL::Device *GetDevice() const;
    DeviceLimits GetLimits() const;

private:
    MTL::Device *mDevice;
    DeviceLimits mLimits;

    Device();
    void InitLimits();
};

} //namespace clmtl

#endif //CLMTL_DEVICE_H
