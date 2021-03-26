#ifndef FSLUTIL_OPENGLES3_NATIVEBATCH2D_HPP
#define FSLUTIL_OPENGLES3_NATIVEBATCH2D_HPP
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

// Make sure Common.hpp is the first include file (to make the error message as helpful as possible when disabled)
#include <FslUtil/OpenGLES3/Common.hpp>
#include <FslBase/String/StringViewLite.hpp>
#include <FslBase/String/StringViewLiteUtil.hpp>
#include <FslGraphics/Color.hpp>
#include <FslGraphics/Render/Adapter/INativeBatch2D.hpp>
#include <FslUtil/OpenGLES3/GLBatch2D.hpp>

namespace Fsl
{
  namespace GLES3
  {
    class NativeBatch2D
      : public GLBatch2D
      , public INativeBatch2D
    {
    public:
      explicit NativeBatch2D(const PxExtent2D& extentPx);
      NativeBatch2D(const std::shared_ptr<GLBatch2DQuadRenderer>& quadRenderer, const PxExtent2D& extentPx);

      void Begin() final
      {
        return GLBatch2D::Begin();
      }

      void Begin(const BlendState blendState) final
      {
        return GLBatch2D::Begin(blendState);
      }

      void Begin(const BlendState blendState, const bool restoreState) final
      {
        return GLBatch2D::Begin(blendState, restoreState);
      }

      void ChangeTo(const BlendState blendState) final
      {
        return GLBatch2D::ChangeTo(blendState);
      }

      void End() final
      {
        return GLBatch2D::End();
      }

      void SetScreenExtent(const PxExtent2D& extentPx) final
      {
        GLBatch2D::SetScreenExtent(extentPx);
      }

      Batch2DStats GetStats() const final
      {
        return GLBatch2D::GetStats();
      }

      // Pull in the draw methods from GLBatch2D
      using GLBatch2D::DebugDrawLine;
      using GLBatch2D::DebugDrawRectangle;
      using GLBatch2D::Draw;
      using GLBatch2D::DrawString;

      // ---------- 0

      void Draw(const INativeTexture2D& srcTexture, const NativeTextureArea& srcArea, const PxAreaRectangleF& dstRectanglePxf,
                const Color& color) final;
      void Draw(const INativeTexture2D& srcTexture, const NativeTextureArea& srcArea, const PxAreaRectangleF& dstRectanglePxf,
                const Vector4& color) final;
      void Draw(const INativeTexture2D& srcTexture, const NativeQuadTextureCoords& srcArea, const PxAreaRectangleF& dstRectanglePxf,
                const Color& color) final;
      void Draw(const INativeTexture2D& srcTexture, const NativeQuadTextureCoords& srcArea, const PxAreaRectangleF& dstRectanglePxf,
                const Vector4& color) final;

      // ---------- 0 with clip

      void Draw(const INativeTexture2D& srcTexture, const NativeTextureArea& srcArea, const PxAreaRectangleF& dstRectanglePxf, const Color& color,
                const PxClipRectangle& clipRectPx) final;
      void Draw(const INativeTexture2D& srcTexture, const NativeTextureArea& srcArea, const PxAreaRectangleF& dstRectanglePxf, const Vector4& color,
                const PxClipRectangle& clipRectPx) final;
      void Draw(const INativeTexture2D& srcTexture, const NativeQuadTextureCoords& srcArea, const PxAreaRectangleF& dstRectanglePxf,
                const Color& color, const PxClipRectangle& clipRectPx) final;
      void Draw(const INativeTexture2D& srcTexture, const NativeQuadTextureCoords& srcArea, const PxAreaRectangleF& dstRectanglePxf,
                const Vector4& color, const PxClipRectangle& clipRectPx) final;

      // ---------- 1
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const PxRectangle& dstRectanglePx, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const PxRectangle& dstRectanglePx, const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const Color& color, const BatchEffect effect) final;
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const Color& color, const BatchEffect effect) final;
      // ---------- 2
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      // ---------- 2 with clip
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final;
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, clipRectPx);
      }
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, clipRectPx);
      }
      // ---------- 2A
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const BatchEffect effect) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const BatchEffect effect) final;

      // ---------- 3
      void Draw(const AtlasTexture2D& srcTexture, const PxRectangle& dstRectanglePx, const PxRectangleU& srcRectanglePx, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const PxRectangle& dstRectanglePx, const PxRectangleU& srcRectanglePx, const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const PxRectangle& dstRectanglePx, const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, dstRectanglePx, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      void Draw(const BaseTexture2D& srcTexture, const PxRectangle& dstRectanglePx, const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, dstRectanglePx, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      // ---------- 4
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangleU& srcRectanglePx,
                const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangleU& srcRectanglePx,
                const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangle& srcRectanglePx,
                const Color& color) final
      {
        Draw(srcTexture, dstRectanglePxf, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, dstRectanglePxf, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      // ---------- 4 with clip
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final;
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final;
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangle& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final
      {
        Draw(srcTexture, dstRectanglePxf, ClampConvertToPxRectangleU(srcRectanglePx), color, clipRectPx);
      }
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangle& srcRectanglePx, const Color& color,
                const PxClipRectangle& clipRectPx) final
      {
        Draw(srcTexture, dstRectanglePxf, ClampConvertToPxRectangleU(srcRectanglePx), color, clipRectPx);
      }

      // ---------- 4A

      //! @brief Scale the texture area so it fits inside the dstRectangle
      void Draw(const AtlasTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const BatchEffect effect) final;

      //! @brief Scale the texture area so it fits inside the dstRectangle
      void Draw(const BaseTexture2D& srcTexture, const PxAreaRectangleF& dstRectanglePxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const BatchEffect effect) final;

      // ---------- 5
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                const Vector2& scale) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                const Vector2& scale) final;
      // ---------- 6
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const Color& color, const float rotation, const Vector2& origin,
                const Vector2& scale) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const Color& color, const float rotation, const Vector2& origin,
                const Vector2& scale) final;
      // ---------- 7
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale) final;
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, origin, scale);
      }
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, origin, scale);
      }


      // ---------- 7 with clip

      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale, const PxClipRectangle& clipRectPx) final;

      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale, const PxClipRectangle& clipRectPx) final;

      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale, const PxClipRectangle& clipRectPx) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, origin, scale, clipRectPx);
      }

      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const Vector2& origin, const Vector2& scale, const PxClipRectangle& clipRectPx) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, origin, scale, clipRectPx);
      }


      // ---------- 8
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const float rotation, const Vector2& origin, const Vector2& scale) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangleU& srcRectanglePx, const Color& color,
                const float rotation, const Vector2& origin, const Vector2& scale) final;
      void Draw(const AtlasTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const float rotation, const Vector2& origin, const Vector2& scale) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, rotation, origin, scale);
      }
      void Draw(const BaseTexture2D& srcTexture, const Vector2& dstPositionPxf, const PxRectangle& srcRectanglePx, const Color& color,
                const float rotation, const Vector2& origin, const Vector2& scale) final
      {
        Draw(srcTexture, dstPositionPxf, ClampConvertToPxRectangleU(srcRectanglePx), color, rotation, origin, scale);
      }
      // ---------- 9
      void Draw(const AtlasTexture2D& srcTexture, const Vector2* const pDstPositions, const int32_t dstPositionsLength, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2* const pDstPositions, const int32_t dstPositionsLength, const Color& color) final;
      // ---------- 10
      void Draw(const AtlasTexture2D& srcTexture, const Vector2* const pDstPositions, const int32_t dstPositionsLength,
                const PxRectangleU& srcRectanglePx, const Color& color) final;
      void Draw(const BaseTexture2D& srcTexture, const Vector2* const pDstPositions, const int32_t dstPositionsLength,
                const PxRectangleU& srcRectanglePx, const Color& color) final;
      void Draw(const AtlasTexture2D& srcTexture, const Vector2* const pDstPositions, const int32_t dstPositionsLength,
                const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, pDstPositions, dstPositionsLength, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      void Draw(const BaseTexture2D& srcTexture, const Vector2* const pDstPositions, const int32_t dstPositionsLength,
                const PxRectangle& srcRectanglePx, const Color& color) final
      {
        Draw(srcTexture, pDstPositions, dstPositionsLength, ClampConvertToPxRectangleU(srcRectanglePx), color);
      }
      // ---------- 11
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const StringViewLite& strView,
                      const Vector2& dstPositionPxf, const Color& color) final;
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const char* const psz, const Vector2& dstPositionPxf,
                      const Color& color) final
      {
        DrawString(srcTexture, font, StringViewLite(psz), dstPositionPxf, color);
      }

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const std::string& str, const Vector2& dstPositionPxf,
                      const Color& color) final
      {
        DrawString(srcTexture, font, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color);
      }

      // ---------- 12
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const StringViewLite& strView,
                      const Vector2& dstPositionPxf, const Color& color, const Vector2& origin, const Vector2& scale) final;
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const char* const psz, const Vector2& dstPositionPxf,
                      const Color& color, const Vector2& origin, const Vector2& scale) final
      {
        DrawString(srcTexture, font, StringViewLite(psz), dstPositionPxf, color, origin, scale);
      }
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const std::string& str, const Vector2& dstPositionPxf,
                      const Color& color, const Vector2& origin, const Vector2& scale) final
      {
        DrawString(srcTexture, font, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, origin, scale);
      }
      // ---------- 13
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig,
                      const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color) final;
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig, const char* const psz,
                      const Vector2& dstPositionPxf, const Color& color) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLite(psz), dstPositionPxf, color);
      }
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig, const std::string& str,
                      const Vector2& dstPositionPxf, const Color& color) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color);
      }
      // ---------- 13 with clip
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig,
                      const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color, const PxClipRectangle& clipRectPx) final;
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig, const char* const psz,
                      const Vector2& dstPositionPxf, const Color& color, const PxClipRectangle& clipRectPx) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLite(psz), dstPositionPxf, color, clipRectPx);
      }
      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig, const std::string& str,
                      const Vector2& dstPositionPxf, const Color& color, const PxClipRectangle& clipRectPx) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, clipRectPx);
      }

      // ---------- 14

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig,
                      const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale) final;

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const char* const psz, const BitmapFontConfig& fontConfig,
                      const Vector2& dstPositionPxf, const Color& color, const Vector2& origin, const Vector2& scale) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLite(psz), dstPositionPxf, color, origin, scale);
      }

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const std::string& str, const BitmapFontConfig& fontConfig,
                      const Vector2& dstPositionPxf, const Color& color, const Vector2& origin, const Vector2& scale) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, origin, scale);
      }

      // ---------- 14 with clip

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const BitmapFontConfig& fontConfig,
                      const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin, const Vector2& scale,
                      const PxClipRectangle& clipRectPx) final;

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const char* const psz, const BitmapFontConfig& fontConfig,
                      const Vector2& dstPositionPxf, const Color& color, const Vector2& origin, const Vector2& scale,
                      const PxClipRectangle& clipRectPx) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLite(psz), dstPositionPxf, color, origin, scale, clipRectPx);
      }

      void DrawString(const BaseTexture2D& srcTexture, const TextureAtlasBitmapFont& font, const std::string& str, const BitmapFontConfig& fontConfig,
                      const Vector2& dstPositionPxf, const Color& color, const Vector2& origin, const Vector2& scale,
                      const PxClipRectangle& clipRectPx) final
      {
        DrawString(srcTexture, font, fontConfig, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, origin, scale, clipRectPx);
      }

      // ---------- 15

      void DrawString(const SpriteFont& font, const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color) final;

      void DrawString(const SpriteFont& font, const char* const psz, const Vector2& dstPositionPxf, const Color& color) final
      {
        DrawString(font, StringViewLite(psz), dstPositionPxf, color);
      }

      void DrawString(const SpriteFont& font, const std::string& str, const Vector2& dstPositionPxf, const Color& color) final
      {
        DrawString(font, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color);
      }

      // ---------- 15 with clip

      void DrawString(const SpriteFont& font, const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color,
                      const PxClipRectangle& clipRectPx) final;

      void DrawString(const SpriteFont& font, const char* const psz, const Vector2& dstPositionPxf, const Color& color,
                      const PxClipRectangle& clipRectPx) final
      {
        DrawString(font, StringViewLite(psz), dstPositionPxf, color, clipRectPx);
      }

      void DrawString(const SpriteFont& font, const std::string& str, const Vector2& dstPositionPxf, const Color& color,
                      const PxClipRectangle& clipRectPx) final
      {
        DrawString(font, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, clipRectPx);
      }

      // ---------- 16

      void DrawString(const SpriteFont& font, const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale) final;

      void DrawString(const SpriteFont& font, const char* const psz, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale) final
      {
        DrawString(font, StringViewLite(psz), dstPositionPxf, color, origin, scale);
      }

      void DrawString(const SpriteFont& font, const std::string& str, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale) final
      {
        DrawString(font, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, origin, scale);
      }

      // ---------- 16 with clip

      void DrawString(const SpriteFont& font, const StringViewLite& strView, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale, const PxClipRectangle& clipRectPx) final;

      void DrawString(const SpriteFont& font, const char* const psz, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale, const PxClipRectangle& clipRectPx) final
      {
        DrawString(font, StringViewLite(psz), dstPositionPxf, color, origin, scale, clipRectPx);
      }

      void DrawString(const SpriteFont& font, const std::string& str, const Vector2& dstPositionPxf, const Color& color, const Vector2& origin,
                      const Vector2& scale, const PxClipRectangle& clipRectPx) final
      {
        DrawString(font, StringViewLiteUtil::AsStringViewLite(str), dstPositionPxf, color, origin, scale, clipRectPx);
      }

      // ----------
      void DebugDrawRectangle(const AtlasTexture2D& srcFillTexture, const PxRectangle& dstRectanglePx, const Color& color) final;
      void DebugDrawRectangle(const BaseTexture2D& srcFillTexture, const PxRectangle& dstRectanglePx, const Color& color) final;
      void DebugDrawRectangle(const AtlasTexture2D& srcFillTexture, const PxAreaRectangleF& dstRectanglePxf, const Color& color) final;
      void DebugDrawRectangle(const BaseTexture2D& srcFillTexture, const PxAreaRectangleF& dstRectanglePxf, const Color& color) final;
      // ----------
      void DebugDrawLine(const AtlasTexture2D& srcFillTexture, const Vector2& dstFromPxf, const Vector2& dstToPxf, const Color& color) final;
      void DebugDrawLine(const BaseTexture2D& srcFillTexture, const Vector2& dstFromPxf, const Vector2& dstToPxf, const Color& color) final;
    };
  }
}

#endif
