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

#include <FslDemoApp/Base/Service/Content/IContentManager.hpp>
#include <FslDemoApp/Base/DemoTime.hpp>
#include <FslUtil/OpenGLES3/Exceptions.hpp>
#include <FslUtil/OpenGLES3/GLCheck.hpp>
#include <FslGraphics/Vertices/VertexPositionTexture.hpp>
#include "FractalShaderMandelbrot.hpp"
#include <Shared/FractalShader/OptionParser.hpp>
#include <FslBase/String/StringUtil.hpp>
#include <FslBase/String/ToString.hpp>
#include <GLES3/gl3.h>
#include <cmath>
#include <iostream>
#include <string>

namespace Fsl
{
  using namespace GLES3;

  FractalShaderMandelbrot::FractalShaderMandelbrot(const DemoAppConfig& config)
    : m_screenResolution(config.ScreenResolution)
    , m_locCoordinateOffset(GLValues::INVALID_LOCATION)
    , m_locCoordinateScale(GLValues::INVALID_LOCATION)
    , m_angle(0, 1.0f)
  {
    std::shared_ptr<OptionParser> options = config.GetOptions<OptionParser>();
    Config cfg = options->GetConfig();

    const std::shared_ptr<IContentManager> contentManager = config.DemoServiceProvider.Get<IContentManager>();
    {
      IO::Path fragmentShaderFile;
      switch (cfg.TheRenderMode)
      {
      // case RenderMode::Tex:
      //  fragmentShaderFile = "Mandelbrot_tex.frag";
      //  break;
      case RenderMode::Smooth:
        fragmentShaderFile = "Mandelbrot_smooth.frag";
        break;
      case RenderMode::Col:
        fragmentShaderFile = "Mandelbrot_col.frag";
        break;
      case RenderMode::Gray:
      default:
        fragmentShaderFile = "Mandelbrot_gray.frag";
        break;
      }

      // Dynamically patch the fragment shader with the desired iteration count
      std::string fragmentShader = contentManager->ReadAllText(fragmentShaderFile);
      StringUtil::Replace(fragmentShader, "##MAX_ITERATIONS##", ToString(cfg.IterationsM));
      m_program.Reset(contentManager->ReadAllText("Shader.vert"), fragmentShader);
    }


    const GLuint hProgram = m_program.Get();
    glUseProgram(hProgram);

    m_locCoordinateOffset = glGetUniformLocation(hProgram, "CoordinateOffset");
    m_locCoordinateScale = glGetUniformLocation(hProgram, "CoordinateScale");


    const float aspect = (m_screenResolution.Y / static_cast<float>(m_screenResolution.X));
    const float scaleX = 2.5f;
    const float scaleY = scaleX * aspect;
    const float u1 = (-1.0f) * scaleX;
    const float u2 = (1.0f) * scaleX;
    const float v1 = (-1.0f) * scaleY;
    const float v2 = (1.0f) * scaleY;

    const std::array<VertexPositionTexture, 4> vertices = {
      VertexPositionTexture(Vector3(-1.0f, 1.0f, 0.0f), Vector2(u1, v2)),
      VertexPositionTexture(Vector3(-1.0f, -1.0f, 0.0f), Vector2(u1, v1)),
      VertexPositionTexture(Vector3(1.0f, 1.0f, 0.0f), Vector2(u2, v2)),
      VertexPositionTexture(Vector3(1.0f, -1.0f, 0.0f), Vector2(u2, v1)),
    };

    m_vertexBuffer.Reset(vertices, GL_STATIC_DRAW);
    glViewport(0, 0, m_screenResolution.X, m_screenResolution.Y);
  }


  FractalShaderMandelbrot::~FractalShaderMandelbrot() = default;


  void FractalShaderMandelbrot::Update(const DemoTime& demoTime)
  {
    const GLuint hProgram = m_program.Get();

    Vector2 location;
    // mini mandelbrot
    location = Vector2(-1.749f, 0.0f);

    // Another mandelbrot
    // location = Vector2(-0.1592, -1.0317);

    // double scepter valley
    // location = Vector2(-0.1002, 0.8383);

    // Scepter variant
    // location = Vector2(-1.108, 0.230);

    // Quad-spiral valley
    // location = Vector2(0.274, 0.482);

    float tz = 0.5f - 0.5f * std::cos(0.225f * m_angle.X);
    // float scale = std::pow(0.5f, 7.0f * tz);
    float scale = std::pow(0.5f, 5.0f * tz);

    glUseProgram(hProgram);
    glUniform2f(m_locCoordinateOffset, location.X, location.Y);
    glUniform1f(m_locCoordinateScale, scale);

    m_angle.X += demoTime.DeltaTime;
  }


  void FractalShaderMandelbrot::Draw()
  {
    const GLuint hProgram = m_program.Get();

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(hProgram);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(m_vertexBuffer.GetTarget(), m_vertexBuffer.Get());
    m_vertexBuffer.EnableAttribArrays();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_vertexBuffer.DisableAttribArrays();

    glBindBuffer(m_vertexBuffer.GetTarget(), 0);
  }
}
