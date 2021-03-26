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

#include "NativeGraphicsBasic2D.hpp"
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Math/Pixel/TypeConverter.hpp>
#include <FslGraphics/Bitmap/Bitmap.hpp>
#include <FslGraphics/Color.hpp>
#include <FslGraphics/Font/EmbeddedFont8x8.hpp>
#include <FslUtil/OpenVG/VGCheck.hpp>
#include <FslUtil/OpenVG/VGUtil.hpp>
#include <cassert>

namespace Fsl
{
  namespace
  {
    const char MIN_VALUE = 33;
    const char MAX_VALUE = 127;

    inline bool IsValidChar(const int ch)
    {
      return (ch >= int(MIN_VALUE) && ch <= int(MAX_VALUE));
    }
  }


  namespace OpenVG
  {
    NativeGraphicsBasic2D::NativeGraphicsBasic2D(const PxExtent2D& extentPx)
      : m_pxCurrentSize(TypeConverter::UncheckedTo<PxSize2D>(extentPx))
      , m_fontSize(EmbeddedFont8x8::CharacterSize())
      , m_inBegin(false)
      , m_oldMatrixMode(0)
      , m_oldScissorEnabled(VG_FALSE)
    {
      m_oldClearColor[0] = 0.0f;
      m_oldClearColor[1] = 0.0f;
      m_oldClearColor[2] = 0.0f;
      m_oldClearColor[3] = 0.0f;
      m_oldOrigin[0] = 0.0f;
      m_oldOrigin[1] = 0.0f;
      m_zeroOrigin[0] = 0.0f;
      m_zeroOrigin[1] = 0.0f;

      // Create the parent image
      Bitmap bitmap;
      EmbeddedFont8x8::CreateFontBitmap(bitmap, PixelFormat::R8G8B8A8_UNORM, PxPoint2(), RectangleSizeRestrictionFlag::Power2);

      // Other filtering possibilities:
      // VG_IMAGE_QUALITY_NONANTIALIASED
      // VG_IMAGE_QUALITY_FASTER
      // VG_IMAGE_QUALITY_BETTER
      m_fontImage.Reset(bitmap, VG_IMAGE_QUALITY_FASTER);

      const uint8_t firstChar = 33;
      const uint8_t lastChar = 127;
      const uint8_t numChars = (lastChar - firstChar) + 1;

      assert(EmbeddedFont8x8::MinCharacter() <= firstChar);
      assert(EmbeddedFont8x8::MaxCharacter() >= lastChar);

      m_font.Reset(numChars);

      // Create child images for each glyph and assign them to the valid chars
      const VGFont parentImage = m_fontImage.GetHandle();
      const int32_t imageWidth = bitmap.Width();
      const PxSize2D fontSize = EmbeddedFont8x8::CharacterSize();
      std::array<VGfloat, 2> origin = {0.0f, static_cast<VGfloat>(fontSize.Height())};
      std::array<VGfloat, 2> escapement = {static_cast<VGfloat>(fontSize.Width()), 0.0f};
      int32_t srcX = 0;
      int32_t srcY = 0;
      for (uint16_t i = 0; i < numChars; ++i)
      {
        const VGFont childImage = vgChildImage(parentImage, srcX, bitmap.Height() - srcY - fontSize.Height(), fontSize.Width(), fontSize.Height());
        vgSetGlyphToImage(m_font.GetHandle(), firstChar + i, childImage, origin.data(), escapement.data());
        m_fontImages[i].Reset(childImage, fontSize);

        srcX += fontSize.Width();
        if ((srcX + fontSize.Width()) > imageWidth)
        {
          srcX = 0;
          srcY += fontSize.Height();
        }
      }
    }


    NativeGraphicsBasic2D::~NativeGraphicsBasic2D() = default;


    void NativeGraphicsBasic2D::SetScreenExtent(const PxExtent2D& extentPx)
    {
      assert(!m_inBegin);
      m_pxCurrentSize = TypeConverter::UncheckedTo<PxSize2D>(extentPx);
    }


