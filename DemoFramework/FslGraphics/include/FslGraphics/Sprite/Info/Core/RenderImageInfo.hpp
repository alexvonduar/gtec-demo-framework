#ifndef FSLGRAPHICS_SPRITE_INFO_CORE_RENDERIMAGEINFO_HPP
#define FSLGRAPHICS_SPRITE_INFO_CORE_RENDERIMAGEINFO_HPP
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

#include <FslBase/Math/Pixel/PxTrimmedImage.hpp>
#include <FslGraphics/NativeTextureArea.hpp>

namespace Fsl
{
  //! @brief Represents a atlas texture 2d image with no trim
  struct RenderImageInfo
  {
    //! The UV coordinates on the source atlas texture in native texture coordinates
    NativeTextureArea TextureArea;

    // ---- Values below can be modified due to 'resize'

    //! The scaled image size in pixels
    PxSize2D ScaledSizePx;

    //! The scaled image trim in pixels
    PxThicknessF ScaledTrimMarginPxf;

    //! The scaled trimmed image size in pixels
    PxSize2DF ScaledTrimmedSizePxf;

    constexpr RenderImageInfo() = default;
    constexpr RenderImageInfo(const NativeTextureArea& imageNativeTextureArea, const PxTrimmedImage& trimmedImagePx)
      : TextureArea(imageNativeTextureArea)
      , ScaledSizePx(trimmedImagePx.SizePx)
      , ScaledTrimMarginPxf(trimmedImagePx.TrimMarginPxf)
      , ScaledTrimmedSizePxf(trimmedImagePx.TrimmedSizePxf)
    {
    }

    constexpr bool operator==(const RenderImageInfo& rhs) const
    {
      return TextureArea == rhs.TextureArea && ScaledSizePx == rhs.ScaledSizePx && ScaledTrimMarginPxf == rhs.ScaledTrimMarginPxf &&
             ScaledTrimmedSizePxf == rhs.ScaledTrimmedSizePxf;
    }

    constexpr bool operator!=(const RenderImageInfo& rhs) const
    {
      return !(*this == rhs);
    }
  };
}

#endif
