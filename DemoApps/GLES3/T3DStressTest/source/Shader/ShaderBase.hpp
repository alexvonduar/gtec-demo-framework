#ifndef SHADER_GLES3_T3DSTRESSTEST_SHADERBASE_HPP
#define SHADER_GLES3_T3DSTRESSTEST_SHADERBASE_HPP
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

#include <FslBase/Math/Matrix.hpp>
#include <FslBase/Math/Vector3.hpp>
#include <FslUtil/OpenGLES3/GLProgram.hpp>
#include <GLES3/gl3.h>
#include <string>
#include "ShaderVertexConfig.hpp"

namespace Fsl
{
  class ShaderBase
  {
    GLES3::GLProgram m_program;
    bool m_isLoaded;

  public:
    class ScopedUse
    {
      ShaderBase& m_shader;

    public:
      ScopedUse(const ScopedUse&) = delete;
      ScopedUse& operator=(const ScopedUse&) = delete;

      explicit ScopedUse(ShaderBase& shader)
        : m_shader(shader)
      {
        m_shader.Load();
      }
      ~ScopedUse()
      {
        m_shader.Unload();
      }
    };
    ShaderBase(const std::string& strVertexShader, const std::string& strFragmentShader);
    virtual ~ShaderBase();

    ShaderVertexConfig GetShaderConfig() const;

  protected:
    ShaderVertexConfig m_shaderConfig;


    bool IsValid() const;
    GLuint Get() const;
    const GLES3::GLProgram& GetProgram() const
    {
      return m_program;
    }
    void Load();
    void Unload();
    bool IsLoaded() const
    {
      return m_isLoaded;
    }
  };
}

#endif
