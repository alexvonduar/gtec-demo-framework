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

#include <FslBase/Exceptions.hpp>
#include <FslBase/Log/Log.hpp>
#include <FslDemoApp/Shared/Host/DemoHostFeatureUtil.hpp>
#include <FslDemoHost/Base/Service/WindowHost/IWindowHostInfoControl.hpp>
#include <FslDemoHost/Vulkan/Config/InstanceConfigUtil.hpp>
#include <FslDemoHost/Vulkan/VulkanDemoHost.hpp>
#include <FslDemoHost/Vulkan/VulkanDemoHostOptionParser.hpp>
#include <FslDemoService/NativeGraphics/Vulkan/NativeGraphicsService.hpp>
#include <FslNativeWindow/Vulkan/VulkanNativeWindowSystemFactory.hpp>
#include <FslNativeWindow/Vulkan/NativeVulkanSetup.hpp>
#include <FslNativeWindow/Vulkan/IVulkanNativeWindowSystem.hpp>
#include <FslNativeWindow/Vulkan/IVulkanNativeWindow.hpp>
#include <FslNativeWindow/Base/INativeWindowEventQueue.hpp>
#include <FslNativeWindow/Base/NativeWindowEventHelper.hpp>
#include <FslNativeWindow/Base/NativeWindowSystemSetup.hpp>
#include <FslNativeWindow/Base/NativeWindowProcessMessagesArgs.hpp>
#include <FslUtil/Vulkan1_0/Log/All.hpp>
#include <FslUtil/Vulkan1_0/Util/InstanceUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/PhysicalDeviceUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/PhysicalDeviceKHRUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/SwapchainKHRUtil.hpp>
#include <RapidVulkan/Debug/Strings/VkFormat.hpp>
#include <RapidVulkan/Debug/Strings/VkPhysicalDeviceType.hpp>
//#include <RapidVulkan/Debug/Strings/VkColorSpaceKHR.hpp>
#include <RapidVulkan/Check.hpp>
#include <RapidVulkan/Semaphore.hpp>
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include "Service/VulkanHost/VulkanHostService.hpp"
// Included last as a workaround
#include <FslUtil/Vulkan1_0/Debug/BitFlags.hpp>

#if 0
#define LOCAL_LOG(X) FSLLOG("VulkanDemoHost: " << X)
#else
#define LOCAL_LOG(X) \
  {                  \
  }
#endif

#if 1
#define LOCAL_LOG_WARNING(X) FSLLOG_WARNING("VulkanDemoHost: " << X)
#else
#define LOCAL_LOG_WARNING(X) \
  {                          \
  }
#endif

using namespace RapidVulkan;

namespace Fsl
{
  using namespace Vulkan;

  namespace
  {
    void LogExtensions()
    {
      auto extensionProperties = InstanceUtil::EnumerateInstanceExtensionProperties(nullptr);
      FSLLOG("Core extensions: " << extensionProperties.size());
      for (const auto& extension : extensionProperties)
      {
        FSLLOG("- Extension: '" << extension.extensionName << "' specVersion: " << extension.specVersion);
      }
    }

    void LogLayers()
    {
      auto layerProperties = InstanceUtil::EnumerateInstanceLayerProperties();
      FSLLOG("Instance layer properties: " << layerProperties.size());
      for (const auto& layer : layerProperties)
      {
        FSLLOG("- layer: '" << layer.layerName << "' specVersion: " << EncodedVulkanVersion(layer.specVersion)
                            << " implementationVersion: " << layer.implementationVersion << " description: '" << layer.description << "'");
        auto extensionProperties = InstanceUtil::EnumerateInstanceExtensionProperties(layer.layerName);
        if (!extensionProperties.empty())
        {
          for (const auto& prop : extensionProperties)
          {
            FSLLOG("  - Extension: '" << prop.extensionName << "' specVersion: " << prop.specVersion);
          }
        }
      }
    }

