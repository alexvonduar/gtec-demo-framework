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

#include <FslGraphics/Sprite/NineSliceSprite.hpp>
#include <FslBase/Math/Pixel/TypeConverter.hpp>
#include <FslGraphics/Sprite/SpriteUnitConverter.hpp>

namespace Fsl
{
  NineSliceSprite::NineSliceSprite(const SpriteMaterialInfo& spriteMaterialInfo, const PxThicknessU& imageTrimMarginPx,
                                   const PxRectangleU& imageTrimmedRectanglePx, const PxThicknessU& nineSlicePx, const PxThicknessU& contentMarginPx,
                                   const uint32_t imageDpi, const StringViewLite& debugName, const uint32_t densityDpi)
    : m_info(spriteMaterialInfo, imageTrimMarginPx, imageTrimmedRectanglePx, nineSlicePx, contentMarginPx, imageDpi, debugName)
  {
    Resize(densityDpi);
  }


  void NineSliceSprite::SetContent(const SpriteMaterialInfo& spriteMaterialInfo, const PxThicknessU& imageTrimMarginPx,
                                   const PxRectangleU& imageTrimmedRectanglePx, const PxThicknessU& nineSlicePx, const PxThicknessU& contentMarginPx,
                                   const uint32_t imageDpi, const StringViewLite& debugName, const uint32_t densityDpi)
  {
    m_info = NineSliceSpriteInfo(spriteMaterialInfo, imageTrimMarginPx, imageTrimmedRectanglePx, nineSlicePx, contentMarginPx, imageDpi, debugName);
    Resize(densityDpi);
  }


  const SpriteMaterialInfo& NineSliceSprite::GetMaterialInfo() const
  {
    return m_info.MaterialInfo;
  }

  void NineSliceSprite::Resize(const uint32_t densityDpi)
  {
    SpriteUnitConverter unitConverter(densityDpi);

    PxTrimmedNineSlice trimmedNineSlice =
      unitConverter.CalcScaledPxTrimmedNineSlice(m_info.ImageInfo.ExtentPx, m_info.ImageInfo.TrimMarginPx, m_info.ImageInfo.TrimmedNineSlicePx,
                                                 m_info.ImageInfo.ContentMarginPx, m_info.ImageDpi);

    m_info.RenderInfo = RenderNineSliceInfo(m_info.RenderInfo.TextureArea, trimmedNineSlice);
  }
}
