#ifndef FSLSIMPLEUI_BASE_CONTROL_IMPL_SLIDERRENDERIMPL_HPP
#define FSLSIMPLEUI_BASE_CONTROL_IMPL_SLIDERRENDERIMPL_HPP
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

#include <FslBase/Math/Dp/DpPoint.hpp>
#include <FslBase/Math/Dp/DpSize.hpp>
#include <FslBase/Math/Dp/DpThickness.hpp>
#include <FslBase/Math/Pixel/PxSize2D.hpp>
#include <FslBase/Math/Pixel/PxVector2.hpp>
#include <FslGraphics/Color.hpp>
#include <FslGraphics/Transition/TransitionColor.hpp>
#include <FslSimpleUI/Base/Control/Logic/SliderPixelSpanInfo.hpp>
#include <FslSimpleUI/Base/DefaultValues.hpp>
#include <FslSimpleUI/Base/LayoutDirection.hpp>
#include <FslSimpleUI/Base/Layout/LayoutOrientation.hpp>
#include <memory>


namespace Fsl
{
  class ImageSprite;
  class INativeBatch2D;
  class NineSliceSprite;
  class SpriteUnitConverter;

  namespace UI
  {
    struct PxAvailableSize;
    struct RoutedEventArgs;
    class WindowMouseOverEvent;

    class SliderRenderImpl
    {
      struct BackgroundGraphicsRecord
      {
        std::shared_ptr<NineSliceSprite> Sprite;
        Color EnabledColor;
        Color DisabledColor;

        explicit BackgroundGraphicsRecord(const Color& enabledColor, const Color& disabledColor)
          : EnabledColor(enabledColor)
          , DisabledColor(disabledColor)
        {
        }
      };
      struct CursorGraphicsRecord
      {
        std::shared_ptr<ImageSprite> Sprite;
        DpPoint OriginDp;
        DpSize SizeDp;
        Color EnabledColor;
        Color DisabledColor;

        explicit CursorGraphicsRecord(const Color& enabledColor, const Color& disabledColor)
          : EnabledColor(enabledColor)
          , DisabledColor(disabledColor)
        {
        }
      };
      struct OverlayGraphicsRecord
      {
        std::shared_ptr<ImageSprite> Sprite;
        Color EnabledColor;
        TransitionColor CurrentColor;

        explicit OverlayGraphicsRecord(const Color& enabledColor, TransitionCache& rTransitionCache, const TransitionTimeSpan& time,
                                       const TransitionType type)
          : EnabledColor(enabledColor)
          , CurrentColor(rTransitionCache, time, type)
        {
        }
      };

      BackgroundGraphicsRecord m_background{DefaultColor::Palette::Primary, DefaultColor::Palette::PrimaryDisabled};
      CursorGraphicsRecord m_cursor{DefaultColor::Palette::Primary, DefaultColor::Palette::PrimaryDisabled};
      OverlayGraphicsRecord m_cursorOverlay;

      bool m_isHovering{false};
      bool m_verticalGraphicsRotationEnabled{false};

    public:
      explicit SliderRenderImpl(TransitionCache& rTransitionCache);

      bool GetEnableVerticalGraphicsRotation() const
      {
        return m_verticalGraphicsRotationEnabled;
      }

      bool SetEnableVerticalGraphicsRotation(const bool enabled)
      {
        const bool modified = enabled != m_verticalGraphicsRotationEnabled;
        if (modified)
        {
          m_verticalGraphicsRotationEnabled = enabled;
        }
        return modified;
      }

      const std::shared_ptr<ImageSprite>& GetCursorSprite() const
      {
        return m_cursor.Sprite;
      }

      bool SetCursorSprite(const std::shared_ptr<ImageSprite>& value)
      {
        const bool modified = value != m_cursor.Sprite;
        if (modified)
        {
          m_cursor.Sprite = value;
        }
        return modified;
      }


      const Color& GetCursorColor() const
      {
        return m_cursor.EnabledColor;
      }

      bool SetCursorColor(const Color& value)
      {
        const bool modified = value != m_cursor.EnabledColor;
        if (modified)
        {
          m_cursor.EnabledColor = value;
        }
        return modified;
      }

      const Color& GetCursorDisabledColor() const
      {
        return m_cursor.DisabledColor;
      }

      bool SetCursorDisabledColor(const Color& value)
      {
        const bool modified = value != m_cursor.DisabledColor;
        if (modified)
        {
          m_cursor.DisabledColor = value;
        }
        return modified;
      }

      const DpPoint& GetCursorOrigin() const
      {
        return m_cursor.OriginDp;
      }

      bool SetCursorOrigin(const DpPoint& value)
      {
        const bool modified = value != m_cursor.OriginDp;
        if (modified)
        {
          m_cursor.OriginDp = value;
        }
        return modified;
      }

      const DpSize& GetCursorSize() const
      {
        return m_cursor.SizeDp;
      }

      bool SetCursorSize(const DpSize& value)
      {
        const bool modified = value != m_cursor.SizeDp;
        if (modified)
        {
          m_cursor.SizeDp = value;
        }
        return modified;
      }

      // ------


      const std::shared_ptr<ImageSprite>& GetCursorOverlaySprite() const
      {
        return m_cursorOverlay.Sprite;
      }

      bool SetCursorOverlaySprite(const std::shared_ptr<ImageSprite>& value)
      {
        const bool modified = value != m_cursorOverlay.Sprite;
        if (modified)
        {
          m_cursorOverlay.Sprite = value;
        }
        return modified;
      }

      const Color& GetCursorOverlayColor() const
      {
        return m_cursorOverlay.EnabledColor;
      }

      bool SetCursorOverlayColor(const Color& value)
      {
        const bool modified = value != m_cursorOverlay.EnabledColor;
        if (modified)
        {
          m_cursorOverlay.EnabledColor = value;
        }
        return modified;
      }

      // ------

      const std::shared_ptr<NineSliceSprite>& GetBackgroundSprite() const
      {
        return m_background.Sprite;
      }

      bool SetBackgroundSprite(const std::shared_ptr<NineSliceSprite>& value)
      {
        const bool modified = value != m_background.Sprite;
        if (modified)
        {
          m_background.Sprite = value;
        }
        return modified;
      }

      const Color& GetBackgroundColor() const
      {
        return m_background.EnabledColor;
      }

      bool SetBackgroundColor(const Color& value)
      {
        const bool modified = value != m_background.EnabledColor;
        if (modified)
        {
          m_background.EnabledColor = value;
        }
        return modified;
      }

      const Color& GetBackgroundDisabledColor() const
      {
        return m_background.DisabledColor;
      }

      bool SetBackgroundDisabledColor(const Color& value)
      {
        const bool modified = value != m_background.DisabledColor;
        if (modified)
        {
          m_background.DisabledColor = value;
        }
        return modified;
      }

      // ------

      SliderPixelSpanInfo WinDraw(INativeBatch2D& batch, const PxVector2 dstPositionPxf, const PxSize2D& renderSizePx,
                                  const LayoutOrientation orientation, const LayoutDirection layoutDirection, const bool isEnabled,
                                  const int32_t cursorPositionPx, const bool isDragging, const SpriteUnitConverter& spriteUnitConverter);

      void OnMouseOver(const RoutedEventArgs& args, const std::shared_ptr<WindowMouseOverEvent>& theEvent);

      PxSize2D Measure(const PxAvailableSize& availableSizePx);

      void UpdateAnimation(const TransitionTimeSpan& timeSpan);

      bool UpdateAnimationState(const bool forceCompleteAnimation, const bool isEnabled, const bool isDragging);
    };
  }
}

#endif
