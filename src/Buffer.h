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

#ifndef CLMTL_BUFFER_H
#define CLMTL_BUFFER_H

#include "Metal.hpp"
#include "Memory.h"

namespace cml {

class Buffer : public Memory {
public:
    static Buffer *DownCast(cl_mem buffer);

public:
    Buffer(Context *context, cl_mem_flags flags, size_t size);
    Buffer(Context *context, cl_mem_flags flags, const void *data, size_t size);
    Buffer(Buffer *parent, cl_mem_flags flags, const cl_buffer_region *region);
    ~Buffer() override;
    void *Map() override;
    void Unmap() override;
    Buffer *GetParent() const;
    MTL::Heap *GetHeap() const;
    MTL::Buffer *GetBuffer() const;

private:
    Buffer *mParent;
    MTL::Heap *mHeap;
    MTL::Buffer *mBuffer;

    void InitHeap(size_t size);
    void InitHeap();
    void InitBuffer(size_t size, size_t offset);
    void InitData(const void *data, size_t size);
};

} //namespace cml

#endif //CLMTL_BUFFER_H