    void LogSurfaceFormats(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface)
    {
      const auto surfaceFormats = PhysicalDeviceKHRUtil::GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface);
      FSLLOG("Supported surface formats: " << surfaceFormats.size());
      for (auto& entry : surfaceFormats)
      {
        FSLLOG("- Format: " << entry.format << " (" << RapidVulkan::Debug::ToString(entry.format) << "), ColorSpace: " << entry.colorSpace);
        //<< " (" << RapidVulkan::Debug::ToString(entry.colorSpace) << ")");
      }
    }
  }


  VulkanDemoHost::VulkanDemoHost(const DemoHostConfig& demoHostConfig)
    : ADemoHost(demoHostConfig)
    , m_options(demoHostConfig.GetOptions<VulkanDemoHostOptionParser>())
    , m_nativeGraphicsService(demoHostConfig.GetServiceProvider().Get<Vulkan::NativeGraphicsService>())
    , m_windowHostInfoControl(demoHostConfig.GetServiceProvider().Get<IWindowHostInfoControl>())
    , m_demoHostConfig(demoHostConfig)
    , m_activeApi(DemoHostFeatureName::Vulkan, 0)
  {
    if (m_options->IsLogExtensionsEnabled())
    {
      LogExtensions();
    }
    if (m_options->IsLogLayersEnabled())
    {
      LogLayers();
    }

    const NativeWindowSystemSetup nativeWindowSystemSetup(demoHostConfig.GetEventQueue(), demoHostConfig.GetVerbosityLevel(),
                                                          m_options->GetNativeWindowConfig(), m_options->GetNativeWindowTag());

    m_windowSystem = VulkanNativeWindowSystemFactory::Allocate(nativeWindowSystemSetup);
    try
    {
      // Set the window system in the host service so that any services or app that is interested will be able to access it
      m_windowHostInfoControl->SetWindowSystem(m_windowSystem);

      const DemoHostAppSetup& hostAppSetup = demoHostConfig.GetDemoHostAppSetup();

      if (hostAppSetup.DemoHostFeatures->empty())
      {
        throw NotSupportedException("Must use at least one feature");
      }

      m_activeApi = hostAppSetup.DemoHostFeatures->front();

      m_nativeWindowSetup.reset(new NativeWindowSetup(demoHostConfig.GetDemoHostAppSetup().AppSetup.ApplicationName, demoHostConfig.GetEventQueue(),
                                                      m_options->GetNativeWindowConfig(), demoHostConfig.GetVerbosityLevel()));

      Init();
    }
    catch (const std::exception&)
    {
      m_windowHostInfoControl->ClearWindowSystem();
      throw;
    }
  }


  VulkanDemoHost::~VulkanDemoHost()
  {
    Shutdown();
  }


  void VulkanDemoHost::OnActivate()
  {
    m_isActivated = true;
  }


  void VulkanDemoHost::OnDeactivate()
  {
    m_isActivated = false;
  }


  void VulkanDemoHost::OnSuspend()
  {
    Shutdown();
  }


  void VulkanDemoHost::OnResume()
  {
    Init();
  }


  DemoHostFeature VulkanDemoHost::GetActiveAPI() const
  {
    return m_activeApi;
  }


  Point2 VulkanDemoHost::GetScreenResolution() const
  {
    Point2 size;
    if (m_window && m_window->TryGetSize(size))
    {
      return size;
    }
    return Point2(0, 0);
  }


  SwapBuffersResult VulkanDemoHost::TrySwapBuffers()
  {
    if (!m_isActivated)
    {
      return SwapBuffersResult::Completed;
    }
    // We rely on the DemoApp doing the actual swap
    return SwapBuffersResult::AppControlled;
  }


  bool VulkanDemoHost::ProcessNativeMessages(const bool allowBlock)
  {
    // This will be called while suspended
    // Allow the native window to process messages
    NativeWindowProcessMessagesArgs args(allowBlock);
    return m_windowSystem->ProcessMessages(args);
  }


  void VulkanDemoHost::Init()
  {
    LOCAL_LOG("Init");
    try
    {
      // Get the basic Vulkan configuration ready
      InitVulkan();

      const NativeVulkanSetup nativeVulkanSetup(m_instance.Get(), m_physicalDevice.Device);
      m_window = m_windowSystem->CreateNativeWindow(*m_nativeWindowSetup, nativeVulkanSetup);
      m_windowHostInfoControl->AddWindow(m_window);


      m_vulkanHostService = m_demoHostConfig.GetServiceProvider().Get<VulkanHostService>();
      m_vulkanHostService->SetLaunchOptions(m_options->GetLaunchOptions());
      m_vulkanHostService->SetInstance(m_instance.Get());
      m_vulkanHostService->SetPhysicalDevice(m_physicalDevice);
      auto surface = m_window->GetVulkanSurface();
      m_vulkanHostService->SetSurfaceKHR(surface);

      if (m_options->IsLogSurfaceFormatsEnabled())
      {
        LogSurfaceFormats(m_physicalDevice.Device, surface);
      }

      if (m_nativeGraphicsService)
      {
        m_nativeGraphicsService->VulkanInit();
      }
    }
    catch (const std::exception& ex)
    {
      FSLLOG_ERROR("Init failed with exception: " << ex.what());
      Shutdown();
      throw;
    }
  }


  void VulkanDemoHost::Shutdown()
  {
    LOCAL_LOG("Shutdown");

    if (m_vulkanHostService)
    {
      m_vulkanHostService->Clear();
    }
    if (m_nativeGraphicsService)
    {
      m_nativeGraphicsService->VulkanShutdown();
    }

    m_windowHostInfoControl->RemoveWindow(m_window);
    m_window.reset();
    ShutdownVulkan();
  }


  void VulkanDemoHost::InitVulkan()
  {
    {    // Init the Vulkan instance
      const auto userChoiceValidationLayer = m_options->GetValidationLayerChoice();
      const auto userChoiceApiDump = m_options->GetApiDumpChoice();
      const auto khrSurfaceExtensionName = m_windowSystem->GetKHRSurfaceExtensionName();
      const auto& applicationName = m_demoHostConfig.GetDemoHostAppSetup().AppSetup.ApplicationName;

      const auto demoHostConfig = m_demoHostConfig.GetDemoHostAppSetup().GetDemoAppHostConfig<DemoAppHostConfigVulkan>();

      InstanceConfigUtil::InstanceUserChoice instanceUserChoice(userChoiceValidationLayer, userChoiceApiDump);

      const auto instanceConfig = InstanceConfigUtil::InstanceConfigAsCharArrays(
        InstanceConfigUtil::BuildInstanceConfig(khrSurfaceExtensionName, instanceUserChoice, demoHostConfig));

      m_instance = InstanceUtil::CreateInstance(applicationName, VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0, 0, instanceConfig.Layers,
                                                instanceConfig.Extensions, m_instanceCreateInfo.get());
    }

    // Select the physical device
    const auto physicialDeviceIndex = m_options->GetPhysicalDeviceIndex();
    m_physicalDevice = VUPhysicalDeviceRecord(InstanceUtil::GetPhysicalDevice(m_instance.Get(), physicialDeviceIndex));

    if (Fsl::Logger::GetLogLevel() >= LogType::Verbose2)
    {
      FSLLOG("Vulkan physical device #" << physicialDeviceIndex << " properties:");
      FSLLOG("- apiVersion: " << EncodedVulkanVersion(m_physicalDevice.Properties.apiVersion));
      FSLLOG("- driverVersion: " << EncodedVulkanVersion(m_physicalDevice.Properties.driverVersion));
      FSLLOG("- vendorID: " << m_physicalDevice.Properties.vendorID);
      FSLLOG("- deviceID: " << m_physicalDevice.Properties.deviceID);
      FSLLOG("- deviceType: " << Debug::GetBitflagsString(m_physicalDevice.Properties.deviceType));    // VkPhysicalDeviceType
      FSLLOG("- deviceName: " << m_physicalDevice.Properties.deviceName);
    }
  }


  void VulkanDemoHost::ShutdownVulkan()
  {
    m_physicalDevice.Reset();
    m_instance.Reset();
  }
}
