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

#include "Util.h"
#include "Dispatch.h"
#include "Platform.h"
#include "Device.h"

/***********************************************************************************************************************
* OpenCL Core APIs
***********************************************************************************************************************/

/***********************************************************************************************************************
* Platform APIs
***********************************************************************************************************************/

cl_int clGetPlatformIDs(cl_uint num_entries, cl_platform_id *platforms, cl_uint *num_platforms) {
    if (!num_entries && platforms) {
        return CL_INVALID_VALUE;
    }

    if (!platforms && !num_platforms) {
        return CL_INVALID_VALUE;
    }

    if (platforms) {
        platforms[0] = clmtl::Platform::GetSingleton();
    }

    if (num_platforms) {
        num_platforms[0] = 1;
    }

    return CL_SUCCESS;
}

cl_int clGetPlatformInfo(cl_platform_id platform, cl_platform_info param_name, size_t param_value_size,
                         void *param_value, size_t *param_value_size_ret) {
    if (platform != clmtl::Platform::GetSingleton()) {
        return CL_INVALID_PLATFORM;
    }

    std::string info;

    switch (param_name) {
        case CL_PLATFORM_PROFILE:
            info = clmtl::Platform::GetProfile();
            break;
        case CL_PLATFORM_VERSION:
            info = clmtl::Platform::GetVersion();
            break;
        case CL_PLATFORM_NAME:
            info = clmtl::Platform::GetName();
            break;
        case CL_PLATFORM_VENDOR:
            info = clmtl::Platform::GetVendor();
            break;
        case CL_PLATFORM_EXTENSIONS:
            info = clmtl::Platform::GetExtensions();
            break;
        case CL_PLATFORM_ICD_SUFFIX_KHR:
            info = clmtl::Platform::GetSuffix();
            break;
        default:
            return CL_INVALID_VALUE;
    }

    if (param_value) {
        if (param_value_size <= info.size()) {
            return CL_INVALID_VALUE;
        } else {
            memcpy(param_value, info.c_str(), info.size() + 1);
        }
    }

    if (param_value_size_ret) {
        param_value_size_ret[0] = info.size() + 1;
    }

    return CL_SUCCESS;
}

/***********************************************************************************************************************
* Device APIs
***********************************************************************************************************************/

cl_int clGetDeviceIDs(cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices,
                      cl_uint *num_devices) {
    if (platform != clmtl::Platform::GetSingleton()) {
        return CL_INVALID_PLATFORM;
    }

    if (!num_entries && devices) {
        return CL_INVALID_VALUE;
    }

    if (!devices && !num_devices) {
        return CL_INVALID_VALUE;
    }

    if (clmtl::Util::TestAnyFlagSet(device_type, CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_ACCELERATOR)) {
        return CL_DEVICE_NOT_FOUND;
    }

    if (devices) {
        if (num_entries < 1) {
            return CL_INVALID_VALUE;
        } else {
            devices[0] = clmtl::Device::GetSingleton();
        }
    }

    if (num_devices) {
        num_devices[0] = 1;
    }

    return CL_SUCCESS;
}