    void NativeGraphicsBasic2D::Begin()
    {
      assert(!m_inBegin);
      m_inBegin = true;

      // Query the old state. This is unfortunately necessary since we have to restore it.
      m_oldMatrixMode = vgGeti(VG_MATRIX_MODE);
      vgGetfv(VG_GLYPH_ORIGIN, static_cast<VGint>(m_oldOrigin.size()), m_oldOrigin.data());
      vgGetfv(VG_CLEAR_COLOR, static_cast<VGint>(m_oldClearColor.size()), m_oldClearColor.data());
      m_oldScissorEnabled = vgGeti(VG_SCISSORING);

      // Disable scissoring
      vgSeti(VG_SCISSORING, VG_FALSE);
      FSLGRAPHICSOPENVG_CHECK_FOR_ERROR();
    }


    void NativeGraphicsBasic2D::End()
    {
      assert(m_inBegin);
      m_inBegin = false;

      // Restore the state we modified to ensure that we don't modify the state too much behind the app's back
      vgSetfv(VG_GLYPH_ORIGIN, static_cast<VGint>(m_oldOrigin.size()), m_oldOrigin.data());
      vgSetfv(VG_CLEAR_COLOR, static_cast<VGint>(m_oldClearColor.size()), m_oldClearColor.data());
      vgSeti(VG_MATRIX_MODE, m_oldMatrixMode);
      vgSeti(VG_SCISSORING, m_oldScissorEnabled);
    }


    void NativeGraphicsBasic2D::DrawPoints(const Vector2* const pSrc, const uint32_t length, const Color& color)
    {
      assert(m_inBegin);
      assert(pSrc != nullptr);
      assert(length >= 0);

      if (length == 0)
      {
        return;
      }

      vgSetfv(VG_CLEAR_COLOR, 4, color.ToVector4().DirectAccess());

      // Benchmarking has shown that vgClear is fairly fast for single dot rendering on most platforms
      // On some creating and destroying a VGPath is faster (especially as the dot count increase)

      const VGint screenHeight = m_pxCurrentSize.Height() - 1;
      const Vector2* pSrcCoord = pSrc;
      const Vector2* const pSrcCoordEnd = pSrcCoord + length;
      while (pSrcCoord < pSrcCoordEnd)
      {
        vgClear(static_cast<VGint>(pSrcCoord->X), screenHeight - static_cast<VGint>(pSrcCoord->Y), 1, 1);
        ++pSrcCoord;
      }
    }


    void NativeGraphicsBasic2D::DrawString(const StringViewLite& strView, const Vector2& dstPosition)
    {
      assert(m_inBegin);

      if (strView.empty())
      {
        return;
      }

      // Ensure we have enough room
      if (strView.size() > m_glyphs.size())
      {
        m_glyphs.resize(strView.size());
        m_xAdjust.resize(strView.size());
      }

      // build the arrays needed to render
      const char* pSrc = strView.data();
      const char* const pSrcEnd = pSrc + strView.size();
      int32_t numGlyphs = 0;

      const auto charWidth = static_cast<float>(m_fontSize.Width());

      VGfloat dstX = dstPosition.X;

      // Handle leading 'non drawable chars' by skipping them
      while (pSrc < pSrcEnd && !IsValidChar(int(*pSrc)))
      {
        dstX += charWidth;
        ++pSrc;
      }

      // Build the 'glyph' list and x-adjust
      VGfloat currentGlyphWidth = 0;
      while (pSrc < pSrcEnd)
      {
        if (IsValidChar(int(*pSrc)))
        {
          m_glyphs[numGlyphs] = static_cast<unsigned char>(*pSrc);
          if (numGlyphs > 0)
          {
            m_xAdjust[numGlyphs - 1] = currentGlyphWidth;
          }
          ++numGlyphs;
          currentGlyphWidth = 0;
        }
        else
        {
          currentGlyphWidth += charWidth;
          FSLLOG3_WARNING_IF(*pSrc == 0, "Zero is not a valid character in a string!");
        }

        ++pSrc;
      }
      m_xAdjust[numGlyphs - 1] = currentGlyphWidth;

      // Finally render the text
      vgSetfv(VG_GLYPH_ORIGIN, static_cast<VGint>(m_zeroOrigin.size()), m_zeroOrigin.data());
      vgSeti(VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE);
      vgLoadIdentity();
      vgTranslate(dstX, (m_pxCurrentSize.Height() - dstPosition.Y));
      vgDrawGlyphs(m_font.GetHandle(), numGlyphs, m_glyphs.data(), m_xAdjust.data(), nullptr, VG_FILL_PATH, VG_TRUE);
    }


    PxSize2D NativeGraphicsBasic2D::FontSize() const
    {
      return m_fontSize;
    }
  }
}
