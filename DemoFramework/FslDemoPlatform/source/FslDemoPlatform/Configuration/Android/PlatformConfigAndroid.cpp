#if defined(__ANDROID__)
/****************************************************************************************************************************************************
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of the Freescale Semiconductor, Inc. nor the names of
 *      its contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************************************************************************/

//#ifdef FSL_USE_ANDROID_GLCONTEXT
//# include <FslDemoHost/EGL/Android/EGLAndroidDemoHostSetup.hpp>
//  namespace Fsl
//  {
//    typedef EGLAndroidDemoHostSetup PlatformEGLDemoHost;
//  }
//#else
//# include <FslDemoHost/EGL/EGLDemoHostSetup.hpp>
//  namespace Fsl
//  {
//    typedef EGLDemoHostSetup PlatformEGLDemoHost;
//  }
//#endif

#include <FslDemoHost/Base/Service/ServiceGroupName.hpp>
#include <FslDemoHost/Base/Service/ServicePriorityList.hpp>
#include <FslDemoService/CpuStats/Impl/Adapter/Linux/CpuStatsAdapterLinux.hpp>
#include <FslDemoService/CpuStats/Impl/CpuStatsServiceFactory.hpp>
#include <FslService/Impl/ServiceType/Local/ThreadLocalSingletonServiceFactoryTemplate.hpp>
//#include <FslNativeGraphicsGLES2/NativeGraphicsServiceGLES2.hpp>
//#include <FslNativeGraphicsGLES3/NativeGraphicsServiceGLES3.hpp>
#include "ImageLibraryServiceAndroidFactory.hpp"
#include "../PlatformConfig.hpp"

namespace Fsl
{
  void PlatformConfig::Configure(IDemoHostRegistry& registry, ServiceRegistry serviceRegistry, bool& rEnableFirewallRequest)
  {
    rEnableFirewallRequest = true;

    // Use the EGLDemoHost for OpenGLES
    // registry.Register(DemoHostFeatureName::OpenGLES, PlatformEGLDemoHost::Get());


    //#ifdef FSL_ENABLE_GRAPHICS_ES2
    //    serviceRegistry.Register<ThreadLocalSingletonServiceFactoryTemplate<NativeGraphicsServiceGLES2, INativeGraphicsService>
    //    >(ServicePriorityList::NativeGraphicsService());
    //#endif
    //#ifdef FSL_ENABLE_GRAPHICS_ES3
    //    serviceRegistry.Register<ThreadLocalSingletonServiceFactoryTemplate<NativeGraphicsServiceGLES3, INativeGraphicsService>
    //    >(ServicePriorityList::NativeGraphicsService());
    //#endif

    const auto imageServiceGroup = serviceRegistry.GetServiceGroupByName(ServiceGroupName::Image());
    serviceRegistry.Register<ImageLibraryServiceAndroidFactory>(ServicePriorityList::ImageLibraryService(), imageServiceGroup);

    // We disable the per core info which is extracted from /proc/stat which is unavailable to apps on newer android builds
    auto cpuStatsServiceFactory = std::make_shared<CpuStatsServiceFactory>([]() { return std::make_unique<CpuStatsAdapterLinux>(false); });
    serviceRegistry.Register(cpuStatsServiceFactory);
  }
}
#endif
