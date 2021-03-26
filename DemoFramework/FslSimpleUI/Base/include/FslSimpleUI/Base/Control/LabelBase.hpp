#ifndef FSLSIMPLEUI_BASE_CONTROL_LABELBASE_HPP
#define FSLSIMPLEUI_BASE_CONTROL_LABELBASE_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
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

#include <FslBase/String/StringViewLiteUtil.hpp>
#include <FslGraphics/Color.hpp>
#include <FslSimpleUI/Base/BaseWindow.hpp>
#include <string>

namespace Fsl
{
  class SpriteFont;

  namespace UI
  {
    class WindowContext;
    class LabelBase : public BaseWindow
    {
      bool m_enabled{true};

    protected:
      const std::shared_ptr<WindowContext> m_windowContext;

    private:
      std::shared_ptr<SpriteFont> m_font;
      Color m_fontColor{DefaultColor::Palette::Font};
      Color m_fontDisabledColor{DefaultColor::Palette::FontDisabled};
      ItemAlignment m_contentAlignmentX{ItemAlignment::Near};
      ItemAlignment m_contentAlignmentY{ItemAlignment::Near};

    public:
      explicit LabelBase(const std::shared_ptr<WindowContext>& context);

      bool IsEnabled() const
      {
        return m_enabled;
      }

      bool SetEnabled(const bool enabled);


      ItemAlignment GetContentAlignmentX() const
      {
        return m_contentAlignmentX;
      };

      void SetContentAlignmentX(const ItemAlignment& value);

      ItemAlignment GetContentAlignmentY() const
      {
        return m_contentAlignmentX;
      };

      void SetContentAlignmentY(const ItemAlignment& value);


      const std::shared_ptr<SpriteFont>& GetFont() const
      {
        return m_font;
      }

      void SetFont(const std::shared_ptr<SpriteFont>& value);

      Color GetFontColor() const
      {
        return m_fontColor;
      }

      void SetFontColor(const Color& color);

      Color GetFontDisabledColor() const
      {
        return m_fontDisabledColor;
      }

      void SetFontDisabledColor(const Color& color);

      void WinDraw(const UIDrawContext& context) override;

    protected:
      virtual StringViewLite DoGetContent() const = 0;

      PxSize2D ArrangeOverride(const PxSize2D& finalSizePx) override;
      PxSize2D MeasureOverride(const PxAvailableSize& availableSizePx) override;

      PxPoint2 DoMeasureRenderedString(const std::string& value) const
      {
        return DoMeasureRenderedString(StringViewLiteUtil::AsStringViewLite(value));
      }

      PxPoint2 DoMeasureRenderedString(const StringViewLite& value) const;
    };
  }
}

#endif
