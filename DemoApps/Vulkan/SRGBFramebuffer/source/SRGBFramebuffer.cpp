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

#include "SRGBFramebuffer.hpp"
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslBase/Math/MathHelper.hpp>
#include <FslBase/Math/Pixel/TypeConverter_Math.hpp>
#include <FslBase/UncheckedNumericCast.hpp>
#include <FslGraphics/Vertices/VertexPositionNormalTexture.hpp>
#include <FslSimpleUI/Base/Control/BackgroundNineSlice.hpp>
#include <FslSimpleUI/Base/IWindowManager.hpp>
#include <FslSimpleUI/Base/Layout/ComplexStackLayout.hpp>
#include <FslSimpleUI/Base/Layout/FillLayout.hpp>
#include <FslSimpleUI/Base/Layout/StackLayout.hpp>
#include <FslSimpleUI/Base/WindowContext.hpp>
#include <FslSimpleUI/Theme/Basic/BasicThemeFactory.hpp>
#include <FslUtil/Vulkan1_0/Draft/VulkanImageCreator.hpp>
#include <FslUtil/Vulkan1_0/Exceptions.hpp>
#include <FslUtil/Vulkan1_0/Util/MatrixUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/VMVertexBufferUtil.hpp>
#include <RapidVulkan/Check.hpp>
#include <vulkan/vulkan.h>
#include <array>

namespace Fsl
{
  namespace
  {
    const auto VERTEX_BUFFER_BIND_ID = 0;

    const Vector3 DEFAULT_CAMERA_POSITION(4.0f, 1.0f, 7.0f);
    const Vector3 DEFAULT_CAMERA_TARGET(0.0f, 0.0f, 0.0f);

    IO::Path GetTextureFile(const VkPhysicalDeviceFeatures& features)
    {
      if (features.textureCompressionASTC_LDR != VK_FALSE)
      {
        FSLLOG3_INFO("Using ASTC texture");
        return IO::Path("Textures/WoodFloor/Floor_ASTC8x8_rgb.ktx");
      }
      if (features.textureCompressionETC2 != VK_FALSE)
      {
        FSLLOG3_INFO("Using ETC2 texture");
        return IO::Path("Textures/WoodFloor/Floor_ETC2_RGB.ktx");
      }
      if (features.textureCompressionBC != VK_FALSE)
      {
        FSLLOG3_INFO("Using BC1 texture");
        return IO::Path("Textures/WoodFloor/Floor_BC1_rgb.ktx");
      }
      throw NotSupportedException("No supported texture compression found");
    }

    Vulkan::VUTexture CreateTexture(Vulkan::VulkanImageCreator& imageCreator, const Texture& texture)
    {
      VkSamplerCreateInfo samplerCreateInfo{};
      samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
      samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerCreateInfo.mipLodBias = 0.0f;
      samplerCreateInfo.anisotropyEnable = VK_FALSE;
      samplerCreateInfo.maxAnisotropy = 1.0f;
      samplerCreateInfo.compareEnable = VK_FALSE;
      samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
      samplerCreateInfo.minLod = 0.0f;
      samplerCreateInfo.maxLod = static_cast<float>(texture.GetLevels());
      samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

      return imageCreator.CreateTexture(texture, samplerCreateInfo);
    }

    RapidVulkan::DescriptorSetLayout CreateDescriptorSetLayout(const Vulkan::VUDevice& device)
    {
      std::array<VkDescriptorSetLayoutBinding, 3> setLayoutBindings{};
      // Binding 0 : Vertex shader uniform buffer
      setLayoutBindings[0].binding = 0;
      setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      setLayoutBindings[0].descriptorCount = 1;
      setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

      // Binding 1 : Fragment shader image sampler
      setLayoutBindings[1].binding = 1;
      setLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      setLayoutBindings[1].descriptorCount = 1;
      setLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

      // Binding 2 : Vertex shader uniform buffer
      setLayoutBindings[2].binding = 2;
      setLayoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      setLayoutBindings[2].descriptorCount = 1;
      setLayoutBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

      VkDescriptorSetLayoutCreateInfo descriptorLayout{};
      descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      descriptorLayout.bindingCount = UncheckedNumericCast<uint32_t>(setLayoutBindings.size());
      descriptorLayout.pBindings = setLayoutBindings.data();

      return RapidVulkan::DescriptorSetLayout(device.Get(), descriptorLayout);
    }


    RapidVulkan::DescriptorPool CreateDescriptorPool(const Vulkan::VUDevice& device, const uint32_t count)
    {
      // Example uses one ubo and one image sampler
      std::array<VkDescriptorPoolSize, 2> poolSizes{};
      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = count * 2;
      poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSizes[1].descriptorCount = count;

      VkDescriptorPoolCreateInfo descriptorPoolInfo{};
      descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      descriptorPoolInfo.maxSets = count;
      descriptorPoolInfo.poolSizeCount = UncheckedNumericCast<uint32_t>(poolSizes.size());
      descriptorPoolInfo.pPoolSizes = poolSizes.data();

      return RapidVulkan::DescriptorPool(device.Get(), descriptorPoolInfo);
    }