cl_int clGetDeviceInfo(cl_device_id device, cl_device_info param_name, size_t param_value_size, void *param_value,
                       size_t *param_value_size_ret) {
    if (!device) {
        return CL_INVALID_DEVICE;
    }

    auto limits = clmtl::Device::DownCast(device)->GetLimits();
    void *info;
    size_t size;

    switch (param_name) {
        case CL_DEVICE_TYPE:
            info = &limits.Type;
            size = sizeof(cl_device_type);
            break;
        case CL_DEVICE_VENDOR_ID:
            info = &limits.VendorId;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_COMPUTE_UNITS:
            info = &limits.MaxComputeUnits;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
            info = &limits.MaxWorkItemDimensions;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_WORK_GROUP_SIZE:
            info = &limits.MaxWorkGroupSize;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_MAX_WORK_ITEM_SIZES:
            info = limits.MaxWorkItemSizes;
            size = sizeof(size_t) * 3;
            break;
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:
            info = &limits.PreferredVectorWidthChar;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:
            info = &limits.PreferredVectorWidthShort;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:
            info = &limits.PreferredVectorWidthInt;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:
            info = &limits.PreferredVectorWidthLong;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:
            info = &limits.PreferredVectorWidthFloat;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:
            info = &limits.PreferredVectorWidthDouble;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_CLOCK_FREQUENCY:
            info = &limits.MaxClockFrequency;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_ADDRESS_BITS:
            info = &limits.AddressBits;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_READ_IMAGE_ARGS:
            info = &limits.MaxReadImageArgs;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_WRITE_IMAGE_ARGS:
            info = &limits.MaxWriteImageArgs;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
            info = &limits.MaxMemAllocSize;
            size = sizeof(cl_ulong);
            break;
        case CL_DEVICE_IMAGE2D_MAX_WIDTH:
            info = &limits.Image2DMaxWidth;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_IMAGE2D_MAX_HEIGHT:
            info = &limits.Image2DMaxHeight;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_IMAGE3D_MAX_WIDTH:
            info = &limits.Image3DMaxWidth;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_IMAGE3D_MAX_HEIGHT:
            info = &limits.Image3DMaxHeight;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_IMAGE3D_MAX_DEPTH:
            info = &limits.Image3DMaxDepth;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_IMAGE_SUPPORT:
            info = &limits.ImageSupport;
            size = sizeof(cl_bool);
            break;
        case CL_DEVICE_MAX_PARAMETER_SIZE:
            info = &limits.MaxParameterSize;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_MAX_SAMPLERS:
            info = &limits.MaxSamplers;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MEM_BASE_ADDR_ALIGN:
            info = &limits.MemBaseAddrAlign;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE:
            info = &limits.MinDataTypeAlignSize;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_SINGLE_FP_CONFIG:
            info = &limits.SingleFpConfig;
            size = sizeof(cl_device_fp_config);
            break;
        case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE:
            info = &limits.GlobalMemCacheType;
            size = sizeof(cl_device_mem_cache_type);
            break;
        case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:
            info = &limits.GlobalMemCachelineSize;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
            info = &limits.GlobalMemCacheSize;
            size = sizeof(cl_ulong);
            break;
        case CL_DEVICE_GLOBAL_MEM_SIZE:
            info = &limits.GlobalMemSize;
            size = sizeof(cl_ulong);
            break;
        case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:
            info = &limits.MaxConstantBufferSize;
            size = sizeof(cl_ulong);
            break;
        case CL_DEVICE_MAX_CONSTANT_ARGS:
            info = &limits.MaxConstantArgs;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_LOCAL_MEM_TYPE:
            info = &limits.LocalMemType;
            size = sizeof(cl_ulong);
            break;
        case CL_DEVICE_LOCAL_MEM_SIZE:
            info = &limits.LocalMemSize;
            size = sizeof(cl_ulong);
            break;
        case CL_DEVICE_ERROR_CORRECTION_SUPPORT:
            info = &limits.ErrorCorrectionSupport;
            size = sizeof(cl_bool);
            break;
        case CL_DEVICE_PROFILING_TIMER_RESOLUTION:
            info = &limits.ProfilingTimeResolution;
            size = sizeof(size_t);
            break;
        case CL_DEVICE_ENDIAN_LITTLE:
            info = &limits.EndianLittle;
            size = sizeof(cl_bool);
            break;
        case CL_DEVICE_AVAILABLE:
            info = &limits.Available;
            size = sizeof(cl_bool);
            break;
        case CL_DEVICE_COMPILER_AVAILABLE:
            info = &limits.CompilerAvailable;
            size = sizeof(cl_bool);
            break;
        case CL_DEVICE_EXECUTION_CAPABILITIES:
            info = &limits.ExecCapabilities;
            size = sizeof(cl_device_exec_capabilities);
            break;
        case CL_DEVICE_QUEUE_PROPERTIES:
            info = &limits.QueueOnHostProperties;
            size = sizeof(cl_command_queue_properties);
            break;
#ifdef CL_VERSION_2_0
        case CL_DEVICE_QUEUE_ON_HOST_PROPERTIES:
            break;
#endif //CL_VERSION_2_0
        case CL_DEVICE_NAME:
            info = limits.Name.data();
            size = limits.Name.size() + 1;
            break;
        case CL_DEVICE_VENDOR:
            info = limits.Vendor.data();
            size = limits.Vendor.size() + 1;
            break;
        case CL_DRIVER_VERSION:
            info = limits.DriverVersion.data();
            size = limits.DriverVersion.size() + 1;
            break;
        case CL_DEVICE_PROFILE:
            info = limits.Profile.data();
            size = limits.Profile.size() + 1;
            break;
        case CL_DEVICE_VERSION:
            info = limits.Version.data();
            size = limits.Version.size() + 1;
            break;
        case CL_DEVICE_EXTENSIONS:
            info = limits.Extensions.data();
            size = limits.Extensions.size() + 1;
            break;
        case CL_DEVICE_PLATFORM:
            info = limits.Platform;
            size = sizeof(cl_platform_id);
            break;
        case CL_DEVICE_DOUBLE_FP_CONFIG:
            info = &limits.DoubleFpConfig;
            size = sizeof(cl_device_fp_config);
            break;
#ifdef CL_VERSION_1_1
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF:
            info = &limits.PreferredVectorWidthHalf;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_HOST_UNIFIED_MEMORY:
            info = &limits.HostUnifiedMemory;
            size = sizeof(cl_bool);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR:
            info = &limits.NativeVectorWidthChar;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT:
            info = &limits.NativeVectorWidthShort;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_INT:
            info = &limits.NativeVectorWidthInt;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG:
            info = &limits.NativeVectorWidthLong;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT:
            info = &limits.NativeVectorWidthFloat;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE:
            info = &limits.NativeVectorWidthDouble;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF:
            info = &limits.NativeVectorWidthHalf;
            size = sizeof(cl_uint);
            break;
        case CL_DEVICE_OPENCL_C_VERSION:
            info = limits.CVersion.data();
            size = limits.CVersion.size() + 1;
            break;
#endif //CL_VERSION_1_1
        default:
            return CL_INVALID_VALUE;
    }

    if (param_value) {
        if (param_value_size < size) {
            return CL_INVALID_VALUE;
        } else {
            memcpy(param_value, info, size);
        }
    }

    if (param_value_size_ret) {
        param_value_size_ret[0] = size;
    }

    return CL_SUCCESS;
}

