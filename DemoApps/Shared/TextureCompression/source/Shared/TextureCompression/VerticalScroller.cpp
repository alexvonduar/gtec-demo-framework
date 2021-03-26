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

#include <Shared/TextureCompression/VerticalScroller.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslBase/Math/EqualHelper.hpp>
#include <FslBase/Math/MathHelper.hpp>
#include <FslBase/Math/Pixel/TypeConverter.hpp>
#include <FslDemoApp/Base/DemoTime.hpp>
#include <FslSimpleUI/Base/BaseWindowContext.hpp>
#include <FslSimpleUI/Base/PropertyTypeFlags.hpp>
#include <algorithm>

namespace Fsl
{
  namespace UI
  {
    VerticalScroller::VerticalScroller(const std::shared_ptr<BaseWindowContext>& context)
      : ContentControl(context)
    {
      Enable(WindowFlags::UpdateEnabled);
    }


    void VerticalScroller::WinUpdate(const DemoTime& demoTime)
    {
      if (!m_animate)
      {
        return;
      }

      m_animationPosition += m_animationSpeed * demoTime.DeltaTime;
      if (m_animationPosition > MathHelper::RADS360)
      {
        m_animationPosition -= MathHelper::RADS360;
      }
      PropertyUpdated(PropertyType::Layout);
    }


    void VerticalScroller::SetScrollPadding(const DpThicknessF& paddingDp)
    {
      m_scrollPaddingDp = paddingDp;
    }


    PxSize2D VerticalScroller::ArrangeOverride(const PxSize2D& finalSizePx)
    {
      if (m_animate)
      {
        const SpriteUnitConverter& unitConverter = GetContext()->UnitConverter;
        auto scrollPaddingPx = unitConverter.ToPxThickness(m_scrollPaddingDp);

        PxSize2D contentDesiredSizePx = GetContentDesiredSizePx();
        contentDesiredSizePx.SetWidth(finalSizePx.Width());
        contentDesiredSizePx.AddHeight(scrollPaddingPx.SumY());
        const auto scollarea = (contentDesiredSizePx.Height() - finalSizePx.Height());
        const float pos = scollarea > 0.0f ? -(((std::cos(m_animationPosition + MathHelper::RADS180) + 1.0f) / 2.0f) * scollarea) : 0.0f;
        const int32_t posPx = TypeConverter::UncheckedChangeTo<int32_t>(pos);
        ContentControl::CustomArrange(contentDesiredSizePx, PxPoint2(0, posPx));

        m_animationSpeed = scollarea > 0.001f ? (1.0f / scollarea) * 600.0f : 0.0f;
        m_animationSpeed = std::min(m_animationSpeed, 4.0f);
        return finalSizePx;
      }

      return ContentControl::ArrangeOverride(finalSizePx);
    }


    PxSize2D VerticalScroller::MeasureOverride(const PxAvailableSize& availableSizePx)
    {
      PxSize2D desiredSizePx = ContentControl::MeasureOverride(PxAvailableSize(availableSizePx.Width(), PxAvailableSizeUtil::InfiniteSpacePx));
      if (desiredSizePx.Height() > availableSizePx.Height())
      {
        // Enable scrolling
        m_animate = true;
      }
      else
      {
        m_animate = false;
        m_animationPosition = 0.0f;
      }
      // We always only take the available size in y (this is kind of a hack, but it works for this purpose)
      // desiredSize.Y = availableSize.Y;
      return availableSizePx.ToPxSize2D();
    }
  }
}
