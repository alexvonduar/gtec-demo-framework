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

#include <FslGraphics/Exceptions.hpp>
#include <FslBase/Log/Log3Core.hpp>
#include <FslGraphics/PixelFormatUtil.hpp>
#include <FslGraphics/PixelFormatLayoutUtil.hpp>
#include <array>
#include <cassert>
#include <map>
#include <vector>

namespace Fsl
{
  namespace
  {
    constexpr const std::array<PixelFormat, 188> g_pixelFormats = {
      PixelFormat::Undefined,
      PixelFormat::R4G4_UNORM_PACK8,
      PixelFormat::R4G4B4A4_UNORM_PACK16,
      PixelFormat::B4G4R4A4_UNORM_PACK16,
      PixelFormat::R5G6B5_UNORM_PACK16,
      PixelFormat::B5G6R5_UNORM_PACK16,
      PixelFormat::R5G5B5A1_UNORM_PACK16,
      PixelFormat::B5G5R5A1_UNORM_PACK16,
      PixelFormat::A1R5G5B5_UNORM_PACK16,
      PixelFormat::R8_UNORM,
      PixelFormat::R8_SNORM,
      PixelFormat::R8_USCALED,
      PixelFormat::R8_SSCALED,
      PixelFormat::R8_UINT,
      PixelFormat::R8_SINT,
      PixelFormat::R8_SRGB,
      PixelFormat::R8G8_UNORM,
      PixelFormat::R8G8_SNORM,
      PixelFormat::R8G8_USCALED,
      PixelFormat::R8G8_SSCALED,
      PixelFormat::R8G8_UINT,
      PixelFormat::R8G8_SINT,
      PixelFormat::R8G8_SRGB,
      PixelFormat::R8G8B8_UNORM,
      PixelFormat::R8G8B8_SNORM,
      PixelFormat::R8G8B8_USCALED,
      PixelFormat::R8G8B8_SSCALED,
      PixelFormat::R8G8B8_UINT,
      PixelFormat::R8G8B8_SINT,
      PixelFormat::R8G8B8_SRGB,
      PixelFormat::B8G8R8_UNORM,
      PixelFormat::B8G8R8_SNORM,
      PixelFormat::B8G8R8_USCALED,
      PixelFormat::B8G8R8_SSCALED,
      PixelFormat::B8G8R8_UINT,
      PixelFormat::B8G8R8_SINT,
      PixelFormat::B8G8R8_SRGB,
      PixelFormat::R8G8B8A8_UNORM,
      PixelFormat::R8G8B8A8_SNORM,
      PixelFormat::R8G8B8A8_USCALED,
      PixelFormat::R8G8B8A8_SSCALED,
      PixelFormat::R8G8B8A8_UINT,
      PixelFormat::R8G8B8A8_SINT,
      PixelFormat::R8G8B8A8_SRGB,
      PixelFormat::B8G8R8A8_UNORM,
      PixelFormat::B8G8R8A8_SNORM,
      PixelFormat::B8G8R8A8_USCALED,
      PixelFormat::B8G8R8A8_SSCALED,
      PixelFormat::B8G8R8A8_UINT,
      PixelFormat::B8G8R8A8_SINT,
      PixelFormat::B8G8R8A8_SRGB,
      PixelFormat::A8B8G8R8_UNORM_PACK32,
      PixelFormat::A8B8G8R8_SNORM_PACK32,
      PixelFormat::A8B8G8R8_USCALED_PACK32,
      PixelFormat::A8B8G8R8_SSCALED_PACK32,
      PixelFormat::A8B8G8R8_UINT_PACK32,
      PixelFormat::A8B8G8R8_SINT_PACK32,
      PixelFormat::A8B8G8R8_SRGB_PACK32,
      PixelFormat::A2R10G10B10_UNORM_PACK32,
      PixelFormat::A2R10G10B10_SNORM_PACK32,
      PixelFormat::A2R10G10B10_USCALED_PACK32,
      PixelFormat::A2R10G10B10_SSCALED_PACK32,
      PixelFormat::A2R10G10B10_UINT_PACK32,
      PixelFormat::A2R10G10B10_SINT_PACK32,
      PixelFormat::A2B10G10R10_UNORM_PACK32,
      PixelFormat::A2B10G10R10_SNORM_PACK32,
      PixelFormat::A2B10G10R10_USCALED_PACK32,
      PixelFormat::A2B10G10R10_SSCALED_PACK32,
      PixelFormat::A2B10G10R10_UINT_PACK32,
      PixelFormat::A2B10G10R10_SINT_PACK32,
      PixelFormat::R16_UNORM,
      PixelFormat::R16_SNORM,
      PixelFormat::R16_USCALED,
      PixelFormat::R16_SSCALED,
      PixelFormat::R16_UINT,
      PixelFormat::R16_SINT,
      PixelFormat::R16_SFLOAT,
      PixelFormat::R16G16_UNORM,
      PixelFormat::R16G16_SNORM,
      PixelFormat::R16G16_USCALED,
      PixelFormat::R16G16_SSCALED,
      PixelFormat::R16G16_UINT,
      PixelFormat::R16G16_SINT,
      PixelFormat::R16G16_SFLOAT,
      PixelFormat::R16G16B16_UNORM,
      PixelFormat::R16G16B16_SNORM,
      PixelFormat::R16G16B16_USCALED,
      PixelFormat::R16G16B16_SSCALED,
      PixelFormat::R16G16B16_UINT,
      PixelFormat::R16G16B16_SINT,
      PixelFormat::R16G16B16_SFLOAT,
      PixelFormat::R16G16B16A16_UNORM,
      PixelFormat::R16G16B16A16_SNORM,
      PixelFormat::R16G16B16A16_USCALED,
      PixelFormat::R16G16B16A16_SSCALED,
      PixelFormat::R16G16B16A16_UINT,
      PixelFormat::R16G16B16A16_SINT,
      PixelFormat::R16G16B16A16_SFLOAT,
      PixelFormat::R32_UINT,
      PixelFormat::R32_SINT,
      PixelFormat::R32_SFLOAT,
      PixelFormat::R32G32_UINT,
      PixelFormat::R32G32_SINT,
      PixelFormat::R32G32_SFLOAT,
      PixelFormat::R32G32B32_UINT,
      PixelFormat::R32G32B32_SINT,
      PixelFormat::R32G32B32_SFLOAT,
      PixelFormat::R32G32B32A32_UINT,
      PixelFormat::R32G32B32A32_SINT,
      PixelFormat::R32G32B32A32_SFLOAT,
      PixelFormat::R64_UINT,
      PixelFormat::R64_SINT,
      PixelFormat::R64_SFLOAT,
      PixelFormat::R64G64_UINT,
      PixelFormat::R64G64_SINT,
      PixelFormat::R64G64_SFLOAT,
      PixelFormat::R64G64B64_UINT,
      PixelFormat::R64G64B64_SINT,
      PixelFormat::R64G64B64_SFLOAT,
      PixelFormat::R64G64B64A64_UINT,
      PixelFormat::R64G64B64A64_SINT,
      PixelFormat::R64G64B64A64_SFLOAT,
      PixelFormat::B10G11R11_UFLOAT_PACK32,
      PixelFormat::E5B9G9R9_UFLOAT_PACK32,
      PixelFormat::D16_UNORM,
      PixelFormat::X8_D24_UNORM_PACK32,
      PixelFormat::D32_SFLOAT,
      PixelFormat::S8_UINT,
      PixelFormat::D16_UNORM_S8_UINT,
      PixelFormat::D24_UNORM_S8_UINT,
      PixelFormat::D32_SFLOAT_S8_UINT,
      PixelFormat::BC1_RGB_UNORM_BLOCK,
      PixelFormat::BC1_RGB_SRGB_BLOCK,
      PixelFormat::BC1_RGBA_UNORM_BLOCK,
      PixelFormat::BC1_RGBA_SRGB_BLOCK,
      PixelFormat::BC2_UNORM_BLOCK,
      PixelFormat::BC2_SRGB_BLOCK,
      PixelFormat::BC3_UNORM_BLOCK,
      PixelFormat::BC3_SRGB_BLOCK,
      PixelFormat::BC4_UNORM_BLOCK,
      PixelFormat::BC4_SNORM_BLOCK,
      PixelFormat::BC5_UNORM_BLOCK,
      PixelFormat::BC5_SNORM_BLOCK,
      PixelFormat::BC6H_UFLOAT_BLOCK,
      PixelFormat::BC6H_SFLOAT_BLOCK,
      PixelFormat::BC7_UNORM_BLOCK,
      PixelFormat::BC7_SRGB_BLOCK,
      PixelFormat::ETC2_R8G8B8_UNORM_BLOCK,
      PixelFormat::ETC2_R8G8B8_SRGB_BLOCK,
      PixelFormat::ETC2_R8G8B8A1_UNORM_BLOCK,
      PixelFormat::ETC2_R8G8B8A1_SRGB_BLOCK,
      PixelFormat::ETC2_R8G8B8A8_UNORM_BLOCK,
      PixelFormat::ETC2_R8G8B8A8_SRGB_BLOCK,
      PixelFormat::EAC_R11_UNORM_BLOCK,
      PixelFormat::EAC_R11_SNORM_BLOCK,
      PixelFormat::EAC_R11G11_UNORM_BLOCK,
      PixelFormat::EAC_R11G11_SNORM_BLOCK,
      PixelFormat::ASTC_4x4_UNORM_BLOCK,
      PixelFormat::ASTC_4x4_SRGB_BLOCK,
      PixelFormat::ASTC_5x4_UNORM_BLOCK,
      PixelFormat::ASTC_5x4_SRGB_BLOCK,
      PixelFormat::ASTC_5x5_UNORM_BLOCK,
      PixelFormat::ASTC_5x5_SRGB_BLOCK,
      PixelFormat::ASTC_6x5_UNORM_BLOCK,
      PixelFormat::ASTC_6x5_SRGB_BLOCK,
      PixelFormat::ASTC_6x6_UNORM_BLOCK,
      PixelFormat::ASTC_6x6_SRGB_BLOCK,
      PixelFormat::ASTC_8x5_UNORM_BLOCK,
      PixelFormat::ASTC_8x5_SRGB_BLOCK,
      PixelFormat::ASTC_8x6_UNORM_BLOCK,
      PixelFormat::ASTC_8x6_SRGB_BLOCK,
      PixelFormat::ASTC_8x8_UNORM_BLOCK,
      PixelFormat::ASTC_8x8_SRGB_BLOCK,
      PixelFormat::ASTC_10x5_UNORM_BLOCK,
      PixelFormat::ASTC_10x5_SRGB_BLOCK,
      PixelFormat::ASTC_10x6_UNORM_BLOCK,
      PixelFormat::ASTC_10x6_SRGB_BLOCK,
      PixelFormat::ASTC_10x8_UNORM_BLOCK,
      PixelFormat::ASTC_10x8_SRGB_BLOCK,
      PixelFormat::ASTC_10x10_UNORM_BLOCK,
      PixelFormat::ASTC_10x10_SRGB_BLOCK,
      PixelFormat::ASTC_12x10_UNORM_BLOCK,
      PixelFormat::ASTC_12x10_SRGB_BLOCK,
      PixelFormat::ASTC_12x12_UNORM_BLOCK,
      PixelFormat::ASTC_12x12_SRGB_BLOCK,
      //! These are custom formats not supported by Vulkan, but they are useful for OpenGL ES
      //! Unlike the Vulkan formats these commonly gets converted to another format by the GPU
      PixelFormat::EX_ALPHA8_UNORM,
      PixelFormat::EX_LUMINANCE8_UNORM,
      PixelFormat::EX_LUMINANCE8_ALPHA8_UNORM,
    };

