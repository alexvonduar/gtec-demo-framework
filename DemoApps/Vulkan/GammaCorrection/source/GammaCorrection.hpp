#ifndef VULKAN_GAMMACORRECTION_GAMMACORRECTION_HPP
#define VULKAN_GAMMACORRECTION_GAMMACORRECTION_HPP
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

#include <FslBase/Transition/TransitionCache.hpp>
#include <FslBase/Transition/TransitionValue.hpp>
#include <FslDemoApp/Vulkan/Basic/DemoAppVulkanBasic.hpp>
#include <FslDemoApp/Base/Service/Keyboard/IKeyboard.hpp>
#include <FslDemoApp/Base/Service/Mouse/IMouse.hpp>
#include <FslGraphics3D/Camera/FirstPersonCamera.hpp>
#include <FslSimpleUI/App/UIDemoAppExtension.hpp>
#include <FslSimpleUI/Base/Control/Label.hpp>
#include <FslUtil/Vulkan1_0/Managed/VMBufferManager.hpp>
#include <FslUtil/Vulkan1_0/Managed/VMVertexBuffer.hpp>
#include <FslUtil/Vulkan1_0/VUTexture.hpp>
#include <RapidVulkan/DescriptorSetLayout.hpp>
#include <RapidVulkan/DescriptorPool.hpp>
#include <RapidVulkan/GraphicsPipeline.hpp>
#include <RapidVulkan/ShaderModule.hpp>
#include <RapidVulkan/PipelineLayout.hpp>
#include <vector>

namespace Fsl
{
  class GammaCorrection
    : public VulkanBasic::DemoAppVulkanBasic
    , public UI::EventListener
  {
  public:
    struct SimpleMesh
    {
      Vulkan::VMVertexBuffer VertexBuffer;
      std::array<VkVertexInputAttributeDescription, 3> VertexAttributeDescription{};
      VkVertexInputBindingDescription VertexInputBindingDescription{};
    };

  private:
    enum class State
    {
      Split4,
      Scene1,
      Scene2,
      Scene3,
      Scene4
    };

    //! Resources that are duplicated per command buffer to ensure that it wont be 'in-use' while we update it
    struct FrameResources
    {
      Vulkan::VUBufferMemory VertUboBuffer;
      Vulkan::VUBufferMemory FragUboBuffer;
      VkDescriptorSet DescriptorSetLinear;
      VkDescriptorSet DescriptorSetSRGB;
    };

    struct VertexUboData
    {
      Matrix MatModelView;
      Matrix MatProj;
    };

    struct FragmentUboData
    {
      Vector4 LightPositions[4];
      Vector4 LightColors[4];
      Vector4 ViewPos;
      bool Gamma = false;
    };

    struct Resources
    {
      Vulkan::VUTexture TexLinear;
      Vulkan::VUTexture TexSRGB;

      RapidVulkan::ShaderModule VertShaderModule;
      RapidVulkan::ShaderModule FragShaderModule;
      RapidVulkan::ShaderModule FragShaderModuleGamma;

      SimpleMesh Mesh;

      RapidVulkan::DescriptorSetLayout MainDescriptorSetLayout;
      RapidVulkan::DescriptorPool MainDescriptorPool;
      std::vector<FrameResources> MainFrameResources;
      RapidVulkan::PipelineLayout MainPipelineLayout;
    };

    struct DependentResources
    {
      RapidVulkan::RenderPass MainRenderPass;
      RapidVulkan::GraphicsPipeline PipelineNoGamma;
      RapidVulkan::GraphicsPipeline PipelineGamma;
    };

    std::shared_ptr<Vulkan::VMBufferManager> m_bufferManager;

    // The UI event listener is responsible for forwarding events to this classes implementation of the UI::EventListener (while its still alive).
    UI::CallbackEventListenerScope m_uiEventListener;
    // The UIDemoAppExtension is a simple extension that sets up the basic UI framework and listens for the events it needs.
    std::shared_ptr<UIDemoAppExtension> m_uiExtension;

    std::shared_ptr<IKeyboard> m_keyboard;
    std::shared_ptr<IMouse> m_mouse;
    std::shared_ptr<IDemoAppControl> m_demoAppControl;
    bool m_mouseCaptureEnabled;
    Graphics3D::FirstPersonCamera m_camera;

    Resources m_resources;
    DependentResources m_dependentResources;
    VertexUboData m_vertexUboData;
    FragmentUboData m_fragmentUboData;

    State m_state;
    TransitionCache m_transitionCache;
    TransitionValue m_splitX;
    TransitionValue m_splitY;
    TransitionValue m_scene1LabelAlpha;
    TransitionValue m_scene2LabelAlpha;
    TransitionValue m_scene3LabelAlpha;
    TransitionValue m_scene4LabelAlpha;

    std::shared_ptr<UI::Label> m_labelTopLeft;
    std::shared_ptr<UI::Label> m_labelTopRight;
    std::shared_ptr<UI::Label> m_labelBottomLeft;
    std::shared_ptr<UI::Label> m_labelBottomRight;

  public:
    GammaCorrection(const DemoAppConfig& config);

  protected:
    void OnKeyEvent(const KeyEvent& event) override;
    void OnMouseButtonEvent(const MouseButtonEvent& event) override;
    void Update(const DemoTime& demoTime) override;
    void VulkanDraw(const DemoTime& demoTime, RapidVulkan::CommandBuffers& rCmdBuffers, const VulkanBasic::DrawContext& drawContext) override;

    VkRenderPass OnBuildResources(const VulkanBasic::BuildResourcesContext& context) override;
    void OnFreeResources() override;

  private:
    void UpdateInput(const DemoTime& demoTime);
    void UpdateSceneTransition(const DemoTime& demoTime);
    void DrawScenes(const FrameResources& frame, const VkCommandBuffer commandBuffer);
    void PrepareTransition();
    void PrepareLights();
    void CreateTextures(const std::shared_ptr<IContentManager>& contentManager, const IO::Path& textureFile);
    void CreateVertexArray();
    void CreateUI();
  };
}

#endif
