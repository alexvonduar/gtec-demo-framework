#ifndef FSLGRAPHICS_SPRITE_FONT_SPRITEFONT_HPP
#define FSLGRAPHICS_SPRITE_FONT_SPRITEFONT_HPP
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

#include <FslGraphics/Font/TextureAtlasBitmapFont.hpp>
#include <FslGraphics/Sprite/Font/SpriteFontInfo.hpp>
#include <FslGraphics/Sprite/ISprite.hpp>

namespace Fsl
{
  class StringViewLite;
  struct SpriteFontConfig;
  struct SpriteMaterialInfo;

  class SpriteFont final : public ISprite
  {
    SpriteFontInfo m_info;
    TextureAtlasBitmapFont m_bitmapFontAtlas;

  public:
    SpriteFont() = default;

    SpriteFont(const SpriteMaterialInfo& spriteMaterialInfo, const uint32_t imageDpi, const BitmapFont& bitmapFont,
               const SpriteFontConfig& spriteFontConfig, const uint32_t densityDpi, const StringViewLite& debugName);

    //! @brief Clears the material and the bitmap font.
    //!        This is intended to be used during 'resizing' where we free the existing content first to have all the memory available for loading.
    void ClearContent();

    void SetContent(const SpriteMaterialInfo& spriteMaterialInfo, const uint32_t imageDpi, const BitmapFont& bitmapFont,
                    const SpriteFontConfig& spriteFontConfig, const uint32_t densityDpi, const StringViewLite& debugName);

    const SpriteFontInfo& GetInfo() const
    {
      return m_info;
    }

    const SpriteMaterialInfo& GetMaterialInfo() const final;
    void Resize(const uint32_t densityDpi) final;

    //! @brief Measure the string size in pixels taking into account the default font config of the font
    PxSize2D MeasureString(const StringViewLite& strView) const;


    const TextureAtlasBitmapFont& GetAtlasBitmapFont() const
    {
      return m_bitmapFontAtlas;
    }
  };
}

#endif
