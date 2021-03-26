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

#include <FslBase/IO/File.hpp>
#include <FslUtil/OpenGLES3/GLCheck.hpp>
#include "ShaderBase.hpp"
#include <algorithm>
#include <cassert>

namespace Fsl
{
  ShaderBase::ShaderBase(const std::string& strVertexShader, const std::string& strFragmentShader)
    : m_program(strVertexShader, strFragmentShader)
    , m_isLoaded(false)
  {
  }


  ShaderBase::~ShaderBase() = default;


  ShaderVertexConfig ShaderBase::GetShaderConfig() const
  {
    return m_shaderConfig;
  }

  bool ShaderBase::IsValid() const
  {
    return m_program.IsValid();
  }


  GLuint ShaderBase::Get() const
  {
    return m_program.Get();
  }


  void ShaderBase::Load()
  {
    assert(!m_isLoaded);
    m_isLoaded = true;
    glUseProgram(m_program.Get());
  }


  void ShaderBase::Unload()
  {
    assert(m_isLoaded);
    glUseProgram(0);
    m_isLoaded = false;
  }
}
