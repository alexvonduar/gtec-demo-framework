#ifndef FSLGRAPHICS_SPRITE_INFO_CORE_COREIMAGEINFO_HPP
#define FSLGRAPHICS_SPRITE_INFO_CORE_COREIMAGEINFO_HPP
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

#include <FslBase/Math/Dp/DpExtent.hpp>
#include <FslBase/Math/Pixel/PxExtent2D.hpp>
#include <FslBase/Math/Pixel/PxRectangleU.hpp>
#include <FslBase/Math/Pixel/PxThicknessU.hpp>

namespace Fsl
{
  //! @brief Represents a atlas texture 2d image with no trim
  struct CoreImageInfo
  {
    //! The actual size of the source image in pixels
    PxExtent2D ExtentPx;

    //! The number of trimmed pixels
    PxThicknessU TrimMarginPx;

    //! The trimmed rectangle on the source atlas texture
    PxRectangleU TrimmedRectanglePx;

    //! The image density extent (this is the dp size of the image)
    DpExtent ExtentDp;

    constexpr CoreImageInfo() = default;
    constexpr CoreImageInfo(const PxExtent2D extentPx, const PxThicknessU& trimMarginPx, const PxRectangleU& trimmedRectanglePx,
                            const DpExtent extentDp)
      : ExtentPx(extentPx)
      , TrimMarginPx(trimMarginPx)
      , TrimmedRectanglePx(trimmedRectanglePx)
      , ExtentDp(extentDp)

    {
    }

    constexpr bool operator==(const CoreImageInfo& rhs) const
    {
      return ExtentPx == rhs.ExtentPx && TrimMarginPx == rhs.TrimMarginPx && TrimmedRectanglePx == rhs.TrimmedRectanglePx && ExtentDp == rhs.ExtentDp;
    }

    constexpr bool operator!=(const CoreImageInfo& rhs) const
    {
      return !(*this == rhs);
    }
  };
}

#endif
