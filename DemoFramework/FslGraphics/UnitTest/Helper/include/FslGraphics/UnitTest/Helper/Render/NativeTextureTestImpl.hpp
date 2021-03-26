#ifndef FSLGRAPHICS_UNITTEST_HELPER_RENDER_NATIVETEXTURETESTIMPL_HPP
#define FSLGRAPHICS_UNITTEST_HELPER_RENDER_NATIVETEXTURETESTIMPL_HPP
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

#include <FslBase/BasicTypes.hpp>
#include <FslBase/Math/Pixel/PxRectangleU.hpp>
#include <FslGraphics/Bitmap/RawBitmap.hpp>
#include <FslGraphics/Render/Adapter/INativeTexture2D.hpp>
#include <FslGraphics/Render/Texture2DFilterHint.hpp>
#include <FslGraphics/Texture/RawTexture.hpp>
#include <FslGraphics/TextureFlags.hpp>
#include <cassert>
#include <vector>

namespace Fsl
{
  class NativeTextureTestImpl : public INativeTexture2D
  {
    std::vector<uint8_t> m_content;
    PxExtent3D m_extentPx;

  public:
    NativeTextureTestImpl(const RawTexture& texture, const Texture2DFilterHint filterHint, const TextureFlags textureFlags)
      : m_extentPx(texture.GetExtent())
    {
      FSL_PARAM_NOT_USED(filterHint);
      FSL_PARAM_NOT_USED(textureFlags);

      m_content.clear();
      const auto* const pSrc = static_cast<const uint8_t*>(texture.GetContent());
      assert(pSrc != nullptr);
      m_content.insert(m_content.begin(), pSrc, pSrc + texture.GetByteSize());
    }

    NativeTextureArea CalcNativeTextureArea(const PxRectangleU& imageRectanglePx) const override
    {
      assert(static_cast<float>(m_extentPx.Width) >= 0.0f);
      assert(static_cast<float>(m_extentPx.Height) >= 0.0f);
      return {imageRectanglePx.Left() == 0 ? 0.0f : imageRectanglePx.Left() / static_cast<float>(m_extentPx.Width),
              imageRectanglePx.Top() == 0 ? 0.0f : imageRectanglePx.Top() / static_cast<float>(m_extentPx.Height),
              imageRectanglePx.Right() == m_extentPx.Width ? 1.0f : imageRectanglePx.Right() / static_cast<float>(m_extentPx.Width),
              imageRectanglePx.Bottom() == m_extentPx.Height ? 1.0f : imageRectanglePx.Bottom() / static_cast<float>(m_extentPx.Height)};
    }
  };
}

#endif
