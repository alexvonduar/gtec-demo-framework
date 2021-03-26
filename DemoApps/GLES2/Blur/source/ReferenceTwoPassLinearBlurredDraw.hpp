#ifndef GLES2_BLUR_REFERENCETWOPASSLINEARBLURREDDRAW_HPP
#define GLES2_BLUR_REFERENCETWOPASSLINEARBLURREDDRAW_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
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
#include <FslBase/Math/Pixel/PxSize2D.hpp>
#include <FslDemoApp/Base/DemoAppConfig.hpp>
#include <FslDemoApp/Base/DemoTime.hpp>
#include <FslUtil/OpenGLES2/GLFrameBuffer.hpp>
#include <FslUtil/OpenGLES2/GLProgram.hpp>
#include <FslUtil/OpenGLES2/GLTexture.hpp>
#include <FslUtil/OpenGLES2/GLVertexBuffer.hpp>
#include <FslUtil/OpenGLES2/NativeBatch2D.hpp>
#include <memory>
#include "ABlurredDraw.hpp"
#include "Config.hpp"
#include "TwoPassShaders.hpp"

namespace Fsl
{
  class ReferenceTwoPassLinearBlurredDraw : public ABlurredDraw
  {
    std::shared_ptr<GLES2::NativeBatch2D> m_batch2D;
    PxSize2D m_screenResolution;
    GLES2::GLFrameBuffer m_framebufferOrg;
    GLES2::GLFrameBuffer m_framebufferBlur;
    TwoPassShaders m_shaders;
    GLES2::GLVertexBuffer m_vertexBufferLeft;
    GLES2::GLVertexBuffer m_vertexBufferRightX;
    GLES2::GLVertexBuffer m_vertexBufferRightY;

  public:
    ReferenceTwoPassLinearBlurredDraw(const DemoAppConfig& config, const Config& blurConfig);
    void Draw(AScene* const pScene) override;
  };
}

#endif