#ifdef CL_VERSION_1_2

cl_int clCreateSubDevices(cl_device_id in_device, const cl_device_partition_property *properties, cl_uint num_devices,
                          cl_device_id *out_devices, cl_uint *num_devices_ret) {
    return CL_INVALID_DEVICE;
}

cl_int clRetainDevice(cl_device_id device) {
    return CL_INVALID_DEVICE;
}

cl_int clReleaseDevice(cl_device_id device) {
    return CL_INVALID_DEVICE;
}

#endif

#ifdef CL_VERSION_2_1

cl_int clSetDefaultDeviceCommandQueue(cl_context context, cl_device_id device, cl_command_queue command_queue) {
    return CL_INVALID_CONTEXT;
}

cl_int clGetDeviceAndHostTimer(cl_device_id device, cl_ulong *device_timestamp, cl_ulong *host_timestamp) {
    return CL_INVALID_DEVICE;
}

cl_int clGetHostTimer(cl_device_id device, cl_ulong *host_timestamp) {
    return CL_INVALID_DEVICE;
}

#endif

/***********************************************************************************************************************
* Context APIs
***********************************************************************************************************************/

cl_context clCreateContext(const cl_context_properties *properties, cl_uint num_devices, const cl_device_id *devices,
                           void (*pfn_notify)(const char *errinfo, const void *private_info, size_t cb,
                                              void *user_data),
                           void *user_data, cl_int *errcode_ret) {
    return nullptr;
}

cl_context clCreateContextFromType(const cl_context_properties *properties, cl_device_type device_type,
                                   void (*pfn_notify)(const char *errinfo, const void *private_info, size_t cb,
                                                      void *user_data),
                                   void *user_data, cl_int *errcode_ret) {
    return nullptr;
}

cl_int clRetainContext(cl_context context) {
    return CL_INVALID_CONTEXT;
}

cl_int clReleaseContext(cl_context context) {
    return CL_INVALID_CONTEXT;
}

cl_int clGetContextInfo(cl_context context, cl_context_info param_name, size_t param_value_size, void *param_value,
                        size_t *param_value_size_ret) {
    return CL_INVALID_CONTEXT;
}

#ifdef CL_VERSION_3_0

cl_int clSetContextDestructorCallback(cl_context context, void (*pfn_notify)(cl_context context, void *user_data),
                                      void *user_data) {
    return CL_INVALID_CONTEXT;
}

#endif

/***********************************************************************************************************************
* Command Queue APIs
***********************************************************************************************************************/

#ifdef CL_VERSION_2_0

cl_command_queue clCreateCommandQueueWithProperties(cl_context context, cl_device_id device,
                                                    const cl_queue_properties *properties, cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_int clRetainCommandQueue(cl_command_queue command_queue) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clReleaseCommandQueue(cl_command_queue command_queue) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clGetCommandQueueInfo(cl_command_queue command_queue, cl_command_queue_info param_name, size_t param_value_size,
                             void *param_value, size_t *param_value_size_ret) {
    return CL_INVALID_COMMAND_QUEUE;
}

/* Memory Object APIs */
cl_mem clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret) {
    return nullptr;
}

#ifdef CL_VERSION_1_1

