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

#include <FslBase/String/ToString.hpp>
#include <FslGraphics/Log/LogPixelFormat.hpp>
#include <FslGraphics/UnitTest/Helper/Common.hpp>
#include <FslGraphics/UnitTest/Helper/TestFixtureFslGraphics.hpp>
#include <array>

using namespace Fsl;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define LOCAL_TO_STRUCT(X) LocalPixelFormatRecord((X), #X)

namespace
{
  using TestLog_LogPixelFormat = TestFixtureFslGraphics;

  struct LocalPixelFormatRecord
  {
    PixelFormat Format{PixelFormat::Undefined};
    std::string Desc;
    LocalPixelFormatRecord() = default;
    LocalPixelFormatRecord(const PixelFormat pixelFormat, const char* pszDesc)
      : Format(pixelFormat)
      , Desc(pszDesc)
    {
    }
  };

  constexpr std::size_t PIXEL_FORMAT_ENTRIES = static_cast<std::size_t>(PixelFormat::ENUM_ID_RANGE_COMMON_SIZE);
  static_assert(PIXEL_FORMAT_ENTRIES == 185, "We currently expect 185 pixel format entries");

  // clang-format off
  std::array<LocalPixelFormatRecord, PIXEL_FORMAT_ENTRIES> g_entries
  {
    LOCAL_TO_STRUCT(PixelFormat::Undefined),
    LOCAL_TO_STRUCT(PixelFormat::R4G4_UNORM_PACK8),
    LOCAL_TO_STRUCT(PixelFormat::R4G4B4A4_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::B4G4R4A4_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::R5G6B5_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::B5G6R5_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::R5G5B5A1_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::B5G5R5A1_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::A1R5G5B5_UNORM_PACK16),
    LOCAL_TO_STRUCT(PixelFormat::R8_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R8_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R8_SRGB),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R8G8_SRGB),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8_SRGB),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_SINT),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8_SRGB),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R8G8B8A8_SRGB),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_SINT),
    LOCAL_TO_STRUCT(PixelFormat::B8G8R8A8_SRGB),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_UNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_SNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_USCALED_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_SSCALED_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_UINT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_SINT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A8B8G8R8_SRGB_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2R10G10B10_UNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2R10G10B10_SNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2R10G10B10_USCALED_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2R10G10B10_SSCALED_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2R10G10B10_UINT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2R10G10B10_SINT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2B10G10R10_UNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2B10G10R10_SNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2B10G10R10_USCALED_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2B10G10R10_SSCALED_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2B10G10R10_UINT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::A2B10G10R10_SINT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::R16_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R16_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R16_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_SNORM),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_USCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_SSCALED),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R16G16B16A16_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R32_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R32_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R32_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32B32_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32B32_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32B32_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32B32A32_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32B32A32_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R32G32B32A32_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R64_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R64_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R64_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64B64_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64B64_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64B64_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64B64A64_UINT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64B64A64_SINT),
    LOCAL_TO_STRUCT(PixelFormat::R64G64B64A64_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::B10G11R11_UFLOAT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::E5B9G9R9_UFLOAT_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::D16_UNORM),
    LOCAL_TO_STRUCT(PixelFormat::X8_D24_UNORM_PACK32),
    LOCAL_TO_STRUCT(PixelFormat::D32_SFLOAT),
    LOCAL_TO_STRUCT(PixelFormat::S8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::D16_UNORM_S8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::D24_UNORM_S8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::D32_SFLOAT_S8_UINT),
    LOCAL_TO_STRUCT(PixelFormat::BC1_RGB_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC1_RGB_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC1_RGBA_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC1_RGBA_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC2_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC2_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC3_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC3_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC4_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC4_SNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC5_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC5_SNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC6H_UFLOAT_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC6H_SFLOAT_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC7_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::BC7_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ETC2_R8G8B8_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ETC2_R8G8B8_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ETC2_R8G8B8A1_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ETC2_R8G8B8A1_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ETC2_R8G8B8A8_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ETC2_R8G8B8A8_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::EAC_R11_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::EAC_R11_SNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::EAC_R11G11_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::EAC_R11G11_SNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_4x4_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_4x4_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_5x4_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_5x4_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_5x5_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_5x5_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_6x5_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_6x5_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_6x6_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_6x6_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_8x5_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_8x5_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_8x6_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_8x6_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_8x8_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_8x8_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x5_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x5_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x6_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x6_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x8_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x8_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x10_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_10x10_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_12x10_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_12x10_SRGB_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_12x12_UNORM_BLOCK),
    LOCAL_TO_STRUCT(PixelFormat::ASTC_12x12_SRGB_BLOCK)
  };
  // clang-format on
}


TEST(TestLog_LogPixelFormat, Log)
{
  for (const auto& entry : g_entries)
  {
    std::string str = entry.Desc;
    str += " (";
    str += ToString(static_cast<uint32_t>(entry.Format));
    str += ")";
    EXPECT_EQ(str, ToString(entry.Format)) << entry.Format;
  }
}
