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

#ifndef CLMTL_PLATFORM_H
#define CLMTL_PLATFORM_H

#include <string>
#include <CL/cl_icd.h>

struct _cl_platform_id {
    cl_icd_dispatch *Dispatch;
};

namespace clmtl {

class Platform : public _cl_platform_id {
public:
    static Platform *GetSingleton();
    static std::string GetProfile();
    static std::string GetVersion();
    static std::string GetName();
    static std::string GetVendor();
    static std::string GetExtensions();
    static std::string GetSuffix();

private:
    Platform();
};

} //namespace clmtl

#endif //CLMTL_PLATFORM_H
