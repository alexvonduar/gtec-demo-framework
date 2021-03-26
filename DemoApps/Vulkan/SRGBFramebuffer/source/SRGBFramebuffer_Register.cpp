/****************************************************************************************************************************************************
 * Copyright 2018 NXP
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
 *    * Neither the name of the NXP. nor the names of
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

#include <FslDemoApp/Vulkan/Setup/RegisterDemoApp.hpp>
#include "SRGBFramebuffer.hpp"

namespace Fsl
{
  // Configure the demo environment to run this demo app in a Vulkan host environment
  void ConfigureDemoAppEnvironment(HostDemoAppSetup& rSetup)
  {
    using namespace Vulkan;

    CustomDemoAppConfig customDemoAppConfig;
    customDemoAppConfig.RestartFlags = CustomDemoAppConfigRestartFlags::Never;

    DemoAppHostConfigVulkan config;
    config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::TextureCompressionASTC_LDR, FeatureRequirement::Optional);
    config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::TextureCompressionBC, FeatureRequirement::Optional);
    config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::TextureCompressionETC2, FeatureRequirement::Optional);

    // Setup our preferred surface formats (in order of preference)
    config.AddPreferedSurfaceFormat(SurfaceFormatInfo(VK_FORMAT_B8G8R8A8_SRGB));
    config.AddPreferedSurfaceFormat(SurfaceFormatInfo(VK_FORMAT_B8G8R8A8_UNORM));

    DemoAppRegister::Vulkan::Register<SRGBFramebuffer>(rSetup, "Vulkan.SRGBFramebuffer", config, customDemoAppConfig);
  }
}
