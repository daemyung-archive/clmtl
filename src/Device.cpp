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

#include "Device.h"

#include "Dispatch.h"
#include "Platform.h"

_cl_device_id::_cl_device_id(cl_icd_dispatch *dispatch) :
        Dispatch{dispatch} {
    assert(Dispatch);
}

namespace clmtl {

Device *Device::GetSingleton() {
    static Device sDevice;
    return &sDevice;
}

Device *Device::DownCast(_cl_device_id *device) {
    return dynamic_cast<Device *>(device);
}

Device::~Device() {
    mDevice->release();
}

MTL::Device *Device::GetDevice() const {
    return mDevice;
}

DeviceLimits Device::GetLimits() const {
    return mLimits;
}

Device::Device()
        : _cl_device_id{Dispatch::GetTable()}, mDevice{MTL::CreateSystemDefaultDevice()} {
    InitLimits();
}

void Device::InitLimits() {
    mLimits.Type = CL_DEVICE_TYPE_DEFAULT | CL_DEVICE_TYPE_GPU;
    mLimits.VendorId = 0;
    mLimits.MaxComputeUnits = 1;
    mLimits.MaxWorkItemDimensions = 3;
    mLimits.MaxWorkGroupSize = 1;
    mLimits.MaxWorkItemSizes[0] = 1;
    mLimits.MaxWorkItemSizes[1] = 1;
    mLimits.MaxWorkItemSizes[2] = 1;
    mLimits.PreferredVectorWidthChar = 1;
    mLimits.PreferredVectorWidthShort = 2;
    mLimits.PreferredVectorWidthInt = 4;
    mLimits.PreferredVectorWidthLong = 8;
    mLimits.PreferredVectorWidthFloat = 4;
    mLimits.PreferredVectorWidthDouble = 0;
    mLimits.MaxClockFrequency = 1000;
    mLimits.AddressBits = 64;
    mLimits.MaxReadImageArgs = 32;
    mLimits.MaxWriteImageArgs = 32;
    mLimits.MaxMemAllocSize = mDevice->maxBufferLength();
    mLimits.Image2DMaxWidth = 8192;
    mLimits.Image2DMaxHeight = 8192;
    mLimits.Image3DMaxWidth = 2048;
    mLimits.Image3DMaxHeight = 2048;
    mLimits.Image3DMaxDepth = 2048;
    mLimits.ImageSupport = CL_TRUE;
    mLimits.MaxParameterSize = 256;
    mLimits.MaxSamplers = mDevice->maxArgumentBufferSamplerCount();
    mLimits.MemBaseAddrAlign = 64;
    mLimits.MinDataTypeAlignSize = 128;
    mLimits.SingleFpConfig = CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN;
    mLimits.GlobalMemCacheType = 0;
    mLimits.GlobalMemCachelineSize = 0;
    mLimits.GlobalMemCacheSize = 0;
    mLimits.GlobalMemSize = 0;
    mLimits.MaxConstantBufferSize = 64;
    mLimits.MaxConstantArgs = 8;
    mLimits.LocalMemType = CL_LOCAL;
    mLimits.LocalMemSize = 0;
    mLimits.ErrorCorrectionSupport = CL_FALSE;
    mLimits.ProfilingTimeResolution = 1;
    mLimits.EndianLittle = CL_TRUE;
    mLimits.Available = CL_TRUE;
    mLimits.CompilerAvailable = CL_FALSE;
    mLimits.ExecCapabilities = CL_EXEC_KERNEL;
    mLimits.QueueOnHostProperties = CL_QUEUE_PROFILING_ENABLE;
    mLimits.Name = mDevice->name()->utf8String();
    mLimits.Vendor = Platform::GetVendor();
    mLimits.DriverVersion = "0.1";
    mLimits.Profile = Platform::GetProfile();
    mLimits.Version = Platform::GetVersion();
    mLimits.Extensions = "";
    mLimits.Platform = Platform::GetSingleton();
    mLimits.DoubleFpConfig = CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_ROUND_TO_ZERO | CL_FP_ROUND_TO_INF |
                             CL_FP_INF_NAN | CL_FP_DENORM;
    mLimits.PreferredVectorWidthHalf = 2;
    mLimits.HostUnifiedMemory = mDevice->hasUnifiedMemory();
    mLimits.NativeVectorWidthChar = 1;
    mLimits.NativeVectorWidthShort = 2;
    mLimits.NativeVectorWidthInt = 4;
    mLimits.NativeVectorWidthLong = 8;
    mLimits.NativeVectorWidthFloat = 4;
    mLimits.NativeVectorWidthDouble = 8;
    mLimits.NativeVectorWidthHalf = 2;
    mLimits.CVersion = "OpenCL C 1.0";
}

} //namespace clmtl
