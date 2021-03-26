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

#include <FslGraphics/Texture/Texture.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Log/Math/LogPoint2.hpp>
#include <FslBase/Log/Math/Pixel/LogPxExtent2D.hpp>
#include <FslBase/Log/Math/Pixel/LogPxExtent3D.hpp>
#include <FslBase/Log/Math/Pixel/LogPxSize2D.hpp>
#include <FslGraphics/Log/LogPixelFormat.hpp>
#include <FslGraphics/Log/LogStrideRequirement.hpp>
#include <FslGraphics/UnitTest/Helper/Common.hpp>
#include <FslGraphics/UnitTest/Helper/TestFixtureFslGraphics.hpp>

using namespace Fsl;

namespace
{
  using TestTexture_Texture = TestFixtureFslGraphics;
}


TEST(TestTexture_Texture, Construct_Default)
{
  Texture texture;
  EXPECT_FALSE(texture.IsValid());
  EXPECT_EQ(BitmapOrigin::Undefined, texture.GetBitmapOrigin());
  EXPECT_THROW(texture.GetExtent(0), UsageErrorException);
  EXPECT_THROW(texture.GetStride(0), UsageErrorException);
  EXPECT_EQ(0u, texture.GetFaces());
  EXPECT_EQ(0u, texture.GetLayers());
  EXPECT_EQ(0u, texture.GetLevels());
  EXPECT_EQ(PixelFormat::Undefined, texture.GetPixelFormat());
  EXPECT_EQ(PixelFormatLayout::Undefined, texture.GetPixelFormatLayout());
  EXPECT_EQ(TextureInfo(), texture.GetTextureInfo());
  EXPECT_EQ(TextureType::Undefined, texture.GetTextureType());
  EXPECT_EQ(0u, texture.GetTotalTexelCount());
  EXPECT_EQ(0u, texture.GetBlobCount());
  EXPECT_THROW(texture.GetBlob(0u), UsageErrorException);
  EXPECT_THROW(texture.GetTextureBlob(0u, 0u, 0u), std::invalid_argument);
  EXPECT_EQ(0u, texture.GetByteSize());
}
