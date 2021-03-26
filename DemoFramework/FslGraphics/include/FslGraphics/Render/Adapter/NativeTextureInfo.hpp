#ifndef FSLGRAPHICS_RENDER_ADAPTER_NATIVETEXTUREINFO_HPP
#define FSLGRAPHICS_RENDER_ADAPTER_NATIVETEXTUREINFO_HPP
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

#include <FslBase/Math/Pixel/PxSize2D.hpp>
#include <FslGraphics/Render/Adapter/INativeTexture2D.hpp>
#include <memory>
#include <utility>

namespace Fsl
{
  struct NativeTextureInfo
  {
    std::shared_ptr<INativeTexture2D> Handle;
    PxSize2D Size;

    NativeTextureInfo() = default;

    NativeTextureInfo(std::shared_ptr<INativeTexture2D> handle, const PxSize2D& size)
      : Handle(std::move(handle))
      , Size(size)
    {
    }

    std::shared_ptr<INativeTexture2D> GetHandle() const
    {
      return Handle;
    }

    PxSize2D GetSize() const
    {
      return Size;
    }

    void Reset()
    {
      Handle.reset();
      Size = {};
    }

    bool operator==(const NativeTextureInfo& rhs) const
    {
      return Handle == rhs.Handle && Size == rhs.Size;
    }
    bool operator!=(const NativeTextureInfo& rhs) const
    {
      return !(*this == rhs);
    }
  };
}

#endif
