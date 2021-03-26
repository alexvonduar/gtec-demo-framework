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

#include <FslUtil/OpenGLES2/GLUtil.hpp>
#include <FslUtil/OpenGLES2/GLCheck.hpp>
#include <FslBase/String/StringUtil.hpp>
#include <FslGraphics/Bitmap/Bitmap.hpp>
#include <FslGraphics/Bitmap/BitmapUtil.hpp>
#include <FslGraphics/Exceptions.hpp>
#include <FslGraphics/Bitmap/RawBitmapUtil.hpp>
#include <array>
#include <cassert>
#include <cstring>
#include <GLES2/gl2.h>

namespace Fsl
{
  namespace GLES2
  {
    std::vector<StringViewLite> GLUtil::GetExtensions()
    {
      const auto* pszExtensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
      if (pszExtensions == nullptr)
      {
        GL_CHECK_FOR_ERROR();
        throw std::runtime_error("Failed to retrieve extension list");
      }
      return StringUtil::Split(pszExtensions, ' ', true);
    }


    bool GLUtil::HasExtension(const char* const pszExtensionName)
    {
      if (pszExtensionName == nullptr)
      {
        throw std::invalid_argument("pszExtensionName can not be null");
      }

      const auto* pszExtensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
      if (pszExtensions == nullptr)
      {
        return false;
      }

      const auto lenExtensionName = std::strlen(pszExtensionName);

      const char* pszCurrentLocation = pszExtensions;
      while (pszCurrentLocation != nullptr)
      {
        const char* pszCharLocation = strstr(pszCurrentLocation, pszExtensionName);
        if (pszCharLocation == nullptr)
        {
          return false;
        }

        // If this isn't the first entry, then the previous character must be a space
        if (pszCharLocation == pszExtensions || *(pszCharLocation - 1) == ' ')
        {
          const char endChar = *(pszCharLocation + lenExtensionName);
          if (endChar == ' ' || endChar == 0)
          {
            return true;
          }
        }
        // Not a exact match -> so exit
        pszCurrentLocation = pszCharLocation + 1;
      }
      return false;
    }

    void GLUtil::Capture(Bitmap& rBitmap, const PixelFormat pixelFormat)
    {
      // Read the viewport to get the current size
      std::array<GLint, 4> viewport{};
      GL_CHECK(glGetIntegerv(GL_VIEWPORT, viewport.data()))

      const Rectangle srcRectangle(viewport[0], viewport[1], viewport[2], viewport[3]);
      return Capture(rBitmap, pixelFormat, srcRectangle);
    }


    void GLUtil::Capture(Bitmap& rBitmap, const PixelFormat pixelFormat, const Rectangle& srcRectangle)
    {
      // We don't need to clear as we are going to overwrite everything anyway
      // We utilize PixelFormatLayout::R8G8B8A8 here since that is what glReadPixels is filling it with
      // that also allows the convert method to detect if the the supplied pixelFormat is different and then
      // convert as necessary
      rBitmap.Reset(PxExtent2D(srcRectangle.Width(), srcRectangle.Height()), PixelFormat::R8G8B8A8_UINT, BitmapOrigin::LowerLeft,
                    BitmapClearMethod::DontClear);

      // glFinish();
      {
        RawBitmapEx rawBitmap;
        Bitmap::ScopedDirectAccess scopedAccess(rBitmap, rawBitmap);
        GL_CHECK(glReadPixels(srcRectangle.X(), srcRectangle.Y(), srcRectangle.Width(), srcRectangle.Height(), GL_RGBA, GL_UNSIGNED_BYTE,
                              rawBitmap.Content()));
        RawBitmapUtil::FlipHorizontal(rawBitmap);
      }

      // Convert if necessary (convert will do nothing if the format is already correct)
      BitmapUtil::Convert(rBitmap, pixelFormat);
    }


    GLenum GLUtil::Convert(const PrimitiveType primitiveType)
    {
      switch (primitiveType)
      {
      case PrimitiveType::LineList:
        return GL_LINES;
      case PrimitiveType::LineStrip:
        return GL_LINE_STRIP;
      case PrimitiveType::TriangleList:
        return GL_TRIANGLES;
      case PrimitiveType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
      default:
        throw UnsupportedPrimitiveTypeException("Unknown primitive type");
      }
    }


    std::vector<GLint> GLUtil::GetCompressedTextureFormats()
    {
      GLint count = 0;
      glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &count);

      std::vector<GLint> res(count);
      if (count > 0)
      {
        glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, res.data());
      }
      return res;
    }

  }
}