cl_mem clCreateSubBuffer(cl_mem buffer, cl_mem_flags flags, cl_buffer_create_type buffer_create_type,
                         const void *buffer_create_info, cl_int *errcode_ret) {
    return nullptr;
}

#endif

#ifdef CL_VERSION_1_2

cl_mem clCreateImage(cl_context context, cl_mem_flags flags, const cl_image_format *image_format,
                     const cl_image_desc *image_desc, void *host_ptr, cl_int *errcode_ret) {
    return nullptr;
}

#endif

#ifdef CL_VERSION_2_0

cl_mem clCreatePipe(cl_context context, cl_mem_flags flags, cl_uint pipe_packet_size, cl_uint pipe_max_packets,
                    const cl_pipe_properties *properties, cl_int *errcode_ret) {
    return nullptr;
}

#endif

#ifdef CL_VERSION_3_0

cl_mem clCreateBufferWithProperties(cl_context context, const cl_mem_properties *properties, cl_mem_flags flags,
                                    size_t size, void *host_ptr, cl_int *errcode_ret) {
    return nullptr;
}

cl_mem clCreateImageWithProperties(cl_context context, const cl_mem_properties *properties, cl_mem_flags flags,
                                   const cl_image_format *image_format, const cl_image_desc *image_desc, void *host_ptr,
                                   cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_int clRetainMemObject(cl_mem memobj) {
    return CL_INVALID_MEM_OBJECT;
}

cl_int clReleaseMemObject(cl_mem memobj) {
    return CL_INVALID_MEM_OBJECT;
}

cl_int clGetSupportedImageFormats(cl_context context, cl_mem_flags flags, cl_mem_object_type image_type,
                                  cl_uint num_entries, cl_image_format *image_formats, cl_uint *num_image_formats) {
    return CL_INVALID_CONTEXT;
}

cl_int clGetMemObjectInfo(cl_mem memobj, cl_mem_info param_name, size_t param_value_size, void *param_value,
                          size_t *param_value_size_ret) {
    return CL_INVALID_MEM_OBJECT;
}

cl_int clGetImageInfo(cl_mem image, cl_image_info param_name, size_t param_value_size, void *param_value,
                      size_t *param_value_size_ret) {
    return CL_INVALID_MEM_OBJECT;
}

#ifdef CL_VERSION_2_0

cl_int clGetPipeInfo(cl_mem pipe, cl_pipe_info param_name, size_t param_value_size, void *param_value,
                     size_t *param_value_size_ret) {
    return CL_INVALID_MEM_OBJECT;
}

#endif

#ifdef CL_VERSION_1_1

cl_int clSetMemObjectDestructorCallback(cl_mem memobj, void (*pfn_notify)(cl_mem memobj, void *user_data),
                                        void *user_data) {
    return CL_INVALID_MEM_OBJECT;
}

#endif

/***********************************************************************************************************************
* SVM Allocation APIs
***********************************************************************************************************************/

#ifdef CL_VERSION_2_0

void *clSVMAlloc(cl_context context, cl_svm_mem_flags flags, size_t size, cl_uint alignment) {
    return nullptr;
}

void clSVMFree(cl_context context, void *svm_pointer) {
}

#endif

/***********************************************************************************************************************
* Sampler APIs
***********************************************************************************************************************/

#ifdef CL_VERSION_2_0

cl_sampler clCreateSamplerWithProperties(cl_context context, const cl_sampler_properties *sampler_properties,
                                         cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_int clRetainSampler(cl_sampler sampler) {
    return CL_INVALID_SAMPLER;
}

cl_int clReleaseSampler(cl_sampler sampler) {
    return CL_INVALID_SAMPLER;
}

cl_int clGetSamplerInfo(cl_sampler sampler, cl_sampler_info param_name, size_t param_value_size, void *param_value,
                        size_t *param_value_size_ret) {
    return CL_INVALID_SAMPLER;
}

/***********************************************************************************************************************
* Program Object APIs
***********************************************************************************************************************/

cl_program clCreateProgramWithSource(cl_context context, cl_uint count, const char **strings, const size_t *lengths,
                                     cl_int *errcode_ret) {
    return nullptr;
}

cl_program clCreateProgramWithBinary(cl_context context, cl_uint num_devices, const cl_device_id *device_list,
                                     const size_t *lengths, const unsigned char **binaries, cl_int *binary_status,
                                     cl_int *errcode_ret) {
    return nullptr;
}

#ifdef CL_VERSION_1_2

cl_program clCreateProgramWithBuiltInKernels(cl_context context, cl_uint num_devices, const cl_device_id *device_list,
                                             const char *kernel_names, cl_int *errcode_ret) {
    return nullptr;
}

#endif

#ifdef CL_VERSION_2_1

cl_program clCreateProgramWithIL(cl_context context, const void *il, size_t length, cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_int clRetainProgram(cl_program program) {
    return CL_INVALID_PROGRAM;
}

cl_int clReleaseProgram(cl_program program) {
    return CL_INVALID_PROGRAM;
}

cl_int clBuildProgram(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options,
                      void (*pfn_notify)(cl_program program, void *user_data), void *user_data) {
    return CL_INVALID_PROGRAM;
}

#ifdef CL_VERSION_1_2

cl_int clCompileProgram(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options,
                        cl_uint num_input_headers, const cl_program *input_headers, const char **header_include_names,
                        void (CL_CALLBACK *pfn_notify)(cl_program program, void *user_data), void *user_data) {
    return CL_INVALID_PROGRAM;
}

cl_program clLinkProgram(cl_context context, cl_uint num_devices, const cl_device_id *device_list, const char *options,
                         cl_uint num_input_programs, const cl_program *input_programs,
                         void (*pfn_notify)(cl_program program, void *user_data), void *user_data,
                         cl_int *errcode_ret) {
    return nullptr;
}

#endif

#ifdef CL_VERSION_2_2

cl_int clSetProgramReleaseCallback(cl_program program, void (*pfn_notify)(cl_program program, void *user_data),
                                   void *user_data) {
    return CL_INVALID_PROGRAM;
}

cl_int clSetProgramSpecializationConstant(cl_program program, cl_uint spec_id, size_t spec_size,
                                          const void *spec_value) {
    return CL_INVALID_PROGRAM;
}

#endif

#ifdef CL_VERSION_1_2

cl_int clUnloadPlatformCompiler(cl_platform_id platform) {
    return CL_INVALID_PLATFORM;
}

#endif

cl_int clGetProgramInfo(cl_program program, cl_program_info param_name, size_t param_value_size, void *param_value,
                        size_t *param_value_size_ret) {
    return CL_INVALID_PROGRAM;
}

cl_int clGetProgramBuildInfo(cl_program program, cl_device_id device, cl_program_build_info param_name,
                             size_t param_value_size, void *param_value, size_t *param_value_size_ret) {
    return CL_INVALID_PROGRAM;
}

/***********************************************************************************************************************
* Kernel Object APIs
***********************************************************************************************************************/

cl_kernel clCreateKernel(cl_program program, const char *kernel_name, cl_int *errcode_ret) {
    return nullptr;
}

cl_int clCreateKernelsInProgram(cl_program program, cl_uint num_kernels, cl_kernel *kernels, cl_uint *num_kernels_ret) {
    return CL_INVALID_PROGRAM;
}

#ifdef CL_VERSION_2_1

cl_kernel clCloneKernel(cl_kernel source_kernel, cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_int clRetainKernel(cl_kernel kernel) {
    return CL_INVALID_KERNEL;
}

cl_int clReleaseKernel(cl_kernel kernel) {
    return CL_INVALID_KERNEL;
}

cl_int clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value) {
    return CL_INVALID_KERNEL;
}

#ifdef CL_VERSION_2_0

cl_int clSetKernelArgSVMPointer(cl_kernel kernel, cl_uint arg_index, const void *arg_value) {
    return CL_INVALID_KERNEL;
}

cl_int clSetKernelExecInfo(cl_kernel kernel, cl_kernel_exec_info param_name, size_t param_value_size,
                           const void *param_value) {
    return CL_INVALID_KERNEL;
}

#endif

cl_int clGetKernelInfo(cl_kernel kernel, cl_kernel_info param_name, size_t param_value_size, void *param_value,
                       size_t *param_value_size_ret) {
    return CL_INVALID_KERNEL;
}

#ifdef CL_VERSION_1_2

cl_int clGetKernelArgInfo(cl_kernel kernel, cl_uint arg_indx, cl_kernel_arg_info param_name, size_t param_value_size,
                          void *param_value, size_t *param_value_size_ret) {
    return CL_INVALID_KERNEL;
}

#endif

cl_int clGetKernelWorkGroupInfo(cl_kernel kernel, cl_device_id device, cl_kernel_work_group_info param_name,
                                size_t param_value_size, void *param_value, size_t *param_value_size_ret) {
    return CL_INVALID_KERNEL;
}

#ifdef CL_VERSION_2_1

cl_int clGetKernelSubGroupInfo(cl_kernel kernel, cl_device_id device, cl_kernel_sub_group_info param_name,
                               size_t input_value_size, const void *input_value, size_t param_value_size,
                               void *param_value, size_t *param_value_size_ret) {
    return CL_INVALID_KERNEL;
}

#endif

/***********************************************************************************************************************
* Event Object APIs
***********************************************************************************************************************/

cl_int clWaitForEvents(cl_uint num_events, const cl_event *event_list) {
    return CL_INVALID_EVENT;
}

cl_int clGetEventInfo(cl_event event, cl_event_info param_name, size_t param_value_size, void *param_value,
                      size_t *param_value_size_ret) {
    return CL_INVALID_EVENT;
}

#ifdef CL_VERSION_1_1

cl_event clCreateUserEvent(cl_context context, cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_int clRetainEvent(cl_event event) {
    return CL_INVALID_EVENT;
}

cl_int clReleaseEvent(cl_event event) {
    return CL_INVALID_EVENT;
}

#ifdef CL_VERSION_1_1

cl_int clSetUserEventStatus(cl_event event, cl_int execution_status) {
    return CL_INVALID_EVENT;
}

cl_int clSetEventCallback(cl_event event, cl_int command_exec_callback_type,
                          void (*pfn_notify)(cl_event event, cl_int event_command_status, void *user_data),
                          void *user_data) {
    return CL_INVALID_EVENT;
}

#endif

/***********************************************************************************************************************
* Profiling APIs
***********************************************************************************************************************/

cl_int clGetEventProfilingInfo(cl_event event, cl_profiling_info param_name, size_t param_value_size, void *param_value,
                               size_t *param_value_size_ret) {
    return CL_INVALID_EVENT;
}

/***********************************************************************************************************************
* Flush and Finish APIs
***********************************************************************************************************************/

cl_int clFlush(cl_command_queue command_queue) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clFinish(cl_command_queue command_queue) {
    return CL_INVALID_COMMAND_QUEUE;
}

/***********************************************************************************************************************
* Enqueued Commands APIs
***********************************************************************************************************************/

cl_int clEnqueueReadBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset,
                           size_t size, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                           cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#ifdef CL_VERSION_1_1

cl_int clEnqueueReadBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read,
                               const size_t *buffer_origin, const size_t *host_origin, const size_t *region,
                               size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch,
                               size_t host_slice_pitch, void *ptr, cl_uint num_events_in_wait_list,
                               const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

cl_int clEnqueueWriteBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset,
                            size_t size, const void *ptr, cl_uint num_events_in_wait_list,
                            const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#ifdef CL_VERSION_1_1

cl_int clEnqueueWriteBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write,
                                const size_t *buffer_origin, const size_t *host_origin, const size_t *region,
                                size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch,
                                size_t host_slice_pitch, const void *ptr, cl_uint num_events_in_wait_list,
                                const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

#ifdef CL_VERSION_1_2

cl_int clEnqueueFillBuffer(cl_command_queue command_queue, cl_mem buffer, const void *pattern, size_t pattern_size,
                           size_t offset, size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                           cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

cl_int clEnqueueCopyBuffer(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, size_t src_offset,
                           size_t dst_offset, size_t size, cl_uint num_events_in_wait_list,
                           const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#ifdef CL_VERSION_1_1

cl_int clEnqueueCopyBufferRect(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer,
                               const size_t *src_origin, const size_t *dst_origin, const size_t *region,
                               size_t src_row_pitch, size_t src_slice_pitch, size_t dst_row_pitch,
                               size_t dst_slice_pitch, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                               cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

cl_int clEnqueueReadImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_read, const size_t *origin,
                          const size_t *region, size_t row_pitch, size_t slice_pitch, void *ptr,
                          cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueWriteImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_write, const size_t *origin,
                           const size_t *region, size_t input_row_pitch, size_t input_slice_pitch, const void *ptr,
                           cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#ifdef CL_VERSION_1_2

cl_int clEnqueueFillImage(cl_command_queue command_queue, cl_mem image, const void *fill_color, const size_t *origin,
                          const size_t *region, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                          cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

cl_int clEnqueueCopyImage(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_image, const size_t *src_origin,
                          const size_t *dst_origin, const size_t *region, cl_uint num_events_in_wait_list,
                          const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueCopyImageToBuffer(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_buffer,
                                  const size_t *src_origin, const size_t *region, size_t dst_offset,
                                  cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueCopyBufferToImage(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image,
                                  size_t src_offset, const size_t *dst_origin, const size_t *region,
                                  cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

void *clEnqueueMapBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags,
                         size_t offset, size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                         cl_event *event, cl_int *errcode_ret) {
    return nullptr;
}

void *clEnqueueMapImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_map, cl_map_flags map_flags,
                        const size_t *origin, const size_t *region, size_t *image_row_pitch, size_t *image_slice_pitch,
                        cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event,
                        cl_int *errcode_ret) {
    return nullptr;
}

cl_int clEnqueueUnmapMemObject(cl_command_queue command_queue, cl_mem memobj, void *mapped_ptr,
                               cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#ifdef CL_VERSION_1_2

cl_int clEnqueueMigrateMemObjects(cl_command_queue command_queue, cl_uint num_mem_objects, const cl_mem *mem_objects,
                                  cl_mem_migration_flags flags, cl_uint num_events_in_wait_list,
                                  const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

cl_int clEnqueueNDRangeKernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim,
                              const size_t *global_work_offset, const size_t *global_work_size,
                              const size_t *local_work_size, cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueNativeKernel(cl_command_queue command_queue, void (*user_func)(void *), void *args, size_t cb_args,
                             cl_uint num_mem_objects, const cl_mem *mem_list, const void **args_mem_loc,
                             cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#ifdef CL_VERSION_1_2

cl_int clEnqueueMarkerWithWaitList(cl_command_queue command_queue, cl_uint num_events_in_wait_list,
                                   const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueBarrierWithWaitList(cl_command_queue command_queue, cl_uint num_events_in_wait_list,
                                    const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

#ifdef CL_VERSION_2_0

cl_int clEnqueueSVMFree(cl_command_queue command_queue, cl_uint num_svm_pointers, void *svm_pointers[],
                        void (*pfn_free_func)(cl_command_queue queue, cl_uint num_svm_pointers, void *svm_pointers[],
                                              void *user_data),
                        void *user_data, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                        cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueSVMMemcpy(cl_command_queue command_queue, cl_bool blocking_copy, void *dst_ptr, const void *src_ptr,
                          size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                          cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueSVMMemFill(cl_command_queue command_queue, void *svm_ptr, const void *pattern, size_t pattern_size,
                           size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                           cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueSVMMap(cl_command_queue command_queue, cl_bool blocking_map, cl_map_flags flags, void *svm_ptr,
                       size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueSVMUnmap(cl_command_queue command_queue, void *svm_ptr, cl_uint num_events_in_wait_list,
                         const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

#ifdef CL_VERSION_2_1

cl_int clEnqueueSVMMigrateMem(cl_command_queue command_queue, cl_uint num_svm_pointers, const void **svm_pointers,
                              const size_t *sizes, cl_mem_migration_flags flags, cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif

#ifdef CL_VERSION_1_2

void *clGetExtensionFunctionAddressForPlatform(cl_platform_id platform, const char *func_name) {
    return nullptr;
}

#endif

#ifdef CL_USE_DEPRECATED_OPENCL_1_0_APIS

cl_int clSetCommandQueueProperty(cl_command_queue command_queue, cl_command_queue_properties properties, cl_bool enable,
                                 cl_command_queue_properties *old_properties) {
    return CL_INVALID_COMMAND_QUEUE;
}

#endif /* CL_USE_DEPRECATED_OPENCL_1_0_APIS */

/***********************************************************************************************************************
* Deprecated OpenCL 1.1 APIs
***********************************************************************************************************************/

cl_mem clCreateImage2D(cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t image_width,
                       size_t image_height, size_t image_row_pitch, void *host_ptr, cl_int *errcode_ret) {
    return nullptr;
}

cl_mem clCreateImage3D(cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t image_width,
                       size_t image_height, size_t image_depth, size_t image_row_pitch, size_t image_slice_pitch,
                       void *host_ptr, cl_int *errcode_ret) {
    return nullptr;
}

cl_int clEnqueueMarker(cl_command_queue command_queue, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueWaitForEvents(cl_command_queue command_queue, cl_uint num_events, const cl_event *event_list) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueBarrier(cl_command_queue command_queue) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clUnloadCompiler(void) {
    return CL_SUCCESS;
}

void *clGetExtensionFunctionAddress(const char *func_name) {
    return func_name ? clmtl::Dispatch::GetExtensionSymbol(func_name) : nullptr;
}

/***********************************************************************************************************************
* Deprecated OpenCL 2.0 APIs
***********************************************************************************************************************/

cl_command_queue clCreateCommandQueue(cl_context context, cl_device_id device, cl_command_queue_properties properties,
                                      cl_int *errcode_ret) {
    return nullptr;
}

cl_sampler clCreateSampler(cl_context context, cl_bool normalized_coords, cl_addressing_mode addressing_mode,
                           cl_filter_mode filter_mode, cl_int *errcode_ret) {
    return nullptr;
}

cl_int clEnqueueTask(cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list,
                     const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

/***********************************************************************************************************************
* OpenCL EGL APIs
***********************************************************************************************************************/

cl_mem clCreateFromEGLImageKHR(cl_context context, CLeglDisplayKHR egldisplay, CLeglImageKHR eglimage,
                               cl_mem_flags flags, const cl_egl_image_properties_khr *properties, cl_int *errcode_ret) {
    return nullptr;
}

cl_int clEnqueueAcquireEGLObjectsKHR(cl_command_queue command_queue, cl_uint num_objects, const cl_mem *mem_objects,
                                     cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                                     cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueReleaseEGLObjectsKHR(cl_command_queue command_queue, cl_uint num_objects, const cl_mem *mem_objects,
                                     cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                                     cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_event clCreateEventFromEGLSyncKHR(cl_context context, CLeglSyncKHR sync, CLeglDisplayKHR display,
                                     cl_int *errcode_ret) {
    return nullptr;
}

/***********************************************************************************************************************
* OpenCL Extension APIs
***********************************************************************************************************************/

/***********************************************************************************************************************
* cl_khr_icd extension
***********************************************************************************************************************/

cl_int clIcdGetPlatformIDsKHR(cl_uint num_entries, cl_platform_id *platforms, cl_uint *num_platforms) {
    if (!num_entries && platforms) {
        return CL_INVALID_VALUE;
    }

    if (!platforms && !num_platforms) {
        return CL_INVALID_VALUE;
    }

    if (!clmtl::Platform::GetSingleton()) {
        return CL_PLATFORM_NOT_FOUND_KHR;
    }

    if (platforms) {
        platforms[0] = clmtl::Platform::GetSingleton();
    }

    if (num_platforms) {
        num_platforms[0] = 1;
    }

    return CL_SUCCESS;
}

/***********************************************************************************************************************
* OpenCL OpenGL APIs
***********************************************************************************************************************/

cl_mem clCreateFromGLBuffer(cl_context context, cl_mem_flags flags, cl_GLuint bufobj, cl_int *errcode_ret) {
    return nullptr;
}

#ifdef CL_VERSION_1_2

cl_mem clCreateFromGLTexture(cl_context context, cl_mem_flags flags, cl_GLenum target, cl_GLint miplevel,
                             cl_GLuint texture, cl_int *errcode_ret) {
    return nullptr;
}

#endif

cl_mem clCreateFromGLRenderbuffer(cl_context context, cl_mem_flags flags, cl_GLuint renderbuffer, cl_int *errcode_ret) {
    return nullptr;
}

cl_int clGetGLObjectInfo(cl_mem memobj, cl_gl_object_type *gl_object_type, cl_GLuint *gl_object_name) {
    return CL_INVALID_MEM_OBJECT;
}

cl_int clGetGLTextureInfo(cl_mem memobj, cl_gl_texture_info param_name, size_t param_value_size, void *param_value,
                          size_t *param_value_size_ret) {
    return CL_INVALID_MEM_OBJECT;
}

cl_int clEnqueueAcquireGLObjects(cl_command_queue command_queue, cl_uint num_objects, const cl_mem *mem_objects,
                                 cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

cl_int clEnqueueReleaseGLObjects(cl_command_queue command_queue, cl_uint num_objects, const cl_mem *mem_objects,
                                 cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    return CL_INVALID_COMMAND_QUEUE;
}

/***********************************************************************************************************************
* Deprecated OpenCL 1.1 APIs
***********************************************************************************************************************/

cl_mem clCreateFromGLTexture2D(cl_context context, cl_mem_flags flags, cl_GLenum target, cl_GLint miplevel,
                               cl_GLuint texture, cl_int *errcode_ret) {
    return nullptr;
}

cl_mem clCreateFromGLTexture3D(cl_context context, cl_mem_flags flags, cl_GLenum target, cl_GLint miplevel,
                               cl_GLuint texture, cl_int *errcode_ret) {
    return nullptr;
}

/***********************************************************************************************************************
* cl_khr_gl_sharing extension
***********************************************************************************************************************/

cl_int clGetGLContextInfoKHR(const cl_context_properties *properties, cl_gl_context_info param_name,
                             size_t param_value_size, void *param_value, size_t *param_value_size_ret) {
    return CL_INVALID_OPERATION;
}

/***********************************************************************************************************************
* cl_khr_gl_event extension
***********************************************************************************************************************/

cl_event clCreateEventFromGLsyncKHR(cl_context context, cl_GLsync sync, cl_int *errcode_ret) {
    return nullptr;
}
