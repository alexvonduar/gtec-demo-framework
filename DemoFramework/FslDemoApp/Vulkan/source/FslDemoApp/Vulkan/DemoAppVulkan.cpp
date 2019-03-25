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

#include <FslDemoApp/Vulkan/DemoAppVulkan.hpp>
#include <FslBase/Log/Log.hpp>
#include <FslBase/Log/Math/LogExtent2D.hpp>
#include <FslDemoApp/Base/Service/Host/IHostInfo.hpp>
#include <FslDemoHost/Vulkan/Config/DemoAppHostConfigVulkan.hpp>
#include <FslDemoHost/Vulkan/Config/Service/IVulkanHostInfo.hpp>
#include <FslDemoHost/Vulkan/Config/PhysicalDeviceFeatureRequest.hpp>
#include <FslDemoHost/Vulkan/Config/PhysicalDeviceFeatureUtil.hpp>
#include <FslDemoHost/Vulkan/Config/PhysicalDeviceFeatureRequestUtil.hpp>
#include <FslDemoHost/Vulkan/Config/VulkanDeviceSetupUtil.hpp>
#include <FslDemoService/NativeGraphics/Vulkan/NativeGraphicsService.hpp>
#include <FslUtil/Vulkan1_0/Log/All.hpp>
#include <FslUtil/Vulkan1_0/Util/ConvertUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/DeviceUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/PhysicalDeviceKHRUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/QueueUtil.hpp>
#include <array>

namespace Fsl
{
  using namespace Vulkan;

  namespace
  {
    std::string ToString(const Extent2D& extent)
    {
      std::stringstream stream;
      stream << extent;
      return stream.str();
    }

    std::string ToString(const VkExtent2D& extent)
    {
      std::stringstream stream;
      stream << extent;
      return stream.str();
    }
  }

  DemoAppVulkan::DemoAppVulkan(const DemoAppConfig& demoAppConfig)
    : ADemoApp(demoAppConfig)
    , m_nativeGraphicsService(demoAppConfig.DemoServiceProvider.Get<Vulkan::NativeGraphicsService>())
  {
    // FIX: move most of this init code to the Vulkan demo host
    auto hostInfo = demoAppConfig.DemoServiceProvider.Get<IHostInfo>();
    auto appHostConfigBase = hostInfo->TryGetAppHostConfig();
    if (!appHostConfigBase)
    {
      throw std::runtime_error("The AppHostConfig was not set");
    }
    auto appHostConfig = std::dynamic_pointer_cast<DemoAppHostConfigVulkan>(appHostConfigBase);
    if (!appHostConfig)
    {
      throw std::runtime_error("The AppHostConfig was not of the expected type");
    }

    auto vulkanHostInfo = demoAppConfig.DemoServiceProvider.Get<IVulkanHostInfo>();
    // Retrieve the launch options
    m_launchOptions = vulkanHostInfo->GetLaunchOptions();

    m_surface = vulkanHostInfo->GetSurfaceKHR();

    std::deque<Vulkan::PhysicalDeviceFeatureRequest> requiredFeatures;
    if (appHostConfig && appHostConfig->HasDeviceRequiredFeatures())
    {
      appHostConfig->ExtractDeviceRequiredFeatures(requiredFeatures);
    }

    m_physicalDevice = vulkanHostInfo->GetPhysicalDevice();
    {
      auto vulkanDeviceSetup = VulkanDeviceSetupUtil::CreateSetup(m_physicalDevice, m_surface, requiredFeatures);
      m_deviceActiveFeatures = vulkanDeviceSetup.DeviceFeatures;
      m_device = std::move(vulkanDeviceSetup.Device);
      m_deviceCreateInfo = vulkanDeviceSetup.DeviceCreateInfo;
      m_deviceQueue = vulkanDeviceSetup.DeviceQueueRecord;
    }

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    RAPIDVULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice.Device, m_surface, &surfaceCapabilities));
    if (ConvertUtil::Convert(surfaceCapabilities.currentExtent) != GetScreenExtent())
    {
      throw InitFailedException("The Vulkan surface extent did not match the window extent. Screen: " + ToString(GetScreenExtent()) +
                                " vs surface: " + ToString(surfaceCapabilities.currentExtent));
    }

    // We do this last to ensure that we dont have to call VulkanDeviceShutdown as nothing else can go wrong
    if (m_nativeGraphicsService)
    {
      m_nativeGraphicsService->VulkanDeviceInit(m_device, m_deviceQueue.Queue, m_deviceQueue.QueueFamilyIndex);
    }
  }


  DemoAppVulkan::~DemoAppVulkan()
  {
    SafeShutdown();
  }

  void DemoAppVulkan::OnDestroy()
  {
    SafeShutdown();
    // Finally call the OnDestroy of our inherited object
    ADemoApp::OnDestroy();
  }


  void DemoAppVulkan::SafeWaitForDeviceIdle() noexcept
  {
    try
    {
      m_device.DeviceWaitIdle();
    }
    catch (const std::exception& ex)
    {
      // We log and swallow it since destructor's are not allowed to throw
      FSLLOG_ERROR("DeviceWaitIdle, threw exception: " << ex.what());
    }
  }

  void DemoAppVulkan::SafeShutdown()
  {
    SafeWaitForDeviceIdle();
    if (m_nativeGraphicsService)
    {
      m_nativeGraphicsService->VulkanDeviceShutdown();
      m_nativeGraphicsService.reset();
    }
  }

}
