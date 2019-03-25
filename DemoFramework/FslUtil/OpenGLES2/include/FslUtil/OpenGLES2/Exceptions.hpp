#ifndef FSLUTIL_OPENGLES2_EXCEPTIONS_HPP
#define FSLUTIL_OPENGLES2_EXCEPTIONS_HPP
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

// Make sure Common.hpp is the first include file (to make the error message as helpful as possible when disabled)
#include <FslUtil/OpenGLES2/Common.hpp>
#include <FslGraphics/Exceptions.hpp>
#include <utility>

namespace Fsl
{
  namespace GLES2
  {
    class GLESGraphicsException : public GraphicsException
    {
      int m_error{};
      std::string m_filename;
      int m_lineNumber;

    public:
      explicit GLESGraphicsException(const std::string& str)
        : GraphicsException(str)
        , m_lineNumber(0)
      {
      }

      GLESGraphicsException(const std::string& str, int error)
        : GraphicsException(str)
        , m_error(error)
        , m_lineNumber(0)
      {
      }

      GLESGraphicsException(const std::string& str, int error, const char* const pszFilename, const int line)
        : GraphicsException(str)
        , m_error(error)
        , m_filename(pszFilename)
        , m_lineNumber(line)
      {
      }

      GLESGraphicsException(const std::string& str, int error, std::string filename, const int line)
        : GraphicsException(str)
        , m_error(error)
        , m_filename(std::move(filename))
        , m_lineNumber(line)
      {
      }

      ~GLESGraphicsException() noexcept override = default;

      int GetError() const
      {
        return m_error;
      }


      const std::string& GetFilename() const
      {
        return m_filename;
      }

      int GetLineNumber() const
      {
        return m_lineNumber;
      }
    };
  }
}

#endif
