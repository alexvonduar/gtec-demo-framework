#ifndef FSLGRAPHICS_RENDER_STRATEGY_BATCHSEGMENTINFO_HPP
#define FSLGRAPHICS_RENDER_STRATEGY_BATCHSEGMENTINFO_HPP
/****************************************************************************************************************************************************
 * Copyright 2019 NXP
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

#include <FslBase/BasicTypes.hpp>
#include <FslGraphics/Render/BatchSdfRenderConfig.hpp>
#include <FslGraphics/Render/BlendState.hpp>

namespace Fsl
{
  template <typename TTexture>
  struct BatchSegmentInfo
  {
    using texture_info_type = TTexture;

    texture_info_type TextureInfo{};
    BlendState ActiveBlendState{BlendState::Opaque};
    BatchSdfRenderConfig SdfRenderConfig;
    uint32_t VertexCount{0};

    inline constexpr BatchSegmentInfo() = default;

    inline constexpr BatchSegmentInfo(const texture_info_type& nativeTexture, const BlendState blendState,
                                      const BatchSdfRenderConfig& sdfRenderConfig)
      : TextureInfo(nativeTexture)
      , ActiveBlendState(blendState)
      , SdfRenderConfig(sdfRenderConfig)
    {
    }

    inline constexpr BatchSegmentInfo(const texture_info_type& nativeTexture, const BlendState blendState,
                                      const BatchSdfRenderConfig& sdfRenderConfig, const uint32_t vertexCount)
      : TextureInfo(nativeTexture)
      , ActiveBlendState(blendState)
      , SdfRenderConfig(sdfRenderConfig)
      , VertexCount(vertexCount)
    {
    }
  };
}

#endif
