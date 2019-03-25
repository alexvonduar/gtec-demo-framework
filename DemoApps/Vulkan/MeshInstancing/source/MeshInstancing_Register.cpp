/****************************************************************************************************************************************************
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
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

#include <FslDemoApp/Vulkan/Setup/RegisterDemoApp.hpp>
#include <FslDemoHost/Vulkan/Config/DemoAppHostConfigVulkan.hpp>
#include "MeshInstancing.hpp"
#include "OptionParserEx.hpp"

namespace Fsl
{
  // Configure the demo environment to run this demo app in a Window host environment
  void ConfigureDemoAppEnvironment(HostDemoAppSetup& rSetup)
  {
    using namespace Vulkan;

    DemoAppHostConfigVulkan config;

    // config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::TessellationShader, FeatureRequirement::Mandatory);
    // config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::PipelineStatisticsQuery, FeatureRequirement::Optional);
    // config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::PipelineStatisticsQuery, FeatureRequirement::Mandatory);
    // config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::FillModeNonSolid, FeatureRequirement::Optional);

    config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::TextureCompressionETC2, FeatureRequirement::Optional);
    config.AddPhysicalDeviceFeatureRequest(PhysicalDeviceFeature::TextureCompressionBC, FeatureRequirement::Optional);

    DemoAppRegister::Vulkan::Register<MeshInstancing, OptionParserEx>(rSetup, "Vulkan.MeshInstancing", config);
  }
}
