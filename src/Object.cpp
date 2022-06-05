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

#include "Object.h"

#include <cassert>

namespace clmtl {

Object::~Object() {
    assert(!mReferenceCount);
}

void Object::Retain() {
    mReferenceCount.fetch_add(1);
}

void Object::Release() {
    mReferenceCount.fetch_sub(1);
}

uint64_t Object::GetReferenceCount() const {
    return mReferenceCount;
}

Object::Object()
        : mReferenceCount{1} {
}

} //namespace clmtl
