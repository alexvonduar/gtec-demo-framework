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

// Make sure Common.hpp is the first include file (to make the error message as helpful as possible when disabled)
#include <FslUtil/Vulkan1_0/Common.hpp>
#include <FslUtil/Vulkan1_0/NativeTexture2D.hpp>
#include <FslUtil/Vulkan1_0/Draft/VulkanImageCreator.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Log/Log.hpp>
#include <cassert>

namespace Fsl
{
  namespace Vulkan
  {
    namespace
    {
      inline VkSamplerCreateInfo Convert(const Texture2DFilterHint filterHint)
      {
        const VkFilter filter = (filterHint == Texture2DFilterHint::Nearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR);

        VkSamplerCreateInfo samplerCreateInfo{};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        // samplerCreateInfo.flags;
        samplerCreateInfo.magFilter = filter;
        samplerCreateInfo.minFilter = filter;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.anisotropyEnable = VK_FALSE;
        samplerCreateInfo.maxAnisotropy = 1.0f;
        samplerCreateInfo.compareEnable = VK_FALSE;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 0.0f;
        // FIX: allow mip maps
        // samplerCreateInfo.maxLod = (useStaging) ? static_cast<float>(m_texture.MipLevels) : 0.0f;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        // samplerCreateInfo.unnormalizedCoordinates;
        return samplerCreateInfo;
      }

      void CheckTextureFlags(const TextureFlags& textureFlags)
      {
        // FIX: implement texture flags support
        FSLLOG_WARNING_IF(textureFlags.Value != TextureFlags::NotDefined, "Vulkan::NativeTexture2D texture flags not supported yet")
      }
    }


    NativeTexture2D::NativeTexture2D(const std::shared_ptr<VulkanImageCreator>& imageCreator, const RawBitmap& bitmap,
                                     const Texture2DFilterHint filterHint, const TextureFlags& textureFlags)
      : m_imageCreator(imageCreator)
    {
      if (!imageCreator)
      {
        throw std::invalid_argument("imageCreator can not be null");
      }
      CheckTextureFlags(textureFlags);
      auto samplerCreateInfo = Convert(filterHint);
      m_impl = m_imageCreator->CreateTexture(bitmap, samplerCreateInfo);
    }


    NativeTexture2D::NativeTexture2D(const std::shared_ptr<VulkanImageCreator>& imageCreator, const RawTexture& texture,
                                     const Texture2DFilterHint filterHint, const TextureFlags& textureFlags)
      : m_imageCreator(imageCreator)
    {
      if (!imageCreator)
      {
        throw std::invalid_argument("imageCreator can not be null");
      }
      CheckTextureFlags(textureFlags);
      auto samplerCreateInfo = Convert(filterHint);
      m_impl = m_imageCreator->CreateTexture(texture, samplerCreateInfo);
    }


    NativeTexture2D::~NativeTexture2D() = default;


    void NativeTexture2D::SetData(const RawBitmap& bitmap, const Texture2DFilterHint filterHint, const TextureFlags& textureFlags)
    {
      CheckTextureFlags(textureFlags);

      // FIX: update the content of the texture instead of creating a new one
      m_impl.Reset();
      auto samplerCreateInfo = Convert(filterHint);
      m_impl = m_imageCreator->CreateTexture(bitmap, samplerCreateInfo);
    }


    void NativeTexture2D::SetData(const RawTexture& texture, const Texture2DFilterHint filterHint, const TextureFlags& textureFlags)
    {
      CheckTextureFlags(textureFlags);

      // FIX: update the content of the texture instead of creating a new one
      m_impl.Reset();
      auto samplerCreateInfo = Convert(filterHint);
      m_impl = m_imageCreator->CreateTexture(texture, samplerCreateInfo);
    }
  }
}
