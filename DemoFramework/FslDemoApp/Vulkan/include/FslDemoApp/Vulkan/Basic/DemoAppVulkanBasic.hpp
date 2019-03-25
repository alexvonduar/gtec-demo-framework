#ifndef FSLDEMOAPP_VULKAN_BASIC_DEMOAPPVULKANBASIC_HPP
#define FSLDEMOAPP_VULKAN_BASIC_DEMOAPPVULKANBASIC_HPP
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

#include <FslDemoApp/Vulkan/DemoAppVulkan.hpp>
#include <FslDemoApp/Vulkan/Basic/BuildResourcesContext.hpp>
#include <FslDemoApp/Vulkan/Basic/DemoAppVulkanSetup.hpp>
#include <FslDemoApp/Vulkan/Basic/DrawContext.hpp>
#include <FslDemoApp/Vulkan/Basic/FrameBufferCreateContext.hpp>
#include <FslDemoApp/Vulkan/Basic/RenderConfig.hpp>
#include <FslDemoApp/Vulkan/Basic/SwapchainInfo.hpp>
#include <FslUtil/Vulkan1_0/VUSwapchainKHR.hpp>
#include <FslUtil/Vulkan1_0/VUImageMemoryView.hpp>
#include <RapidVulkan/CommandBuffers.hpp>
#include <RapidVulkan/CommandPool.hpp>
#include <RapidVulkan/Fence.hpp>
#include <RapidVulkan/Framebuffer.hpp>
#include <RapidVulkan/ImageView.hpp>
#include <RapidVulkan/RenderPass.hpp>
#include <RapidVulkan/Semaphore.hpp>
#include <vulkan/vulkan.h>
#include <cassert>
#include <memory>
#include <vector>

namespace Fsl
{
  class DemoAppProfilerOverlay;

  namespace VulkanBasic
  {
    class DemoAppVulkanBasic : public DemoAppVulkan
    {
    public:
      //! The default maximum number of frames currently being rendered
      static constexpr uint32_t GetDefaultMaxFramesInFlight()
      {
        return 2;
      }

    private:
      enum class AppState
      {
        // The app is ready to render
        Ready,
        // We are waiting for the swapchain to be re-creatable.
        // This can occur on windows when the app is minimized and the vkGetPhysicalDeviceSurfaceCapabilitiesKHR call can
        // return a surface with a max extent of zero.
        WaitForSwapchainRecreation
      };

      enum class RecreateSwapchainResult
      {
        Failed,
        NotReady,
        Completed    // Named Complete because we can't use Success as the X11 header might define it :(
      };

      struct FrameDrawRecord
      {
        RapidVulkan::Semaphore ImageAcquiredSemaphore;
        RapidVulkan::Semaphore RenderingCompleteSemaphore;
        RapidVulkan::Fence InFlightFence;
      };

      struct SwapchainRecord
      {
        RapidVulkan::ImageView SwapchainImageView;
        RapidVulkan::Framebuffer Framebuffer;
      };

      struct Resources
      {
        RapidVulkan::CommandPool MainCommandPool;
        std::vector<FrameDrawRecord> Frames;
        uint32_t CurrentFrame = 0;
      };

      struct DependentResources
      {
        bool Valid = false;
        uint32_t MaxFramesInFlight = 0;
        //! A optional Depth image, only enabled if requested
        //! We only need one RenderAttachment as command buffers on the same queue are executed in order
        Vulkan::VUImageMemoryView DepthImage;
        RapidVulkan::CommandBuffers CmdBuffers;
        uint32_t CurrentSwapBufferIndex = 0;
        std::vector<SwapchainRecord> SwapchainRecords;
      };

      DemoAppVulkanSetup m_appSetup;
      Resources m_resources;
      Vulkan::VUSwapchainKHR m_swapchain;
      DependentResources m_dependentResources;

      AppState m_currentAppState = AppState::Ready;
      std::unique_ptr<DemoAppProfilerOverlay> m_demoAppProfilerOverlay;

    protected:
      DemoAppVulkanBasic(const DemoAppConfig& demoAppConfig, const DemoAppVulkanSetup& demoAppVulkanSetup = {});
      ~DemoAppVulkanBasic() override;
      void OnConstructed() override;
      void OnDestroy() override;

      //! @brief Get information about the 'basic' rendering configuration
      //!        This can be used to to the initial setup before BuildResources.
      RenderConfig GetRenderConfig() const;

      AppDrawResult TryPrepareDraw(const DemoTime& demoTime) override;

    public:
      void _Draw(const DemoTime& demoTime) override;

    protected:
      void Resized(const Point2& size) override;
      void Draw(const DemoTime& demoTime) override;
      AppDrawResult TrySwapBuffers(const DemoTime& demoTime) override;

      void AddSystemUI(VkCommandBuffer hCmdBuffer, const uint32_t cmdBufferIndex);

      void BuildResources();
      void FreeResources();

      //! Check if the dependent resources are currently allocated
      bool IsResourcesAllocated() const
      {
        return m_dependentResources.Valid;
      }

      VkCommandPool GetCommandPool() const
      {
        return m_resources.MainCommandPool.Get();
      }

      bool TryRebuildResources();

      //! @brief Called after the swapchain has been created
      //! @return Must return the main RenderPass.
      virtual VkRenderPass OnBuildResources(const BuildResourcesContext& context) = 0;

      //! @brief Called after the swapchain ImageView and FrameBuffers has been deleted
      virtual void OnFreeResources()
      {
      }

      virtual void VulkanDraw(const DemoTime& demoTime, RapidVulkan::CommandBuffers& rCmdBuffers, const DrawContext& drawContext) = 0;

      //! @brief get the swapchain image count
      uint32_t GetSwapchainImageCount() const
      {
        if (!m_swapchain.IsValid() || m_currentAppState != AppState::Ready)
        {
          return 0;
        }
        return m_swapchain.GetImageCount();
      }

      bool TryGetSwapchainInfo(SwapchainInfo& rSwapchainInfo) const;

      //! @brief Call this to create a very basic render pass. This will only be useful for the most basic of applications and
      //         to quickly get something on the screen to get started.
      RapidVulkan::RenderPass CreateBasicRenderPass();

      virtual RapidVulkan::Framebuffer CreateFramebuffer(const FrameBufferCreateContext& frameBufferCreateContext);

      //! @brief Call this to crate a 'basic' depth image view.
      //         The created depth image view will be in the VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL layout.
      static Vulkan::VUImageMemoryView CreateBasicDepthImageView(const Vulkan::VUDevice& device, const VkExtent2D& swapchainImageExtent,
                                                                 const VkCommandPool commandPool);

    private:
      static std::vector<FrameDrawRecord> CreateFrameSyncObjects(const VkDevice device, const uint32_t maxFramesInFlight);
      void BuildSwapchainImageView(SwapchainRecord& rSwapchainRecord, const uint32_t swapBufferIndex);

      RecreateSwapchainResult TryRecreateSwapchain();

      AppDrawResult TryDoPrepareDraw(const DemoTime& demoTime);
      AppDrawResult TryDoSwapBuffers(const DemoTime& demoTime);
      void SetAppState(AppDrawResult result);
    };
  }
}

#endif
