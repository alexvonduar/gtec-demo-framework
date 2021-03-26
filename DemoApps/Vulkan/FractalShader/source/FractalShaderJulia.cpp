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

#include "FractalShaderJulia.hpp"
#include <FslBase/UncheckedNumericCast.hpp>
#include <FslBase/String/StringUtil.hpp>
#include <FslDemoApp/Base/Service/Content/IContentManager.hpp>
#include <FslDemoApp/Base/DemoTime.hpp>
#include <FslGraphics/Vertices/VertexPositionTexture.hpp>
#include <FslUtil/Vulkan1_0/Util/VMVertexBufferUtil.hpp>
#include <Shared/FractalShader/OptionParser.hpp>
#include <Shared/FractalShader/ShaderUtil.hpp>

namespace Fsl
{
  namespace
  {
    const uint32_t VERTEX_BUFFER_BIND_ID = 0;

    RapidVulkan::DescriptorSetLayout CreateDescriptorSetLayout(const Vulkan::VUDevice& device)
    {
      std::array<VkDescriptorSetLayoutBinding, 0> setLayoutBindings{};

      VkDescriptorSetLayoutCreateInfo descriptorLayout{};
      descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      descriptorLayout.bindingCount = UncheckedNumericCast<uint32_t>(setLayoutBindings.size());
      descriptorLayout.pBindings = setLayoutBindings.data();

      return RapidVulkan::DescriptorSetLayout(device.Get(), descriptorLayout);
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


    RapidVulkan::PipelineLayout CreatePipelineLayout(const RapidVulkan::DescriptorSetLayout& descripterSetLayout, const uint32_t cbPushConstants)
    {
      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      pushConstantRange.size = cbPushConstants;

      VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
      pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutCreateInfo.setLayoutCount = 1;
      pipelineLayoutCreateInfo.pSetLayouts = descripterSetLayout.GetPointer();
      pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
      pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

      return RapidVulkan::PipelineLayout(descripterSetLayout.GetDevice(), pipelineLayoutCreateInfo);
    }

    RapidVulkan::GraphicsPipeline CreatePipeline(const RapidVulkan::PipelineLayout& pipelineLayout, const VkExtent2D& extent,
                                                 const VkShaderModule vertexShaderModule, const VkShaderModule fragmentShaderModule,
                                                 const QuadMesh& mesh, const VkRenderPass renderPass, const uint32_t subpass, const bool cullEnabled)
    {
      assert(pipelineLayout.IsValid());
      assert(vertexShaderModule != VK_NULL_HANDLE);
      assert(fragmentShaderModule != VK_NULL_HANDLE);
      assert(renderPass != VK_NULL_HANDLE);
      assert(!mesh.AttributeDescription.empty());

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
      pipelineVertexInputCreateInfo.pVertexBindingDescriptions = &mesh.BindingDescription;
      pipelineVertexInputCreateInfo.vertexAttributeDescriptionCount = UncheckedNumericCast<uint32_t>(mesh.AttributeDescription.size());
      pipelineVertexInputCreateInfo.pVertexAttributeDescriptions = mesh.AttributeDescription.data();

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
      pipelineRasterizationStateCreateInfo.cullMode = (cullEnabled ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_NONE);
      pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
      graphicsPipelineCreateInfo.pDynamicState = nullptr;
      graphicsPipelineCreateInfo.layout = pipelineLayout.Get();
      graphicsPipelineCreateInfo.renderPass = renderPass;
      graphicsPipelineCreateInfo.subpass = subpass;
      graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
      graphicsPipelineCreateInfo.basePipelineIndex = 0;

      return RapidVulkan::GraphicsPipeline(pipelineLayout.GetDevice(), VK_NULL_HANDLE, graphicsPipelineCreateInfo);
    }
  }

