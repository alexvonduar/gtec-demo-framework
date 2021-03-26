#ifdef FSL_ENABLE_DEVIL
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

#include <FslBase/BasicTypes.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/IO/File.hpp>
#include <FslBase/IO/Path.hpp>
#include <FslGraphics/Bitmap/BitmapUtil.hpp>
#include <FslGraphics/Exceptions.hpp>
#include <FslGraphics/PixelFormatUtil.hpp>
#include <FslGraphics/IO/BMPUtil.hpp>
#include <FslGraphics/Texture/Texture.hpp>
#include <FslGraphics/Texture/TextureBlobBuilder.hpp>
#include <FslDemoApp/Util/Graphics/Service/ImageLibrary/ImageLibraryServiceDevIL.hpp>
#include <cassert>
#include <utility>
#include <IL/il.h>
#ifdef _WIN32
#include <FslBase/System/Platform/PlatformWin32.hpp>
#endif

#ifdef _WIN32
#include <FslBase/System/Platform/PlatformWin32.hpp>
#endif

namespace Fsl
{
  namespace
  {
    const char* GetDevILErrorString(ILenum error)
    {
      switch (error)
      {
      case IL_NO_ERROR:
        return "IL_NO_ERROR";
      case IL_INVALID_ENUM:
        return "IL_INVALID_ENUM";
      case IL_OUT_OF_MEMORY:
        return "IL_OUT_OF_MEMORY";
      case IL_FORMAT_NOT_SUPPORTED:
        return "IL_FORMAT_NOT_SUPPORTED";
      case IL_INTERNAL_ERROR:
        return "IL_INTERNAL_ERROR";
      case IL_INVALID_VALUE:
        return "IL_INVALID_VALUE";
      case IL_ILLEGAL_OPERATION:
        return "IL_ILLEGAL_OPERATION";
      case IL_ILLEGAL_FILE_VALUE:
        return "IL_ILLEGAL_FILE_VALUE";
      case IL_INVALID_FILE_HEADER:
        return "IL_INVALID_FILE_HEADER";
      case IL_INVALID_PARAM:
        return "IL_INVALID_PARAM";
      case IL_COULD_NOT_OPEN_FILE:
        return "IL_COULD_NOT_OPEN_FILE";
      case IL_INVALID_EXTENSION:
        return "IL_INVALID_EXTENSION";
      case IL_FILE_ALREADY_EXISTS:
        return "IL_FILE_ALREADY_EXISTS";
      case IL_OUT_FORMAT_SAME:
        return "IL_OUT_FORMAT_SAME";
      case IL_STACK_OVERFLOW:
        return "IL_STACK_OVERFLOW";
      case IL_STACK_UNDERFLOW:
        return "IL_STACK_UNDERFLOW";
      case IL_INVALID_CONVERSION:
        return "IL_INVALID_CONVERSION";
      case IL_BAD_DIMENSIONS:
        return "IL_BAD_DIMENSIONS";
      case IL_FILE_READ_ERROR:
        // case IL_FILE_WRITE_ERROR:
        return "IL_FILE_READ_ERROR/IL_FILE_WRITE_ERROR";
      default:
        return "unknown";
      }
    }


    struct DevILPixelFormat
    {
      ILint Format{0};
      ILint Type{0};

      constexpr DevILPixelFormat() = default;

      constexpr DevILPixelFormat(const ILint format, const ILint type)
        : Format(format)
        , Type(type)
      {
      }

      constexpr bool operator==(const DevILPixelFormat& rhs) const
      {
        return Format == rhs.Format && Type == rhs.Type;
      }


      constexpr bool operator!=(const DevILPixelFormat& rhs) const
      {
        return !(*this == rhs);
      }
    };


    ILenum Convert(BitmapOrigin& rOriginHint)
    {
      switch (rOriginHint)
      {
      case BitmapOrigin::UpperLeft:
        return IL_ORIGIN_UPPER_LEFT;
      case BitmapOrigin::LowerLeft:
        return IL_ORIGIN_LOWER_LEFT;
      default:
        FSLLOG3_WARNING("Unsupported origin {}", static_cast<int32_t>(rOriginHint));
        rOriginHint = BitmapOrigin::UpperLeft;
        return IL_ORIGIN_UPPER_LEFT;
      }
    }