    Vulkan::VUBufferMemory CreateUBO(const Vulkan::VUDevice& device, const VkDeviceSize size)
    {
      VkBufferCreateInfo bufferCreateInfo{};
      bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferCreateInfo.size = size;
      bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
      bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      Vulkan::VUBufferMemory buffer(device, bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      // Keep the buffer mapped as we update it each frame
      buffer.Map();
      return buffer;
    }


    VkDescriptorSet CreateDescriptorSet(const RapidVulkan::DescriptorPool& descriptorPool,
                                        const RapidVulkan::DescriptorSetLayout& descriptorSetLayout)
    {
      assert(descriptorPool.IsValid());
      assert(descriptorSetLayout.IsValid());

      // Allocate a new descriptor set from the global descriptor pool
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool.Get();
      allocInfo.descriptorSetCount = 1;
      allocInfo.pSetLayouts = descriptorSetLayout.GetPointer();

      VkDescriptorSet descriptorSet = nullptr;
      RapidVulkan::CheckError(vkAllocateDescriptorSets(descriptorPool.GetDevice(), &allocInfo, &descriptorSet), "vkAllocateDescriptorSets", __FILE__,
                              __LINE__);

      return descriptorSet;
    }


    VkDescriptorSet UpdateDescriptorSet(const VkDevice device, const VkDescriptorSet descriptorSet, const Vulkan::VUBufferMemory& vertUboBuffer,
                                        const Vulkan::VUBufferMemory& fragUboBuffer, const Vulkan::VUTexture& texture)

    {
      assert(descriptorSet != nullptr);
      assert(vertUboBuffer.IsValid());
      assert(fragUboBuffer.IsValid());
      assert(texture.IsValid());

      std::array<VkWriteDescriptorSet, 3> writeDescriptorSets{};
      // Binding 0 : Vertex shader uniform buffer
      auto vertUboBufferInfo = vertUboBuffer.GetDescriptorBufferInfo();
      writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeDescriptorSets[0].dstSet = descriptorSet;
      writeDescriptorSets[0].dstBinding = 0;
      writeDescriptorSets[0].descriptorCount = 1;
      writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      writeDescriptorSets[0].pBufferInfo = &vertUboBufferInfo;

      // Binding 1 : Fragment shader texture sampler
      auto textureImageInfo = texture.GetDescriptorImageInfo();
      writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeDescriptorSets[1].dstSet = descriptorSet;
      writeDescriptorSets[1].dstBinding = 1;
      writeDescriptorSets[1].descriptorCount = 1;
      writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      writeDescriptorSets[1].pImageInfo = &textureImageInfo;

      // Binding 2 : Fragment shader uniform buffer
      auto fragUboBufferInfo = fragUboBuffer.GetDescriptorBufferInfo();
      writeDescriptorSets[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeDescriptorSets[2].dstSet = descriptorSet;
      writeDescriptorSets[2].dstBinding = 2;
      writeDescriptorSets[2].descriptorCount = 1;
      writeDescriptorSets[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      writeDescriptorSets[2].pBufferInfo = &fragUboBufferInfo;

      vkUpdateDescriptorSets(device, UncheckedNumericCast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

      return descriptorSet;
    }


    RapidVulkan::PipelineLayout CreatePipelineLayout(const RapidVulkan::DescriptorSetLayout& descripterSetLayout)
    {
      VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
      pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutCreateInfo.setLayoutCount = 1;
      pipelineLayoutCreateInfo.pSetLayouts = descripterSetLayout.GetPointer();

      return RapidVulkan::PipelineLayout(descripterSetLayout.GetDevice(), pipelineLayoutCreateInfo);
    }


    RapidVulkan::GraphicsPipeline CreatePipeline(const RapidVulkan::PipelineLayout& pipelineLayout, const VkExtent2D& extent,
                                                 const VkShaderModule vertexShaderModule, const VkShaderModule fragmentShaderModule,
                                                 const SRGBFramebuffer::SimpleMesh& mesh, const VkRenderPass renderPass, const uint32_t subpass)
    {
      assert(pipelineLayout.IsValid());
      assert(vertexShaderModule != VK_NULL_HANDLE);
      assert(fragmentShaderModule != VK_NULL_HANDLE);
      assert(renderPass != VK_NULL_HANDLE);
      assert(!mesh.VertexAttributeDescription.empty());

      std::array<VkPipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfo{};
      pipelineShaderStageCreateInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      pipelineShaderStageCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
      pipelineShaderStageCreateInfo[0].module = vertexShaderModule;
      pipelineShaderStageCreateInfo[0].pName = "main";
      pipelineShaderStageCreateInfo[0].pSpecializationInfo = nullptr;

      pipelineShaderStageCreateInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      pipelineShaderStageCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      pipelineShaderStageCreateInfo[1].module = fragmentShaderModule;
      pipelineShaderStageCreateInfo[1].pName = "main";
      pipelineShaderStageCreateInfo[1].pSpecializationInfo = nullptr;

      VkPipelineVertexInputStateCreateInfo pipelineVertexInputCreateInfo{};
      pipelineVertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      pipelineVertexInputCreateInfo.vertexBindingDescriptionCount = 1;
      pipelineVertexInputCreateInfo.pVertexBindingDescriptions = &mesh.VertexInputBindingDescription;
      pipelineVertexInputCreateInfo.vertexAttributeDescriptionCount = UncheckedNumericCast<uint32_t>(mesh.VertexAttributeDescription.size());
      pipelineVertexInputCreateInfo.pVertexAttributeDescriptions = mesh.VertexAttributeDescription.data();

      VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
      pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
      pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

      VkViewport viewport{};
      viewport.width = static_cast<float>(extent.width);
      viewport.height = static_cast<float>(extent.height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;

      VkRect2D scissor{{0, 0}, extent};

      VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
      pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      pipelineViewportStateCreateInfo.viewportCount = 1;
      pipelineViewportStateCreateInfo.pViewports = &viewport;
      pipelineViewportStateCreateInfo.scissorCount = 1;
      pipelineViewportStateCreateInfo.pScissors = &scissor;

      VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
      pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
      pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
      pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
      pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
      pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
      pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
      pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
      pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
      pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
      pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

      VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
      pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
      pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
      pipelineMultisampleStateCreateInfo.minSampleShading = 0.0f;
      pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
      pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
      pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

      VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState{};
      pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
      pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
      pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
      pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
      pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
      pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
      pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
      pipelineColorBlendAttachmentState.colorWriteMask = 0xf;

      VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
      pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
      pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
      pipelineColorBlendStateCreateInfo.attachmentCount = 1;
      pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
      pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
      pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
      pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
      pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;

      VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
      depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
      depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
      depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
      depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
      depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
      depthStencilStateCreateInfo.front = {};
      depthStencilStateCreateInfo.back = {};
      depthStencilStateCreateInfo.minDepthBounds = 0.0f;
      depthStencilStateCreateInfo.maxDepthBounds = 1.0f;

      std::array<VkDynamicState, 1> dynamicState{VK_DYNAMIC_STATE_VIEWPORT};

      VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
      pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
      pipelineDynamicStateCreateInfo.dynamicStateCount = UncheckedNumericCast<uint32_t>(dynamicState.size());
      pipelineDynamicStateCreateInfo.pDynamicStates = dynamicState.data();

      VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
      graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      graphicsPipelineCreateInfo.stageCount = UncheckedNumericCast<uint32_t>(pipelineShaderStageCreateInfo.size());
      graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfo.data();
      graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputCreateInfo;
      graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
      graphicsPipelineCreateInfo.pTessellationState = nullptr;
      graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
      graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
      graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
      graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
      graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
      graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
      graphicsPipelineCreateInfo.layout = pipelineLayout.Get();
      graphicsPipelineCreateInfo.renderPass = renderPass;
      graphicsPipelineCreateInfo.subpass = subpass;
      graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
      graphicsPipelineCreateInfo.basePipelineIndex = 0;

      return RapidVulkan::GraphicsPipeline(pipelineLayout.GetDevice(), VK_NULL_HANDLE, graphicsPipelineCreateInfo);
    }


    VulkanBasic::DemoAppVulkanSetup CreateSetup()
    {
      VulkanBasic::DemoAppVulkanSetup setup;
      return setup;
    }
  }


  SRGBFramebuffer::SRGBFramebuffer(const DemoAppConfig& config)
    : VulkanBasic::DemoAppVulkanBasic(config, CreateSetup())
    , m_bufferManager(
        std::make_shared<Vulkan::VMBufferManager>(m_physicalDevice, m_device.Get(), m_deviceQueue.Queue, m_deviceQueue.QueueFamilyIndex))
    , m_uiEventListener(this)    // The UI listener forwards call to 'this' object
    , m_uiExtension(
        std::make_shared<UIDemoAppExtension>(config, m_uiEventListener.GetListener(), "UIAtlas/UIAtlas_160dpi"))    // Prepare the extension
    , m_keyboard(config.DemoServiceProvider.Get<IKeyboard>())
    , m_mouse(config.DemoServiceProvider.Get<IMouse>())
    , m_demoAppControl(config.DemoServiceProvider.Get<IDemoAppControl>())
    , m_mouseCaptureEnabled(false)
    , m_state(State::Split2)
    , m_splitX(m_transitionCache, TransitionTimeSpan(400, TransitionTimeUnit::Milliseconds), TransitionType::Smooth)
    , m_splitSceneWidthL(m_transitionCache, TransitionTimeSpan(400, TransitionTimeUnit::Milliseconds), TransitionType::Smooth)
    , m_splitSceneWidthR(m_transitionCache, TransitionTimeSpan(400, TransitionTimeUnit::Milliseconds), TransitionType::Smooth)
    , m_splitSceneAlphaL(m_transitionCache, TransitionTimeSpan(200, TransitionTimeUnit::Milliseconds), TransitionType::Smooth)
    , m_splitSceneAlphaR(m_transitionCache, TransitionTimeSpan(200, TransitionTimeUnit::Milliseconds), TransitionType::Smooth)
  {
    const auto chosenSurfaceFormat = GetSurfaceFormatInfo();
    bool hasSRGBFramebuffer = (chosenSurfaceFormat.Format == VK_FORMAT_B8G8R8A8_SRGB);

    m_camera.SetPosition(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_TARGET, Vector3::Up());

    const auto contentManager = GetContentManager();

    PrepareTransition();
    PrepareLights();
    CreateUI(hasSRGBFramebuffer);

    const auto textureFile = GetTextureFile(m_physicalDevice.Features);
    CreateTextures(contentManager, textureFile);
    CreateVertexArray();

    const auto* shaderFilename = hasSRGBFramebuffer ? "GammaCorrection.frag.spv" : "GammaCorrectionGamma.frag.spv";

    m_resources.VertShaderModule.Reset(m_device.Get(), 0, contentManager->ReadBytes("GammaCorrection.vert.spv"));
    m_resources.FragShaderModule.Reset(m_device.Get(), 0, contentManager->ReadBytes(shaderFilename));

    m_resources.MainDescriptorSetLayout = CreateDescriptorSetLayout(m_device);
    const uint32_t maxFramesInFlight = GetRenderConfig().MaxFramesInFlight;
    m_resources.MainDescriptorPool = CreateDescriptorPool(m_device, maxFramesInFlight * 2);

    m_resources.MainFrameResources.resize(maxFramesInFlight);
    for (auto& rFrame : m_resources.MainFrameResources)
    {
      rFrame.VertUboBufferL = CreateUBO(m_device, sizeof(VertexUboData));
      rFrame.VertUboBufferR = CreateUBO(m_device, sizeof(VertexUboData));
      rFrame.FragUboBuffer = CreateUBO(m_device, sizeof(FragmentUboData));
      rFrame.DescriptorSetLinear = CreateDescriptorSet(m_resources.MainDescriptorPool, m_resources.MainDescriptorSetLayout);
      rFrame.DescriptorSetSRGB = CreateDescriptorSet(m_resources.MainDescriptorPool, m_resources.MainDescriptorSetLayout);
      UpdateDescriptorSet(m_device.Get(), rFrame.DescriptorSetLinear, rFrame.VertUboBufferL, rFrame.FragUboBuffer, m_resources.TexLinear);
      UpdateDescriptorSet(m_device.Get(), rFrame.DescriptorSetSRGB, rFrame.VertUboBufferR, rFrame.FragUboBuffer, m_resources.TexSRGB);
    }
    m_resources.MainPipelineLayout = CreatePipelineLayout(m_resources.MainDescriptorSetLayout);

    UpdateUIToState();
    UpdateSceneTransition(DemoTime());
    m_splitX.ForceComplete();
    m_splitSceneWidthL.ForceComplete();
    m_splitSceneWidthR.ForceComplete();
    m_splitSceneAlphaL.ForceComplete();
    m_splitSceneAlphaR.ForceComplete();
  }

  void SRGBFramebuffer::OnContentChanged(const UI::RoutedEventArgs& /*args*/, const std::shared_ptr<UI::WindowContentChangedEvent>& theEvent)
  {
    if (theEvent->GetSource() == m_leftCB || theEvent->GetSource() == m_rightCB)
    {
      const bool isCheckedL = m_leftCB->IsChecked();
      const bool isCheckedR = m_rightCB->IsChecked();
      if (isCheckedL)
      {
        if (isCheckedR)
        {
          SetState(State::Split2);
        }
        else
        {
          SetState(State::Scene1);
        }
      }
      else if (isCheckedR)
      {
        SetState(State::Scene2);
      }
      else
      {
        // If none is checked we force update the UI
        UpdateUIToState();
      }
    }
  }

  void SRGBFramebuffer::OnKeyEvent(const KeyEvent& event)
  {
    if (event.IsHandled() || !event.IsPressed())
    {
      return;
    }

    switch (event.GetKey())
    {
    case VirtualKey::Code1:
      SetState(State::Scene1);
      break;
    case VirtualKey::Code2:
      SetState(State::Scene2);
      break;
    case VirtualKey::Code3:
    case VirtualKey::Space:
      SetState(State::Split2);
      break;
    default:
      break;
    }
  }


  void SRGBFramebuffer::OnMouseButtonEvent(const MouseButtonEvent& event)
  {
    if (event.IsHandled())
    {
      return;
    }

    switch (event.GetButton())
    {
    case VirtualMouseButton::Right:
    {
      const bool mouseCapture = event.IsPressed();
      if (m_demoAppControl->TryEnableMouseCaptureMode(mouseCapture))
      {
        m_mouseCaptureEnabled = mouseCapture;
      }
      else
      {
        m_mouseCaptureEnabled = false;
      }
      event.Handled();
      break;
    }
    case VirtualMouseButton::Middle:
      if (event.IsPressed())
      {
        m_camera.SetPosition(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_TARGET, Vector3::Up());
        event.Handled();
      }
      break;
    default:
      break;
    }
  }


  void SRGBFramebuffer::Update(const DemoTime& demoTime)
  {
    UpdateInput(demoTime);
    UpdateSceneTransition(demoTime);

    const auto windowSizePx = GetWindowSizePx();
    auto matrixWorld = Matrix::GetIdentity();
    auto matrixView = m_camera.GetViewMatrix();

    // Deal with the new Vulkan coordinate system (see method description for more info).
    // Consider using: https://github.com/KhronosGroup/Vulkan-Docs/blob/master/appendices/VK_KHR_maintenance1.txt
    const auto vulkanClipMatrix = Vulkan::MatrixUtil::GetClipMatrix();

    const float aspectL = m_splitSceneWidthL.GetValue() / windowSizePx.Height();

    m_vertexUboDataL.MatProj = Matrix::CreatePerspectiveFieldOfView(MathHelper::ToRadians(45.0f), aspectL, 0.1f, 100.0f) * vulkanClipMatrix;
    m_vertexUboDataL.MatModelView = matrixWorld * matrixView;

    const float aspectR = m_splitSceneWidthR.GetValue() / windowSizePx.Height();
    m_vertexUboDataR.MatProj = Matrix::CreatePerspectiveFieldOfView(MathHelper::ToRadians(45.0f), aspectR, 0.1f, 100.0f) * vulkanClipMatrix;
    m_vertexUboDataR.MatModelView = matrixWorld * matrixView;

    auto cameraPos = m_camera.GetPosition();
    m_fragmentUboData.ViewPos = Vector4(cameraPos.X, cameraPos.Y, cameraPos.Z, 1.0f);
  }


  void SRGBFramebuffer::VulkanDraw(const DemoTime& /*demoTime*/, RapidVulkan::CommandBuffers& rCmdBuffers,
                                   const VulkanBasic::DrawContext& drawContext)
  {
    const uint32_t frameIndex = drawContext.CurrentFrameIndex;
    const uint32_t currentSwapBufferIndex = drawContext.CurrentSwapBufferIndex;

    // Upload the changes
    m_resources.MainFrameResources[frameIndex].VertUboBufferL.Upload(0, &m_vertexUboDataL, sizeof(VertexUboData));
    m_resources.MainFrameResources[frameIndex].VertUboBufferR.Upload(0, &m_vertexUboDataR, sizeof(VertexUboData));
    m_resources.MainFrameResources[frameIndex].FragUboBuffer.Upload(0, &m_fragmentUboData, sizeof(FragmentUboData));

    const VkCommandBuffer hCmdBuffer = rCmdBuffers[currentSwapBufferIndex];
    rCmdBuffers.Begin(currentSwapBufferIndex, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, VK_FALSE, 0, 0);
    {
      std::array<VkClearValue, 1> clearValues{};
      clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};

      VkRenderPassBeginInfo renderPassBeginInfo{};
      renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassBeginInfo.renderPass = m_dependentResources.MainRenderPass.Get();
      renderPassBeginInfo.framebuffer = drawContext.Framebuffer;
      renderPassBeginInfo.renderArea.offset.x = 0;
      renderPassBeginInfo.renderArea.offset.y = 0;
      renderPassBeginInfo.renderArea.extent = drawContext.SwapchainImageExtent;
      renderPassBeginInfo.clearValueCount = UncheckedNumericCast<uint32_t>(clearValues.size());
      renderPassBeginInfo.pClearValues = clearValues.data();

      rCmdBuffers.CmdBeginRenderPass(currentSwapBufferIndex, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
      {
        DrawScenes(m_resources.MainFrameResources[frameIndex], hCmdBuffer);

        // Calling this last allows the UI to draw on top of everything.
        // Beware that the UI drawing methods might alter the OpenGL state!
        m_uiExtension->Draw();

        // Remember to call this as the last operation in your renderPass
        AddSystemUI(hCmdBuffer, currentSwapBufferIndex);
      }
      rCmdBuffers.CmdEndRenderPass(currentSwapBufferIndex);
    }
    rCmdBuffers.End(currentSwapBufferIndex);
  }


  VkRenderPass SRGBFramebuffer::OnBuildResources(const VulkanBasic::BuildResourcesContext& context)
  {
    m_dependentResources.MainRenderPass = CreateBasicRenderPass();

    m_dependentResources.Pipeline =
      CreatePipeline(m_resources.MainPipelineLayout, context.SwapchainImageExtent, m_resources.VertShaderModule.Get(),
                     m_resources.FragShaderModule.Get(), m_resources.Mesh, m_dependentResources.MainRenderPass.Get(), 0);


    return m_dependentResources.MainRenderPass.Get();
  }


  void SRGBFramebuffer::OnFreeResources()
  {
    m_dependentResources = {};
  }


  void SRGBFramebuffer::UpdateInput(const DemoTime& demoTime)
  {
    {    // Mouse camera rotation
      const auto mouseState = m_mouse->GetState();

      if (!m_mouseCaptureEnabled)
      {
        const bool rotateCamera = mouseState.IsRightButtonPressed();
        m_camera.RotateViaPosition(rotateCamera, mouseState.Position);
      }
      else
      {
        if (mouseState.IsRightButtonPressed())
        {
          const auto rawPosition = Vector2(mouseState.RawPosition.X, -mouseState.RawPosition.Y);
          m_camera.Rotate(rawPosition);
        }
      }
    }

    {    // Keyboard camera movement
      const float movementSpeed = 2.0f * demoTime.DeltaTime;
      auto keyboardState = m_keyboard->GetState();

      if (keyboardState.IsKeyDown(VirtualKey::W))
      {
        m_camera.MoveForward(movementSpeed);
      }
      if (keyboardState.IsKeyDown(VirtualKey::S))
      {
        m_camera.MoveBackwards(movementSpeed);
      }
      if (keyboardState.IsKeyDown(VirtualKey::A))
      {
        m_camera.MoveLeft(movementSpeed);
      }
      if (keyboardState.IsKeyDown(VirtualKey::D))
      {
        m_camera.MoveRight(movementSpeed);
      }
    }
  }

  void SRGBFramebuffer::UpdateSceneTransition(const DemoTime& demoTime)
  {
    const auto res = TypeConverter::UncheckedTo<Vector2>(GetWindowSizePx());

    switch (m_state)
    {
    case State::Scene1:
      m_splitX.SetValue(res.X);
      m_splitSceneWidthL.SetValue(res.X);
      m_splitSceneWidthR.SetValue(0.0f);
      m_splitSceneAlphaL.SetValue(1.0f);
      m_splitSceneAlphaR.SetValue(0.0f);
      break;
    case State::Scene2:
      m_splitX.SetValue(0.0f);
      m_splitSceneWidthL.SetValue(0.0f);
      m_splitSceneWidthR.SetValue(res.X);
      m_splitSceneAlphaL.SetValue(0.0f);
      m_splitSceneAlphaR.SetValue(1.0f);
      break;
    case State::Split2:
    default:
      m_splitX.SetValue(res.X / 2.0f);
      m_splitSceneWidthL.SetValue(res.X / 2.0f);
      m_splitSceneWidthR.SetValue(res.X / 2.0f);
      m_splitSceneAlphaL.SetValue(1.0f);
      m_splitSceneAlphaR.SetValue(1.0f);
      break;
    }
    m_splitX.Update(TransitionTimeSpan(demoTime.DeltaTimeInMicroseconds, TransitionTimeUnit::Microseconds));
    m_splitSceneWidthL.Update(TransitionTimeSpan(demoTime.DeltaTimeInMicroseconds, TransitionTimeUnit::Microseconds));
    m_splitSceneWidthR.Update(TransitionTimeSpan(demoTime.DeltaTimeInMicroseconds, TransitionTimeUnit::Microseconds));
    m_splitSceneAlphaL.Update(TransitionTimeSpan(demoTime.DeltaTimeInMicroseconds, TransitionTimeUnit::Microseconds));
    m_splitSceneAlphaR.Update(TransitionTimeSpan(demoTime.DeltaTimeInMicroseconds, TransitionTimeUnit::Microseconds));

    const float alphaL = m_splitSceneAlphaL.GetValue();
    const float alphaR = m_splitSceneAlphaR.GetValue();
    m_labelLeft->SetFontColor(Color(alphaL, alphaL, alphaL, alphaL));
    m_labelRight->SetFontColor(Color(alphaR, alphaR, alphaR, alphaR));
  }


  void SRGBFramebuffer::DrawScenes(const FrameResources& frame, const VkCommandBuffer commandBuffer)
  {
    const auto windowSizePx = GetWindowSizePx();

    const VkDeviceSize offsets = 0;
    vkCmdBindVertexBuffers(commandBuffer, VERTEX_BUFFER_BIND_ID, 1, m_resources.Mesh.VertexBuffer.GetBufferPointer(), &offsets);

    const auto splitX = static_cast<int32_t>(std::round(m_splitX.GetValue()));
    const int32_t remainderX = windowSizePx.Width() - splitX;
    const auto height = static_cast<float>(windowSizePx.Height());

    //// bottom left (no gamma correction, rgb texture)
    // vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_dependentResources.PipelineNoGamma.Get());

    // VkViewport viewport{0.0f, static_cast<float>(splitY), static_cast<float>(splitX), static_cast<float>(remainderY), 0.0f, 1.0f};
    // if (viewport.width > 0 && viewport.height > 0)
    //{
    //  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_resources.MainPipelineLayout.Get(), 0, 1,
    //  &frame.DescriptorSetLinear,
    //                          0, nullptr);

    //  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    //  vkCmdDraw(commandBuffer, m_resources.Mesh.VertexBuffer.GetVertexCount(), 1, 0, 0);
    //}

    //// top left (no gamma correction, srgb texture)
    // bool descriptorBound = false;
    // viewport = {0.0f, 0.0f, static_cast<float>(splitX), static_cast<float>(splitY), 0.0f, 1.0f};
    // if (viewport.width > 0 && viewport.height > 0)
    //{
    //  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_resources.MainPipelineLayout.Get(), 0, 1, &frame.DescriptorSetSRGB,
    //  0,
    //                          nullptr);

    //  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    //  vkCmdDraw(commandBuffer, m_resources.Mesh.VertexBuffer.GetVertexCount(), 1, 0, 0);
    //}

    // Bottom right (gamma correction, srgb texture)
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_dependentResources.Pipeline.Get());
    VkViewport viewport = {0, 0, static_cast<float>(splitX), height, 0.0f, 1.0f};
    if (viewport.width > 0 && viewport.height > 0)
    {
      vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_resources.MainPipelineLayout.Get(), 0, 1, &frame.DescriptorSetLinear,
                              0, nullptr);
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      vkCmdDraw(commandBuffer, m_resources.Mesh.VertexBuffer.GetVertexCount(), 1, 0, 0);
    }

    // top right (gamma correction enabled, rgb texture)

    viewport = {static_cast<float>(splitX), 0.0f, static_cast<float>(remainderX), height, 0.0f, 1.0f};
    if (viewport.width > 0 && viewport.height > 0)
    {
      vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_resources.MainPipelineLayout.Get(), 0, 1, &frame.DescriptorSetSRGB, 0,
                              nullptr);

      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      vkCmdDraw(commandBuffer, m_resources.Mesh.VertexBuffer.GetVertexCount(), 1, 0, 0);
    }
  }


  void SRGBFramebuffer::PrepareTransition()
  {
    // Force set the initial value so we dont begin with a transition
    const auto res = TypeConverter::UncheckedTo<Vector2>(GetWindowSizePx());
    m_splitX.SetActualValue(res.X / 2.0f);
    m_splitSceneWidthL.SetActualValue(res.X / 2.0f);
    m_splitSceneWidthR.SetActualValue(res.X / 2.0f);
    m_splitSceneAlphaL.SetActualValue(1.0f);
    m_splitSceneAlphaL.SetActualValue(1.0f);
  }


  void SRGBFramebuffer::PrepareLights()
  {
    // lighting info
    m_fragmentUboData.LightPositions[0] = Vector4(0.0f, 0.0f, -3.0f, 1.0f);
    m_fragmentUboData.LightPositions[1] = Vector4(0.0f, 0.0f, -1.0f, 1.0f);
    m_fragmentUboData.LightPositions[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
    m_fragmentUboData.LightPositions[3] = Vector4(0.0f, 0.0f, 3.0f, 1.0f);

    m_fragmentUboData.LightColors[0] = Vector4(0.25f, 0.25f, 0.25f, 1.0f);
    m_fragmentUboData.LightColors[1] = Vector4(0.50f, 0.50f, 0.50f, 1.0f);
    m_fragmentUboData.LightColors[2] = Vector4(0.75f, 0.75f, 0.75f, 1.0f);
    m_fragmentUboData.LightColors[3] = Vector4(1.00f, 1.00f, 1.00f, 1.0f);
  }


  void SRGBFramebuffer::CreateTextures(const std::shared_ptr<IContentManager>& contentManager, const IO::Path& textureFile)
  {
    // Beware the KTX reader does not extract the origin
    auto tex = contentManager->ReadTexture(textureFile);
    // Since we discover the format from the loaded texture, we try to switch to a linear representation
    tex.ChangeCompatiblePixelFormatFlags(PixelFormatFlags::NF_UNorm);

    Vulkan::VulkanImageCreator imageCreator(m_device, m_deviceQueue.Queue, m_deviceQueue.QueueFamilyIndex);

    m_resources.TexLinear = CreateTexture(imageCreator, tex);
    // Since we discover the format from the loaded texture, we try to switch to a SRGB representation
    tex.ChangeCompatiblePixelFormatFlags(PixelFormatFlags::NF_Srgb);
    m_resources.TexSRGB = CreateTexture(imageCreator, tex);
  }


  void SRGBFramebuffer::CreateVertexArray()
  {
    // B D
    // |\|
    // A C
    // A = 1.0
    const float x0 = -10.0f;
    const float x1 = 10.0f;
    const float y = -0.5f;
    const float z0 = -10.0f;
    const float z1 = 10.0f;
    const float u0 = 0.0f;
    const float u1 = 10.0f;
    const float v0 = 10.0f;
    const float v1 = 0.0f;
    const Vector3 normal(0.0f, 1.0f, 0.0f);
    std::array<VertexPositionNormalTexture, 6> vertices = {
      VertexPositionNormalTexture(Vector3(x0, y, z0), normal, Vector2(u0, v0)),
      VertexPositionNormalTexture(Vector3(x0, y, z1), normal, Vector2(u0, v1)),
      VertexPositionNormalTexture(Vector3(x1, y, z1), normal, Vector2(u1, v1)),

      VertexPositionNormalTexture(Vector3(x0, y, z0), normal, Vector2(u0, v0)),
      VertexPositionNormalTexture(Vector3(x1, y, z1), normal, Vector2(u1, v1)),
      VertexPositionNormalTexture(Vector3(x1, y, z0), normal, Vector2(u1, v0)),
    };

    std::array<VertexElementUsage, 3> shaderBindOrder = {VertexElementUsage::Position, VertexElementUsage::Normal,
                                                         VertexElementUsage::TextureCoordinate};
    m_resources.Mesh.VertexBuffer.Reset(m_bufferManager, vertices, Vulkan::VMBufferUsage::STATIC);

    Vulkan::VMVertexBufferUtil::FillVertexInputAttributeDescription(m_resources.Mesh.VertexAttributeDescription, shaderBindOrder,
                                                                    m_resources.Mesh.VertexBuffer);
    m_resources.Mesh.VertexInputBindingDescription.binding = 0;
    m_resources.Mesh.VertexInputBindingDescription.stride = m_resources.Mesh.VertexBuffer.GetElementStride();
    m_resources.Mesh.VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  }


  void SRGBFramebuffer::CreateUI(const bool hasSRGBFramebuffer)
  {
    // Give the UI a chance to intercept the various DemoApp events.
    RegisterExtension(m_uiExtension);

    // Next up we prepare the actual UI
    auto context = m_uiExtension->GetContext();

    UI::Theme::BasicThemeFactory factory(context, m_uiExtension->GetSpriteResourceManager(), m_uiExtension->GetDefaultMaterialId());

    // Create a label to write stuff into when a button is pressed

    m_labelLeft = factory.CreateLabel("RGB texture, gamma correction");
    m_labelLeft->SetAlignmentX(UI::ItemAlignment::Near);
    m_labelLeft->SetAlignmentY(UI::ItemAlignment::Near);

    m_labelRight = factory.CreateLabel("SRGB texture, gamma correction (Correct)");
    m_labelRight->SetAlignmentX(UI::ItemAlignment::Far);
    m_labelRight->SetAlignmentY(UI::ItemAlignment::Near);

    auto label1 = factory.CreateLabel(hasSRGBFramebuffer ? "SRGB framebuffer" : "SRGB framebuffer not available. Emulating output using shader");
    label1->SetAlignmentX(UI::ItemAlignment::Center);
    label1->SetAlignmentY(UI::ItemAlignment::Center);

    auto leftCB = factory.CreateSwitch("Incorrect");
    auto rightCB = factory.CreateSwitch("Correct");
    leftCB->SetAlignmentX(UI::ItemAlignment::Center);
    rightCB->SetAlignmentX(UI::ItemAlignment::Center);
    auto controls = std::make_shared<UI::ComplexStackLayout>(context);
    controls->SetAlignmentX(UI::ItemAlignment::Stretch);
    controls->PushLayoutLength(UI::LayoutLength(UI::LayoutUnitType::Star));
    controls->PushLayoutLength(UI::LayoutLength(UI::LayoutUnitType::Auto));
    controls->PushLayoutLength(UI::LayoutLength(UI::LayoutUnitType::Star));
    controls->SetLayoutOrientation(UI::LayoutOrientation::Horizontal);
    controls->AddChild(leftCB);
    controls->AddChild(label1);
    controls->AddChild(rightCB);


    auto bottomBar = factory.CreateBottomBar(controls);

    // Create a 'root' layout we use the recommended fill layout as it will utilize all available space on the screen
    // We then add the 'player' stack to it and the label
    auto fillLayout = std::make_shared<UI::FillLayout>(context);
    fillLayout->AddChild(m_labelLeft);
    fillLayout->AddChild(m_labelRight);
    fillLayout->AddChild(bottomBar);

    m_leftCB = leftCB;
    m_rightCB = rightCB;

    // Finally add everything to the window manager (to ensure its seen)
    m_uiExtension->GetWindowManager()->Add(fillLayout);
  }

  void SRGBFramebuffer::SetState(State state)
  {
    if (state == m_state)
    {
      return;
    }

    m_state = state;
    UpdateUIToState();
  }

  void SRGBFramebuffer::UpdateUIToState()
  {
    switch (m_state)
    {
    case State::Scene1:
      m_leftCB->SetIsChecked(true);
      m_rightCB->SetIsChecked(false);
      break;
    case State::Scene2:
      m_leftCB->SetIsChecked(false);
      m_rightCB->SetIsChecked(true);
      break;
    case State::Split2:
      m_leftCB->SetIsChecked(true);
      m_rightCB->SetIsChecked(true);
      break;
    default:
      FSLLOG3_WARNING("Unsupported");
      break;
    }
  }

}