    // Do some sanity checking
    static_assert(g_pixelFormats.size() == static_cast<std::size_t>(PixelFormat::ENUM_ID_RANGE_SIZE),
                  "g_pixelFormats needs to match the size of the enum range");

    constexpr const std::array<PixelFormat, 1> g_pixelLayoutUndefined = {PixelFormat::Undefined};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutR4G4 = {PixelFormat::R4G4_UNORM_PACK8};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutR4G4B4A4_PACK16 = {PixelFormat::R4G4B4A4_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutB4G4R4A4_PACK16 = {PixelFormat::B4G4R4A4_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutR5G6B5_PACK16 = {PixelFormat::R5G6B5_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutB5G6R5_PACK16 = {PixelFormat::B5G6R5_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutR5G5B5A1_PACK16 = {PixelFormat::R5G5B5A1_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutB5G5R5A1_PACK16 = {PixelFormat::B5G5R5A1_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutA1R5G5B5_PACK16 = {PixelFormat::A1R5G5B5_UNORM_PACK16};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR8 = {PixelFormat::R8_UNORM,   PixelFormat::R8_SNORM, PixelFormat::R8_USCALED,
                                                                  PixelFormat::R8_SSCALED, PixelFormat::R8_UINT,  PixelFormat::R8_SINT,
                                                                  PixelFormat::R8_SRGB};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR8G8 = {PixelFormat::R8G8_UNORM,   PixelFormat::R8G8_SNORM, PixelFormat::R8G8_USCALED,
                                                                    PixelFormat::R8G8_SSCALED, PixelFormat::R8G8_UINT,  PixelFormat::R8G8_SINT,
                                                                    PixelFormat::R8G8_SRGB};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR8G8B8 = {
      PixelFormat::R8G8B8_UNORM, PixelFormat::R8G8B8_SNORM, PixelFormat::R8G8B8_USCALED, PixelFormat::R8G8B8_SSCALED,
      PixelFormat::R8G8B8_UINT,  PixelFormat::R8G8B8_SINT,  PixelFormat::R8G8B8_SRGB};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutB8G8R8 = {
      PixelFormat::B8G8R8_UNORM, PixelFormat::B8G8R8_SNORM, PixelFormat::B8G8R8_USCALED, PixelFormat::B8G8R8_SSCALED,
      PixelFormat::B8G8R8_UINT,  PixelFormat::B8G8R8_SINT,  PixelFormat::B8G8R8_SRGB};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR8G8B8A8 = {
      PixelFormat::R8G8B8A8_UNORM, PixelFormat::R8G8B8A8_SNORM, PixelFormat::R8G8B8A8_USCALED, PixelFormat::R8G8B8A8_SSCALED,
      PixelFormat::R8G8B8A8_UINT,  PixelFormat::R8G8B8A8_SINT,  PixelFormat::R8G8B8A8_SRGB};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutB8G8R8A8 = {
      PixelFormat::B8G8R8A8_UNORM, PixelFormat::B8G8R8A8_SNORM, PixelFormat::B8G8R8A8_USCALED, PixelFormat::B8G8R8A8_SSCALED,
      PixelFormat::B8G8R8A8_UINT,  PixelFormat::B8G8R8A8_SINT,  PixelFormat::B8G8R8A8_SRGB};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutA8B8G8R8_PACK32 = {
      PixelFormat::A8B8G8R8_UNORM_PACK32,   PixelFormat::A8B8G8R8_SNORM_PACK32, PixelFormat::A8B8G8R8_USCALED_PACK32,
      PixelFormat::A8B8G8R8_SSCALED_PACK32, PixelFormat::A8B8G8R8_UINT_PACK32,  PixelFormat::A8B8G8R8_SINT_PACK32,
      PixelFormat::A8B8G8R8_SRGB_PACK32};
    constexpr const std::array<PixelFormat, 6> g_pixelLayoutA2R10G10B10_PACK32 = {
      PixelFormat::A2R10G10B10_UNORM_PACK32,   PixelFormat::A2R10G10B10_SNORM_PACK32, PixelFormat::A2R10G10B10_USCALED_PACK32,
      PixelFormat::A2R10G10B10_SSCALED_PACK32, PixelFormat::A2R10G10B10_UINT_PACK32,  PixelFormat::A2R10G10B10_SINT_PACK32};
    constexpr const std::array<PixelFormat, 6> g_pixelLayoutA2B10G10R10_PACK32 = {
      PixelFormat::A2B10G10R10_UNORM_PACK32,   PixelFormat::A2B10G10R10_SNORM_PACK32, PixelFormat::A2B10G10R10_USCALED_PACK32,
      PixelFormat::A2B10G10R10_SSCALED_PACK32, PixelFormat::A2B10G10R10_UINT_PACK32,  PixelFormat::A2B10G10R10_SINT_PACK32};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR16{PixelFormat::R16_UNORM,   PixelFormat::R16_SNORM, PixelFormat::R16_USCALED,
                                                                PixelFormat::R16_SSCALED, PixelFormat::R16_UINT,  PixelFormat::R16_SINT,
                                                                PixelFormat::R16_SFLOAT};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR16G16 = {
      PixelFormat::R16G16_UNORM, PixelFormat::R16G16_SNORM, PixelFormat::R16G16_USCALED, PixelFormat::R16G16_SSCALED,
      PixelFormat::R16G16_UINT,  PixelFormat::R16G16_SINT,  PixelFormat::R16G16_SFLOAT};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR16G16B16 = {
      PixelFormat::R16G16B16_UNORM, PixelFormat::R16G16B16_SNORM, PixelFormat::R16G16B16_USCALED, PixelFormat::R16G16B16_SSCALED,
      PixelFormat::R16G16B16_UINT,  PixelFormat::R16G16B16_SINT,  PixelFormat::R16G16B16_SFLOAT};
    constexpr const std::array<PixelFormat, 7> g_pixelLayoutR16G16B16A16 = {
      PixelFormat::R16G16B16A16_UNORM, PixelFormat::R16G16B16A16_SNORM, PixelFormat::R16G16B16A16_USCALED, PixelFormat::R16G16B16A16_SSCALED,
      PixelFormat::R16G16B16A16_UINT,  PixelFormat::R16G16B16A16_SINT,  PixelFormat::R16G16B16A16_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR32 = {PixelFormat::R32_UINT, PixelFormat::R32_SINT, PixelFormat::R32_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR32G32 = {PixelFormat::R32G32_UINT, PixelFormat::R32G32_SINT, PixelFormat::R32G32_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR32G32B32 = {PixelFormat::R32G32B32_UINT, PixelFormat::R32G32B32_SINT,
                                                                         PixelFormat::R32G32B32_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR32G32B32A32 = {PixelFormat::R32G32B32A32_UINT, PixelFormat::R32G32B32A32_SINT,
                                                                            PixelFormat::R32G32B32A32_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR64 = {PixelFormat::R64_UINT, PixelFormat::R64_SINT, PixelFormat::R64_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR64G64 = {PixelFormat::R64G64_UINT, PixelFormat::R64G64_SINT, PixelFormat::R64G64_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR64G64B64 = {PixelFormat::R64G64B64_UINT, PixelFormat::R64G64B64_SINT,
                                                                         PixelFormat::R64G64B64_SFLOAT};
    constexpr const std::array<PixelFormat, 3> g_pixelLayoutR64G64B64A64 = {PixelFormat::R64G64B64A64_UINT, PixelFormat::R64G64B64A64_SINT,
                                                                            PixelFormat::R64G64B64A64_SFLOAT};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutB10G11R11_PACK32 = {PixelFormat::B10G11R11_UFLOAT_PACK32};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutE5B9G9R9_PACK32 = {PixelFormat::E5B9G9R9_UFLOAT_PACK32};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutD16 = {PixelFormat::D16_UNORM};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutX8_D24_PACK32 = {PixelFormat::X8_D24_UNORM_PACK32};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutD32 = {PixelFormat::D32_SFLOAT};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutS8 = {PixelFormat::S8_UINT};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutD16_S8 = {PixelFormat::D16_UNORM_S8_UINT};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutD24_S8 = {PixelFormat::D24_UNORM_S8_UINT};
    constexpr const std::array<PixelFormat, 1> g_pixelLayoutD32_S8 = {PixelFormat::D32_SFLOAT_S8_UINT};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC1_RGB_BLOCK = {PixelFormat::BC1_RGB_UNORM_BLOCK, PixelFormat::BC1_RGB_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC1_RGBA_BLOCK = {PixelFormat::BC1_RGBA_UNORM_BLOCK, PixelFormat::BC1_RGBA_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC2_BLOCK = {PixelFormat::BC2_UNORM_BLOCK, PixelFormat::BC2_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC3_BLOCK = {PixelFormat::BC3_UNORM_BLOCK, PixelFormat::BC3_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC4_BLOCK = {PixelFormat::BC4_UNORM_BLOCK, PixelFormat::BC4_SNORM_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC5_BLOCK = {PixelFormat::BC5_UNORM_BLOCK, PixelFormat::BC5_SNORM_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC6H_BLOCK = {PixelFormat::BC6H_UFLOAT_BLOCK, PixelFormat::BC6H_SFLOAT_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutBC7_BLOCK = {PixelFormat::BC7_UNORM_BLOCK, PixelFormat::BC7_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutETC2_R8G8B8_BLOCK = {PixelFormat::ETC2_R8G8B8_UNORM_BLOCK,
                                                                                 PixelFormat::ETC2_R8G8B8_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutETC2_R8G8B8A1_BLOCK = {PixelFormat::ETC2_R8G8B8A1_UNORM_BLOCK,
                                                                                   PixelFormat::ETC2_R8G8B8A1_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutETC2_R8G8B8A8_BLOCK = {PixelFormat::ETC2_R8G8B8A8_UNORM_BLOCK,
                                                                                   PixelFormat::ETC2_R8G8B8A8_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutEAC_R11_BLOCK = {PixelFormat::EAC_R11_UNORM_BLOCK, PixelFormat::EAC_R11_SNORM_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutEAC_R11G11_BLOCK = {PixelFormat::EAC_R11G11_UNORM_BLOCK,
                                                                                PixelFormat::EAC_R11G11_SNORM_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_4x4_BLOCK = {PixelFormat::ASTC_4x4_UNORM_BLOCK, PixelFormat::ASTC_4x4_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_5x4_BLOCK = {PixelFormat::ASTC_5x4_UNORM_BLOCK, PixelFormat::ASTC_5x4_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_5x5_BLOCK = {PixelFormat::ASTC_5x5_UNORM_BLOCK, PixelFormat::ASTC_5x5_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_6x5_BLOCK = {PixelFormat::ASTC_6x5_UNORM_BLOCK, PixelFormat::ASTC_6x5_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_6x6_BLOCK = {PixelFormat::ASTC_6x6_UNORM_BLOCK, PixelFormat::ASTC_6x6_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_8x5_BLOCK = {PixelFormat::ASTC_8x5_UNORM_BLOCK, PixelFormat::ASTC_8x5_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_8x6_BLOCK = {PixelFormat::ASTC_8x6_UNORM_BLOCK, PixelFormat::ASTC_8x6_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_8x8_BLOCK = {PixelFormat::ASTC_8x8_UNORM_BLOCK, PixelFormat::ASTC_8x8_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_10x5_BLOCK = {PixelFormat::ASTC_10x5_UNORM_BLOCK, PixelFormat::ASTC_10x5_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_10x6_BLOCK = {PixelFormat::ASTC_10x6_UNORM_BLOCK, PixelFormat::ASTC_10x6_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_10x8_BLOCK = {PixelFormat::ASTC_10x8_UNORM_BLOCK, PixelFormat::ASTC_10x8_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_10x10_BLOCK = {PixelFormat::ASTC_10x10_UNORM_BLOCK,
                                                                                PixelFormat::ASTC_10x10_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_12x10_BLOCK = {PixelFormat::ASTC_12x10_UNORM_BLOCK,
                                                                                PixelFormat::ASTC_12x10_SRGB_BLOCK};
    constexpr const std::array<PixelFormat, 2> g_pixelLayoutASTC_12x12_BLOCK = {PixelFormat::ASTC_12x12_UNORM_BLOCK,
                                                                                PixelFormat::ASTC_12x12_SRGB_BLOCK};

    struct PixelLayoutFormatArray
    {
      const PixelFormat* const Formats{nullptr};
      const std::size_t Entries{0};

      constexpr PixelLayoutFormatArray() = default;

      explicit constexpr PixelLayoutFormatArray(const PixelFormat* const pFormats, const std::size_t entries)
        : Formats(pFormats)
        , Entries(entries)
      {
      }

      template <std::size_t TCount>
      explicit PixelLayoutFormatArray(const std::array<PixelFormat, TCount>& formats)
        : PixelLayoutFormatArray(formats.data(), formats.size())
      {
      }
    };


    const std::array<PixelLayoutFormatArray, 66> g_pixelLayoutToFormats = {
      PixelLayoutFormatArray(g_pixelLayoutUndefined),
      PixelLayoutFormatArray(g_pixelLayoutR4G4),
      PixelLayoutFormatArray(g_pixelLayoutR4G4B4A4_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutB4G4R4A4_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutR5G6B5_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutB5G6R5_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutR5G5B5A1_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutB5G5R5A1_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutA1R5G5B5_PACK16),
      PixelLayoutFormatArray(g_pixelLayoutR8),
      PixelLayoutFormatArray(g_pixelLayoutR8G8),
      PixelLayoutFormatArray(g_pixelLayoutR8G8B8),
      PixelLayoutFormatArray(g_pixelLayoutB8G8R8),
      PixelLayoutFormatArray(g_pixelLayoutR8G8B8A8),
      PixelLayoutFormatArray(g_pixelLayoutB8G8R8A8),
      PixelLayoutFormatArray(g_pixelLayoutA8B8G8R8_PACK32),
      PixelLayoutFormatArray(g_pixelLayoutA2R10G10B10_PACK32),
      PixelLayoutFormatArray(g_pixelLayoutA2B10G10R10_PACK32),
      PixelLayoutFormatArray(g_pixelLayoutR16),
      PixelLayoutFormatArray(g_pixelLayoutR16G16),
      PixelLayoutFormatArray(g_pixelLayoutR16G16B16),
      PixelLayoutFormatArray(g_pixelLayoutR16G16B16A16),
      PixelLayoutFormatArray(g_pixelLayoutR32),
      PixelLayoutFormatArray(g_pixelLayoutR32G32),
      PixelLayoutFormatArray(g_pixelLayoutR32G32B32),
      PixelLayoutFormatArray(g_pixelLayoutR32G32B32A32),
      PixelLayoutFormatArray(g_pixelLayoutR64),
      PixelLayoutFormatArray(g_pixelLayoutR64G64),
      PixelLayoutFormatArray(g_pixelLayoutR64G64B64),
      PixelLayoutFormatArray(g_pixelLayoutR64G64B64A64),
      PixelLayoutFormatArray(g_pixelLayoutB10G11R11_PACK32),
      PixelLayoutFormatArray(g_pixelLayoutE5B9G9R9_PACK32),
      PixelLayoutFormatArray(g_pixelLayoutD16),
      PixelLayoutFormatArray(g_pixelLayoutX8_D24_PACK32),
      PixelLayoutFormatArray(g_pixelLayoutD32),
      PixelLayoutFormatArray(g_pixelLayoutS8),
      PixelLayoutFormatArray(g_pixelLayoutD16_S8),
      PixelLayoutFormatArray(g_pixelLayoutD24_S8),
      PixelLayoutFormatArray(g_pixelLayoutD32_S8),
      PixelLayoutFormatArray(g_pixelLayoutBC1_RGB_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC1_RGBA_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC2_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC3_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC4_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC5_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC6H_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutBC7_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutETC2_R8G8B8_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutETC2_R8G8B8A1_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutETC2_R8G8B8A8_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutEAC_R11_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutEAC_R11G11_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_4x4_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_5x4_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_5x5_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_6x5_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_6x6_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_8x5_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_8x6_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_8x8_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_10x5_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_10x6_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_10x8_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_10x10_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_12x10_BLOCK),
      PixelLayoutFormatArray(g_pixelLayoutASTC_12x12_BLOCK),
    };

    PixelLayoutFormatArray TryGetArray(const PixelFormatLayout pixelFormatLayout)
    {
      const uint32_t layoutId = PixelFormatLayoutUtil::GetId(pixelFormatLayout);
      if (layoutId >= (sizeof(g_pixelLayoutToFormats) / sizeof(PixelLayoutFormatArray)))
      {
        return {};
      }
      return g_pixelLayoutToFormats[layoutId];
    }
  }


  uint32_t PixelFormatUtil::GetId(const PixelFormat pixelFormat)
  {
    const uint32_t pixelFormatId = static_cast<uint32_t>(pixelFormat) & PixelFormatFlags::BIT_MASK_FORMAT_ID;

    if (pixelFormatId <= static_cast<uint32_t>(PixelFormat::ENUM_ID_BEGIN_RANGE) ||
        pixelFormatId > static_cast<uint32_t>(PixelFormat::ENUM_ID_END_RANGE))
    {
      throw std::invalid_argument("The pixelFormat appears to be invalid");
    }
    return pixelFormatId;
  }


  uint32_t PixelFormatUtil::GetFormatRangeIndex(const PixelFormat pixelFormat)
  {
    const uint32_t pixelFormatId = static_cast<uint32_t>(pixelFormat) & PixelFormatFlags::BIT_MASK_FORMAT_ID;

    if (pixelFormatId <= static_cast<uint32_t>(PixelFormat::ENUM_ID_BEGIN_RANGE) ||
        pixelFormatId > static_cast<uint32_t>(PixelFormat::ENUM_ID_END_RANGE))
    {
      throw std::invalid_argument("The pixelFormat appears to be invalid");
    }
    return pixelFormatId - static_cast<uint32_t>(PixelFormat::ENUM_ID_BEGIN_RANGE);
  }


  bool PixelFormatUtil::IsCompressed(const PixelFormat pixelFormat)
  {
    return PixelFormatLayoutUtil::IsCompressed(GetPixelFormatLayout(pixelFormat));
  }


  bool PixelFormatUtil::IsPacked(const PixelFormat pixelFormat)
  {
    return PixelFormatLayoutUtil::IsPacked(GetPixelFormatLayout(pixelFormat));
  }


  PixelFormat PixelFormatUtil::TryGetPixelFormatById(const uint32_t formatId)
  {
    return (formatId < static_cast<uint32_t>(PixelFormat::ENUM_ID_RANGE_SIZE)) ? g_pixelFormats[formatId] : PixelFormat::Undefined;
  }


  PixelFormat PixelFormatUtil::TryFindPixelFormat(const uint32_t formatBits)
  {
    // Strip the FormatId and try to locate a format that is equal to the rest of the bits
    const uint32_t searchBits = (formatBits & (~static_cast<uint32_t>(PixelFormatFlags::BIT_MASK_FORMAT_ID)));
    bool found = false;
    uint32_t i = 0;
    while (i < static_cast<uint32_t>(PixelFormat::ENUM_ID_RANGE_SIZE) && !found)
    {
      found = searchBits == (static_cast<uint32_t>(g_pixelFormats[i]) & (~static_cast<uint32_t>(PixelFormatFlags::BIT_MASK_FORMAT_ID)));
      ++i;
    }
    // -1 since we ++ after the flag is set
    const auto foundIndex = i - 1;
    while (i < static_cast<uint32_t>(PixelFormat::ENUM_ID_RANGE_SIZE))
    {
      if (searchBits == (static_cast<uint32_t>(g_pixelFormats[i]) & (~static_cast<uint32_t>(PixelFormatFlags::BIT_MASK_FORMAT_ID))))
      {
        FSLLOG3_DEBUG_WARNING("Multiple candidates found");
        return PixelFormat::Undefined;
      }
      ++i;
    }
    // If we found a format then return it else return undefined
    return found ? g_pixelFormats[foundIndex] : PixelFormat::Undefined;
  }


  PixelFormat PixelFormatUtil::TryTransform(const PixelFormat pixelFormat, const PixelFormatFlags::Enum numericFormat)
  {
    if (pixelFormat == PixelFormat::Undefined)
    {
      return PixelFormat::Undefined;
    }

    const auto currentNumericFormat = GetNumericFormat(pixelFormat);
    if (currentNumericFormat == numericFormat)
    {
      return pixelFormat;
    }

    const auto currentLayout = GetPixelFormatLayout(pixelFormat);

    const auto layoutCompatiblePixelFormats = TryGetArray(currentLayout);
    if (layoutCompatiblePixelFormats.Formats == nullptr)
    {
      return PixelFormat::Undefined;
    }

    // Try to locate the entry
    for (uint32_t i = 0; i < layoutCompatiblePixelFormats.Entries; ++i)
    {
      const auto entryNumericFormat = GetNumericFormat(layoutCompatiblePixelFormats.Formats[i]);
      if (entryNumericFormat == numericFormat)
      {
        return layoutCompatiblePixelFormats.Formats[i];
      }
    }
    return PixelFormat::Undefined;
  }


  PixelFormat PixelFormatUtil::Transform(const PixelFormat pixelFormat, const PixelChannelOrder preferredChannelOrder)
  {
    if (pixelFormat == PixelFormat::Undefined)
    {
      return PixelFormat::Undefined;
    }
    if (preferredChannelOrder == PixelChannelOrder::Undefined)
    {
      return pixelFormat;
    }

    const auto pixelFormatLayout = GetPixelFormatLayout(pixelFormat);
    const auto transformedPixelLayout = PixelFormatLayoutUtil::Transform(pixelFormatLayout, preferredChannelOrder);
    if (pixelFormatLayout == transformedPixelLayout)
    {
      return pixelFormat;
    }

    // We found a new layout that matches the preferred channel order, so now we just need to check if its a valid pixel format
    const auto layoutCompatiblePixelFormats = TryGetArray(transformedPixelLayout);
    if (layoutCompatiblePixelFormats.Formats == nullptr)
    {
      return pixelFormat;
    }

    const auto originalNumericFormat = GetNumericFormat(pixelFormat);
    for (uint32_t i = 0; i < layoutCompatiblePixelFormats.Entries; ++i)
    {
      const auto entryNumericFormat = GetNumericFormat(layoutCompatiblePixelFormats.Formats[i]);
      if (entryNumericFormat == originalNumericFormat)
      {
        return layoutCompatiblePixelFormats.Formats[i];
      }
    }
    return pixelFormat;
  }


// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define LOCAL_ERROR_MESSAGE "The index did not match our assumption"


  // Due to lack of constexpr we use this nasty macro
  // So because of the macros be sure to keep all real code above this so you dont get any interference from them
  //#define LOCAL_GET_PIXELFORMAT_ID(X) ((static_cast<uint32_t>((X)) & static_cast<uint32_t>(PixelFormatFlags::BIT_MASK_FORMAT_ID)) -
  // static_cast<uint32_t>(PixelFormat::ENUM_ID_BEGIN_RANGE))
  constexpr uint32_t LOCAL_GET_PIXELFORMAT_ID(const PixelFormat pf)
  {
    return ((static_cast<uint32_t>((pf)) & static_cast<uint32_t>(PixelFormatFlags::BIT_MASK_FORMAT_ID)) -
            static_cast<uint32_t>(PixelFormat::ENUM_ID_BEGIN_RANGE));
  }


  // Some cut and pasted verification checks just to be 100% on the safe side and its a compile time check so its fine
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::Undefined) == 0, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R4G4_UNORM_PACK8) == 1, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R4G4B4A4_UNORM_PACK16) == 2, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B4G4R4A4_UNORM_PACK16) == 3, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R5G6B5_UNORM_PACK16) == 4, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B5G6R5_UNORM_PACK16) == 5, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R5G5B5A1_UNORM_PACK16) == 6, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B5G5R5A1_UNORM_PACK16) == 7, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A1R5G5B5_UNORM_PACK16) == 8, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_UNORM) == 9, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_SNORM) == 10, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_USCALED) == 11, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_SSCALED) == 12, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_UINT) == 13, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_SINT) == 14, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8_SRGB) == 15, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_UNORM) == 16, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_SNORM) == 17, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_USCALED) == 18, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_SSCALED) == 19, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_UINT) == 20, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_SINT) == 21, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8_SRGB) == 22, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_UNORM) == 23, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_SNORM) == 24, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_USCALED) == 25, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_SSCALED) == 26, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_UINT) == 27, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_SINT) == 28, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8_SRGB) == 29, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_UNORM) == 30, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_SNORM) == 31, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_USCALED) == 32, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_SSCALED) == 33, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_UINT) == 34, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_SINT) == 35, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8_SRGB) == 36, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_UNORM) == 37, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_SNORM) == 38, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_USCALED) == 39, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_SSCALED) == 40, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_UINT) == 41, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_SINT) == 42, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R8G8B8A8_SRGB) == 43, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_UNORM) == 44, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_SNORM) == 45, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_USCALED) == 46, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_SSCALED) == 47, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_UINT) == 48, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_SINT) == 49, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B8G8R8A8_SRGB) == 50, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_UNORM_PACK32) == 51, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_SNORM_PACK32) == 52, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_USCALED_PACK32) == 53, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_SSCALED_PACK32) == 54, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_UINT_PACK32) == 55, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_SINT_PACK32) == 56, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A8B8G8R8_SRGB_PACK32) == 57, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2R10G10B10_UNORM_PACK32) == 58, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2R10G10B10_SNORM_PACK32) == 59, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2R10G10B10_USCALED_PACK32) == 60, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2R10G10B10_SSCALED_PACK32) == 61, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2R10G10B10_UINT_PACK32) == 62, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2R10G10B10_SINT_PACK32) == 63, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2B10G10R10_UNORM_PACK32) == 64, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2B10G10R10_SNORM_PACK32) == 65, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2B10G10R10_USCALED_PACK32) == 66, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2B10G10R10_SSCALED_PACK32) == 67, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2B10G10R10_UINT_PACK32) == 68, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::A2B10G10R10_SINT_PACK32) == 69, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_UNORM) == 70, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_SNORM) == 71, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_USCALED) == 72, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_SSCALED) == 73, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_UINT) == 74, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_SINT) == 75, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16_SFLOAT) == 76, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_UNORM) == 77, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_SNORM) == 78, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_USCALED) == 79, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_SSCALED) == 80, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_UINT) == 81, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_SINT) == 82, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16_SFLOAT) == 83, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_UNORM) == 84, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_SNORM) == 85, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_USCALED) == 86, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_SSCALED) == 87, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_UINT) == 88, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_SINT) == 89, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16_SFLOAT) == 90, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_UNORM) == 91, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_SNORM) == 92, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_USCALED) == 93, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_SSCALED) == 94, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_UINT) == 95, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_SINT) == 96, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R16G16B16A16_SFLOAT) == 97, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32_UINT) == 98, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32_SINT) == 99, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32_SFLOAT) == 100, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32_UINT) == 101, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32_SINT) == 102, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32_SFLOAT) == 103, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32B32_UINT) == 104, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32B32_SINT) == 105, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32B32_SFLOAT) == 106, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32B32A32_UINT) == 107, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32B32A32_SINT) == 108, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R32G32B32A32_SFLOAT) == 109, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64_UINT) == 110, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64_SINT) == 111, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64_SFLOAT) == 112, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64_UINT) == 113, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64_SINT) == 114, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64_SFLOAT) == 115, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64B64_UINT) == 116, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64B64_SINT) == 117, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64B64_SFLOAT) == 118, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64B64A64_UINT) == 119, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64B64A64_SINT) == 120, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::R64G64B64A64_SFLOAT) == 121, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::B10G11R11_UFLOAT_PACK32) == 122, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::E5B9G9R9_UFLOAT_PACK32) == 123, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::D16_UNORM) == 124, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::X8_D24_UNORM_PACK32) == 125, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::D32_SFLOAT) == 126, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::S8_UINT) == 127, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::D16_UNORM_S8_UINT) == 128, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::D24_UNORM_S8_UINT) == 129, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::D32_SFLOAT_S8_UINT) == 130, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC1_RGB_UNORM_BLOCK) == 131, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC1_RGB_SRGB_BLOCK) == 132, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC1_RGBA_UNORM_BLOCK) == 133, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC1_RGBA_SRGB_BLOCK) == 134, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC2_UNORM_BLOCK) == 135, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC2_SRGB_BLOCK) == 136, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC3_UNORM_BLOCK) == 137, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC3_SRGB_BLOCK) == 138, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC4_UNORM_BLOCK) == 139, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC4_SNORM_BLOCK) == 140, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC5_UNORM_BLOCK) == 141, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC5_SNORM_BLOCK) == 142, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC6H_UFLOAT_BLOCK) == 143, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC6H_SFLOAT_BLOCK) == 144, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC7_UNORM_BLOCK) == 145, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::BC7_SRGB_BLOCK) == 146, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ETC2_R8G8B8_UNORM_BLOCK) == 147, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ETC2_R8G8B8_SRGB_BLOCK) == 148, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ETC2_R8G8B8A1_UNORM_BLOCK) == 149, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ETC2_R8G8B8A1_SRGB_BLOCK) == 150, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ETC2_R8G8B8A8_UNORM_BLOCK) == 151, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ETC2_R8G8B8A8_SRGB_BLOCK) == 152, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EAC_R11_UNORM_BLOCK) == 153, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EAC_R11_SNORM_BLOCK) == 154, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EAC_R11G11_UNORM_BLOCK) == 155, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EAC_R11G11_SNORM_BLOCK) == 156, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_4x4_UNORM_BLOCK) == 157, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_4x4_SRGB_BLOCK) == 158, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_5x4_UNORM_BLOCK) == 159, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_5x4_SRGB_BLOCK) == 160, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_5x5_UNORM_BLOCK) == 161, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_5x5_SRGB_BLOCK) == 162, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_6x5_UNORM_BLOCK) == 163, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_6x5_SRGB_BLOCK) == 164, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_6x6_UNORM_BLOCK) == 165, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_6x6_SRGB_BLOCK) == 166, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_8x5_UNORM_BLOCK) == 167, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_8x5_SRGB_BLOCK) == 168, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_8x6_UNORM_BLOCK) == 169, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_8x6_SRGB_BLOCK) == 170, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_8x8_UNORM_BLOCK) == 171, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_8x8_SRGB_BLOCK) == 172, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x5_UNORM_BLOCK) == 173, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x5_SRGB_BLOCK) == 174, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x6_UNORM_BLOCK) == 175, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x6_SRGB_BLOCK) == 176, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x8_UNORM_BLOCK) == 177, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x8_SRGB_BLOCK) == 178, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x10_UNORM_BLOCK) == 179, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_10x10_SRGB_BLOCK) == 180, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_12x10_UNORM_BLOCK) == 181, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_12x10_SRGB_BLOCK) == 182, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_12x12_UNORM_BLOCK) == 183, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::ASTC_12x12_SRGB_BLOCK) == 184, LOCAL_ERROR_MESSAGE);

  // extended formats
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EX_ALPHA8_UNORM) == 185, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EX_LUMINANCE8_UNORM) == 186, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELFORMAT_ID(PixelFormat::EX_LUMINANCE8_ALPHA8_UNORM) == 187, LOCAL_ERROR_MESSAGE);

  constexpr uint32_t LOCAL_GET_PIXELLAYOUT_ID(const PixelFormatLayout pixelFormatLayout)
  {
    return (static_cast<uint32_t>(pixelFormatLayout) & static_cast<uint32_t>(PixelFormatLayoutFlags::BIT_MASK_FORMAT_ID));
  }

  // Layout to pixelFormat lookup
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::Undefined) == 0, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R4G4) == 1, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R4G4B4A4_PACK16) == 2, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::B4G4R4A4_PACK16) == 3, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R5G6B5_PACK16) == 4, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::B5G6R5_PACK16) == 5, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R5G5B5A1_PACK16) == 6, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::B5G5R5A1_PACK16) == 7, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::A1R5G5B5_PACK16) == 8, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R8) == 9, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R8G8) == 10, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R8G8B8) == 11, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::B8G8R8) == 12, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R8G8B8A8) == 13, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::B8G8R8A8) == 14, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::A8B8G8R8_PACK32) == 15, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::A2R10G10B10_PACK32) == 16, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::A2B10G10R10_PACK32) == 17, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R16) == 18, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R16G16) == 19, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R16G16B16) == 20, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R16G16B16A16) == 21, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R32) == 22, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R32G32) == 23, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R32G32B32) == 24, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R32G32B32A32) == 25, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R64) == 26, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R64G64) == 27, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R64G64B64) == 28, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::R64G64B64A64) == 29, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::B10G11R11_PACK32) == 30, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::E5B9G9R9_PACK32) == 31, LOCAL_ERROR_MESSAGE);

  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::D16) == 32, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::X8_D24_PACK32) == 33, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::D32) == 34, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::S8) == 35, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::D16_S8) == 36, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::D24_S8) == 37, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::D32_S8) == 38, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC1_RGB_BLOCK) == 39, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC1_RGBA_BLOCK) == 40, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC2_BLOCK) == 41, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC3_BLOCK) == 42, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC4_BLOCK) == 43, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC5_BLOCK) == 44, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC6H_BLOCK) == 45, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::BC7_BLOCK) == 46, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ETC2_R8G8B8_BLOCK) == 47, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ETC2_R8G8B8A1_BLOCK) == 48, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ETC2_R8G8B8A8_BLOCK) == 49, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::EAC_R11_BLOCK) == 50, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::EAC_R11G11_BLOCK) == 51, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_4x4_BLOCK) == 52, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_5x4_BLOCK) == 53, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_5x5_BLOCK) == 54, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_6x5_BLOCK) == 55, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_6x6_BLOCK) == 56, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_8x5_BLOCK) == 57, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_8x6_BLOCK) == 58, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_8x8_BLOCK) == 59, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_10x5_BLOCK) == 60, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_10x6_BLOCK) == 61, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_10x8_BLOCK) == 62, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_10x10_BLOCK) == 63, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_12x10_BLOCK) == 64, LOCAL_ERROR_MESSAGE);
  static_assert(LOCAL_GET_PIXELLAYOUT_ID(PixelFormatLayout::ASTC_12x12_BLOCK) == 65, LOCAL_ERROR_MESSAGE);
}