  FractalShaderJulia::FractalShaderJulia(const DemoAppConfig& config, const Vulkan::VUDevice& device,
                                         const Vulkan::VUDeviceQueueRecord& /*deviceQueue*/,
                                         const std::shared_ptr<Vulkan::VMBufferManager>& bufferManager,
                                         const RapidVulkan::DescriptorPool& descriptorPool, const uint32_t /*maxFrames*/)
    : m_screenResolution(config.ScreenResolution)
    , m_config(config.GetOptions<OptionParser>()->GetConfig())
    , m_helper(m_config, m_config.TheAnimationMode, config.DemoServiceProvider)
  {
    const std::shared_ptr<IContentManager> contentManager = config.DemoServiceProvider.Get<IContentManager>();
    {
      if (m_config.ForceUnroll)
      {
        throw NotSupportedException("Force unroll is not supported");
      }

      IO::Path fragmentShaderFile;
      switch (m_config.TheRenderMode)
      {
      // case RenderMode::Tex:
      //  fragmentShaderFile = "Julia_tex.frag";
      //  break;
      case RenderMode::Smooth:
        fragmentShaderFile =
          (m_config.IterationsM == DefaultValues::JULIA_DEFAULT_ITERATIONS ? "Julia_smooth_default.frag.spv" : "Julia_smooth.frag.spv");
        break;
      case RenderMode::Col:
        fragmentShaderFile = (m_config.IterationsM == DefaultValues::JULIA_DEFAULT_ITERATIONS ? "Julia_col_default.frag.spv" : "Julia_col.frag.spv");
        break;
      case RenderMode::Gray:
      default:
        fragmentShaderFile =
          (m_config.IterationsM == DefaultValues::JULIA_DEFAULT_ITERATIONS ? "Julia_gray_default.frag.spv" : "Julia_gray.frag.spv");
        break;
      }

      // auto fragmentShaderFile = ShaderUtil::GetFragmentShader(m_config, contentManager, 3);
      m_resources.FragShader.Reset(device.Get(), 0, contentManager->ReadBytes(fragmentShaderFile));
      m_resources.VertShader.Reset(device.Get(), 0, contentManager->ReadBytes("Shader.vert.spv"));
    }

    const float aspect = (m_screenResolution.Y / static_cast<float>(m_screenResolution.X));
    const float scaleX = 2.5f;
    const float scaleY = scaleX * aspect;
    const float u1 = (-1.0f) * scaleX;
    const float u2 = (1.0f) * scaleX;
    const float v1 = (1.0f) * scaleY;
    const float v2 = (-1.0f) * scaleY;

    std::array<VertexPositionTexture, 4> vertices = {
      VertexPositionTexture(Vector3(-1.0f, 1.0f, 0.0f), Vector2(u1, v2)),
      VertexPositionTexture(Vector3(-1.0f, -1.0f, 0.0f), Vector2(u1, v1)),
      VertexPositionTexture(Vector3(1.0f, 1.0f, 0.0f), Vector2(u2, v2)),
      VertexPositionTexture(Vector3(1.0f, -1.0f, 0.0f), Vector2(u2, v1)),
    };

    std::array<VertexElementUsage, 2> shaderBindOrder = {VertexElementUsage::Position, VertexElementUsage::TextureCoordinate};
    m_resources.Mesh.VertexBuffer.Reset(bufferManager, vertices, Vulkan::VMBufferUsage::STATIC);

    Vulkan::VMVertexBufferUtil::FillVertexInputAttributeDescription(m_resources.Mesh.AttributeDescription, shaderBindOrder,
                                                                    m_resources.Mesh.VertexBuffer);
    m_resources.Mesh.BindingDescription.binding = 0;
    m_resources.Mesh.BindingDescription.stride = m_resources.Mesh.VertexBuffer.GetElementStride();
    m_resources.Mesh.BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    m_resources.SceneDescriptorSetLayout = CreateDescriptorSetLayout(device);
    m_resources.DescriptorSet = CreateDescriptorSet(descriptorPool, m_resources.SceneDescriptorSetLayout);
    m_resources.ScenePipelineLayout = CreatePipelineLayout(m_resources.SceneDescriptorSetLayout, sizeof(float) * 5);
  }


  FractalShaderJulia::~FractalShaderJulia() = default;


  void FractalShaderJulia::OnBuildResources(const VulkanBasic::BuildResourcesContext& context, const VkRenderPass hRenderPass)
  {
    m_dependentResources.Pipeline = CreatePipeline(m_resources.ScenePipelineLayout, context.SwapchainImageExtent, m_resources.VertShader.Get(),
                                                   m_resources.FragShader.Get(), m_resources.Mesh, hRenderPass, 0, false);
  }


  void FractalShaderJulia::OnFreeResources()
  {
    m_dependentResources = {};
  }

  void FractalShaderJulia::Update(const DemoTime& demoTime)
  {
    m_helper.Update(demoTime);

    m_pushConstants.Coordinate = m_helper.GetLocation();
    m_pushConstants.MaxIterations = m_config.IterationsJ;
  }


  void FractalShaderJulia::Draw(const uint32_t /*frameIndex*/, const VkCommandBuffer hCmdBuffer)
  {
    const VkPipelineLayout pipelineLayout = m_resources.ScenePipelineLayout.Get();

    vkCmdPushConstants(hCmdBuffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants), &m_pushConstants);

    vkCmdBindDescriptorSets(hCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_resources.DescriptorSet, 0, nullptr);
    vkCmdBindPipeline(hCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_dependentResources.Pipeline.Get());

    VkDeviceSize offsets = 0;
    vkCmdBindVertexBuffers(hCmdBuffer, VERTEX_BUFFER_BIND_ID, 1, m_resources.Mesh.VertexBuffer.GetBufferPointer(), &offsets);
    vkCmdDraw(hCmdBuffer, m_resources.Mesh.VertexBuffer.GetVertexCount(), 1, 0, 0);

    m_helper.Draw(m_screenResolution);
  }
}