    bool TryConvert(const DevILPixelFormat& format, PixelFormat& rPixelFormat)
    {
      switch (format.Format)
      {
      case IL_RGB:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::R8G8B8_UINT;
          return true;
        }
        else if (format.Type == IL_HALF)
        {
          rPixelFormat = PixelFormat::R16G16B16_SFLOAT;
          return true;
        }
        else if (format.Type == IL_FLOAT)
        {
          rPixelFormat = PixelFormat::R32G32B32_SFLOAT;
          return true;
        }
        break;
      case IL_BGR:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::B8G8R8_UINT;
          return true;
        }
        break;
      case IL_RGBA:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::R8G8B8A8_UINT;
          return true;
        }
        else if (format.Type == IL_FLOAT)
        {
          rPixelFormat = PixelFormat::R32G32B32A32_SFLOAT;
          return true;
        }
        else if (format.Type == IL_HALF)
        {
          rPixelFormat = PixelFormat::R16G16B16A16_SFLOAT;
          return true;
        }
        break;
      case IL_BGRA:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::B8G8R8A8_UINT;
          return true;
        }
        break;
      case IL_ALPHA:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::EX_ALPHA8_UNORM;
          return true;
        }
        break;
      case IL_LUMINANCE:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::EX_LUMINANCE8_UNORM;
          return true;
        }
        break;
      case IL_LUMINANCE_ALPHA:
        if (format.Type == IL_UNSIGNED_BYTE)
        {
          rPixelFormat = PixelFormat::EX_LUMINANCE8_ALPHA8_UNORM;
          return true;
        }
        break;
      default:
        break;
      }
      return false;
    }


    PixelFormat Convert(const DevILPixelFormat& format)
    {
      PixelFormat value;
      if (TryConvert(format, value))
      {
        return value;
      }
      throw GraphicsException("Unsupported image format");
    }


    DevILPixelFormat Convert(const PixelFormat format)
    {
      // Handle the special formats first
      switch (format)
      {
      case PixelFormat::EX_ALPHA8_UNORM:
        return {IL_ALPHA, IL_UNSIGNED_BYTE};
      case PixelFormat::EX_LUMINANCE8_UNORM:
        return {IL_LUMINANCE, IL_UNSIGNED_BYTE};
      case PixelFormat::EX_LUMINANCE8_ALPHA8_UNORM:
        return {IL_LUMINANCE_ALPHA, IL_UNSIGNED_BYTE};
      default:
        break;
      }

      switch (PixelFormatUtil::GetPixelFormatLayout(format))
      {
      case PixelFormatLayout::R8G8B8:
        return {IL_RGB, IL_UNSIGNED_BYTE};
      case PixelFormatLayout::B8G8R8:
        return {IL_BGR, IL_UNSIGNED_BYTE};
      case PixelFormatLayout::R8G8B8A8:
        return {IL_RGBA, IL_UNSIGNED_BYTE};
      case PixelFormatLayout::B8G8R8A8:
        return {IL_BGRA, IL_UNSIGNED_BYTE};
      case PixelFormatLayout::R32G32B32:
        return {IL_RGB, IL_FLOAT};
      case PixelFormatLayout::R32G32B32A32:
        return {IL_RGBA, IL_FLOAT};
      case PixelFormatLayout::R16G16B16:
        return {IL_RGB, IL_HALF};
      case PixelFormatLayout::R16G16B16A16:
        return {IL_RGBA, IL_HALF};
      default:
        throw UnsupportedPixelFormatException(format);
      }
    }

    struct ScopedDevILImage
    {
      ScopedDevILImage(const ScopedDevILImage&) = delete;
      ScopedDevILImage& operator=(const ScopedDevILImage&) = delete;

      ILuint Id{};

      ScopedDevILImage()
      {
        ilGenImages(1, &Id);
        if (ilGetError() != IL_NO_ERROR)
        {
          throw GraphicsException("Failed to create DevIL image");
        }
      }
      ~ScopedDevILImage()
      {
        ilDeleteImages(1, &Id);
      }
    };


    void ResetObject(Bitmap& rBitmap, std::vector<uint8_t>&& content, const PxExtent2D& extent, const PixelFormat pixelFormat,
                     const BitmapOrigin bitmapOrigin)
    {
      rBitmap.Reset(std::move(content), extent, pixelFormat, bitmapOrigin);
    }


    void ResetObject(Texture& rTexture, std::vector<uint8_t>&& content, const PxExtent2D& extent, const PixelFormat pixelFormat,
                     const BitmapOrigin bitmapOrigin)
    {
      rTexture.Reset(std::move(content), extent, pixelFormat, bitmapOrigin);
    }


    template <typename TImageContainer>
    bool LoadILImage(TImageContainer& rImageContainer, const IO::Path& path, const PixelFormat pixelFormatHint, const BitmapOrigin bitmapOrigin,
                     const PixelChannelOrder preferredChannelOrderHint)
    {
      bool doFormatConversion = pixelFormatHint != PixelFormat::Undefined;
      DevILPixelFormat targetFormat;
      if (doFormatConversion)
      {
        targetFormat = Convert(pixelFormatHint);
      }

      // Check if we should try a format conversion or not
      // the conversion is just a hint and if we don't support it we should just not do any conversion
      if (doFormatConversion)
      {
        PixelFormat tmpFormat;
        if (!TryConvert(targetFormat, tmpFormat))
        {
          doFormatConversion = false;
        }
      }

      ScopedDevILImage image;

      ilBindImage(image.Id);
      // FIX: investigate if DevIL supports UTF8 on all platforms.
      // Since the ToAsciiString() conversion breaks UTF8 support.
      // Its quite likely that UTF8 works on linux based platforms,
      // but windows might require a different solution.

#ifdef _WIN32
      // Loads into the current bound image
      ilLoadImage(PlatformWin32::Widen(path.ToUTF8String()).c_str());
#else
      ilLoadImage(path.ToAsciiString().c_str());
#endif

      ILenum devilError = ilGetError();
      if (devilError != IL_NO_ERROR)
      {
        FSLLOG3_WARNING("devIL image loading of '{}' not successfull: {} ({}).", path.ToUTF8String(), GetDevILErrorString(devilError), devilError);
        return false;
      }

      // int intFormatMode = ilGetInteger(IL_FORMAT_MODE);
      // int bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
      // int bytesPP = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

      int width = ilGetInteger(IL_IMAGE_WIDTH);
      int height = ilGetInteger(IL_IMAGE_HEIGHT);
      const int imageFormat = ilGetInteger(IL_IMAGE_FORMAT);
      const int imageType = ilGetInteger(IL_IMAGE_TYPE);
      DevILPixelFormat currentPixelFormat(imageFormat, imageType);

      if (pixelFormatHint == PixelFormat::Undefined && preferredChannelOrderHint != PixelChannelOrder::Undefined)
      {
        const PixelFormat activePixelFormat = Convert(currentPixelFormat);
        const auto preferredPixelFormat = PixelFormatUtil::Transform(activePixelFormat, preferredChannelOrderHint);
        if (activePixelFormat != preferredPixelFormat)
        {
          PixelFormat tmpFormat;
          targetFormat = Convert(preferredPixelFormat);
          doFormatConversion = TryConvert(targetFormat, tmpFormat);
        }
      }

      if (doFormatConversion && currentPixelFormat != targetFormat)
      {
        // The ilCopyPixels crash when using IL_ALPHA this prevents that
        if (targetFormat.Format != IL_ALPHA)
        {
          currentPixelFormat = targetFormat;
        }
      }


      // Extract the image.
      {
        const PixelFormat activePixelFormat = Convert(currentPixelFormat);
        const DevILPixelFormat activeImageFormat = Convert(activePixelFormat);
        const int bytesPerPixel = PixelFormatUtil::GetBytesPerPixel(activePixelFormat);

        // WARNING: IL_ALPHA conversions appear to be broken in devil (as it crashes in ilCopyPixels)

        const std::size_t widthEx = width;
        std::vector<uint8_t> content(widthEx * height * bytesPerPixel);
        ilCopyPixels(0, 0, 0, width, height, 1, activeImageFormat.Format, activeImageFormat.Type, content.data());

        ResetObject(rImageContainer, std::move(content), PxExtent2D(width, height), activePixelFormat, bitmapOrigin);
      }

      devilError = ilGetError();

      // Log any error that occurs
      FSLLOG3_WARNING_IF(devilError != IL_NO_ERROR, "devIL image conversion not successfull: {} ({}).\n", GetDevILErrorString(devilError),
                         devilError);
      return (devilError == IL_NO_ERROR);
    }


    template <typename TImageContainer>
    bool TryReadNow(TImageContainer& rImageContainer, const IO::Path& path, const PixelFormat pixelFormatHint, const BitmapOrigin originHint,
                    const PixelChannelOrder preferredChannelOrderHint, BitmapOrigin& rLastOrigin)
    {
      try
      {
        BitmapOrigin origin = originHint;
        if (originHint != rLastOrigin)
        {
          rLastOrigin = originHint;
          const ILenum devOrigin = Convert(origin);
          ilOriginFunc(devOrigin);
        }


        return LoadILImage(rImageContainer, path, pixelFormatHint, origin, preferredChannelOrderHint);
      }
      catch (std::exception& ex)
      {
        FSLLOG3_DEBUG_WARNING("devIL image conversion not successfull ({}).\n", ex.what());
        FSL_PARAM_NOT_USED(ex);
        return false;
      }
    }
  }


  ImageLibraryServiceDevIL::ImageLibraryServiceDevIL(const ServiceProvider& serviceProvider)
    : ThreadLocalService(serviceProvider)
    , m_lastOrigin(BitmapOrigin::UpperLeft)
  {
    ILuint devilError = IL_NO_ERROR;
    ilInit();
    devilError = ilGetError();
    if (devilError != IL_NO_ERROR)
    {
      throw std::runtime_error("devIL lib init failed");
    }

    // Make sure the images are using the expected origin
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
    ilEnable(IL_ORIGIN_SET);
  }


  ImageLibraryServiceDevIL::~ImageLibraryServiceDevIL() = default;


  std::string ImageLibraryServiceDevIL::GetName() const
  {
    return std::string("ImageLibraryServiceDevIL");
  }


  void ImageLibraryServiceDevIL::ExtractSupportedImageFormats(std::deque<ImageFormat>& rFormats)
  {
    rFormats.push_back(ImageFormat::Bmp);
    // rFormats.push_back(ImageFormat::DDS);
    // rFormats.push_back(ImageFormat::Exr);
    rFormats.push_back(ImageFormat::Hdr);
    rFormats.push_back(ImageFormat::Jpeg);
    rFormats.push_back(ImageFormat::Png);
    rFormats.push_back(ImageFormat::Tga);
  }


  bool ImageLibraryServiceDevIL::TryRead(Bitmap& rBitmap, const IO::Path& absolutePath, const PixelFormat pixelFormatHint,
                                         const BitmapOrigin originHint, const PixelChannelOrder preferredChannelOrderHint)
  {
    if (PixelFormatUtil::IsCompressed(pixelFormatHint))
    {
      return false;
    }
    return TryReadNow(rBitmap, absolutePath, pixelFormatHint, originHint, preferredChannelOrderHint, m_lastOrigin);
  }


  bool ImageLibraryServiceDevIL::TryRead(Texture& rTexture, const IO::Path& absolutePath, const PixelFormat pixelFormatHint,
                                         const BitmapOrigin originHint, const PixelChannelOrder preferredChannelOrderHint)
  {
    if (PixelFormatUtil::IsCompressed(pixelFormatHint))
    {
      return false;
    }
    return TryReadNow(rTexture, absolutePath, pixelFormatHint, originHint, preferredChannelOrderHint, m_lastOrigin);
  }


  bool ImageLibraryServiceDevIL::TryWrite(const IO::Path& path, const Bitmap& bitmap, const ImageFormat imageFormat, const bool allowOverwrite)
  {
    FSL_PARAM_NOT_USED(path);
    FSL_PARAM_NOT_USED(bitmap);
    FSL_PARAM_NOT_USED(imageFormat);
    FSL_PARAM_NOT_USED(allowOverwrite);
    // TODO: implement DevIL save support
    return false;
  }
}
#endif
