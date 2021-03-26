#ifndef VULKAN_SHADERCLOCK_SHADERCLOCK_HPP
#define VULKAN_SHADERCLOCK_SHADERCLOCK_HPP
/****************************************************************************************************************************************************
 * Copyright 2020 NXP
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

#include <FslDemoApp/Vulkan/Basic/DemoAppVulkanBasic.hpp>
#include <FslSimpleUI/App/UIDemoAppExtension.hpp>
#include <FslSimpleUI/Base/Control/LabelNineSliceButton.hpp>
#include <FslSimpleUI/Base/Control/SliderAndFmtValueLabel.hpp>
#include <FslSimpleUI/Base/Control/Switch.hpp>
#include <FslUtil/Vulkan1_0/Managed/VMBufferManager.hpp>
#include <RapidVulkan/DescriptorPool.hpp>
#include "FractalShaderMandelbrot.hpp"
#include "DefaultValues.hpp"


namespace Fsl
{
  class ShaderClock final
    : public VulkanBasic::DemoAppVulkanBasic
    , public UI::EventListener
  {
    struct Resources
    {
      std::shared_ptr<Vulkan::VMBufferManager> BufferManager;
      RapidVulkan::DescriptorPool DescriptorPool;
    };

    struct DependentResources
    {
      RapidVulkan::RenderPass MainRenderPass;
    };


    struct UIRecord
    {
      std::shared_ptr<UI::SliderAndFmtValueLabel<uint32_t>> SliderHeatmap;
      std::shared_ptr<UI::Switch> SwitchHeatmap;
      std::shared_ptr<UI::SliderAndFmtValueLabel<uint16_t>> SliderIterations;
      std::shared_ptr<UI::LabelNineSliceButton> BtnDefault;

      UIRecord() = default;
      UIRecord(std::shared_ptr<UI::SliderAndFmtValueLabel<uint32_t>> sliderHeatmap, std::shared_ptr<UI::Switch> switchHeatmap,
               std::shared_ptr<UI::SliderAndFmtValueLabel<uint16_t>> sliderIterations, std::shared_ptr<UI::LabelNineSliceButton> btnDefault)
        : SliderHeatmap(std::move(sliderHeatmap))
        , SwitchHeatmap(std::move(switchHeatmap))
        , SliderIterations(std::move(sliderIterations))
        , BtnDefault(std::move(btnDefault))
      {
      }
    };

    bool m_heatmapSupport;

    // The UI event listener is responsible for forwarding events to this classes implementation of the UI::EventListener (while its still alive).
    UI::CallbackEventListenerScope m_uiEventListener;
    // The UIDemoAppExtension is a simple extension that sets up the basic UI framework and listens for the events it needs.
    std::shared_ptr<UIDemoAppExtension> m_uiExtension;

    uint32_t m_defaultHeatmapScale{DefaultValues::HeatmapScale};
    uint16_t m_defaultIterations{DefaultValues::MandelbrotDefaultIterations};
    UIRecord m_ui;

    Resources m_resources;
    DependentResources m_dependentResources;

    FractalShaderMandelbrot m_scene;

  public:
    explicit ShaderClock(const DemoAppConfig& config);

  protected:
    void OnKeyEvent(const KeyEvent& event) final;
    void OnContentChanged(const UI::RoutedEventArgs& args, const std::shared_ptr<UI::WindowContentChangedEvent>& theEvent) final;
    void OnSelect(const UI::RoutedEventArgs& args, const std::shared_ptr<UI::WindowSelectEvent>& theEvent) final;
    void Update(const DemoTime& demoTime) override;
    void VulkanDraw(const DemoTime& demoTime, RapidVulkan::CommandBuffers& rCmdBuffers, const VulkanBasic::DrawContext& drawContext) override;

    VkRenderPass OnBuildResources(const VulkanBasic::BuildResourcesContext& context) override;
    void OnFreeResources() override;

  private:
    static Resources CreateResources(const Vulkan::VUDevice& device, const Vulkan::VUDeviceQueueRecord& deviceQueue,
                                     const VulkanBasic::RenderConfig& renderConfig);

    UIRecord CreateUI(const bool heatmapSupported, const uint32_t heatmapScale, const uint16_t iterations);

    void SetDefaultSettings();
  };
}

#endif
