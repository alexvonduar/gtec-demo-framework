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

#include <FslUtil/OpenGLES3/Draw/GLLineDraw.hpp>
#include <FslUtil/OpenGLES3/GLCheck.hpp>
#include <FslBase/Math/Vector3.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslGraphics/Exceptions.hpp>
#include <algorithm>
#include <cassert>
#include <limits>
#include <type_traits>

namespace Fsl
{
  namespace GLES3
  {
    GLLineDraw::GLLineDraw(const uint32_t initialLineCapacity)
      : LineVertexBuffer(nullptr, initialLineCapacity * 2u, VertexPositionColor::GetVertexDeclaration(), GL_DYNAMIC_DRAW)
    {
    }


    void GLLineDraw::Draw(const VertexPositionColor* pVertices, const std::size_t vertexCount, const GLVertexAttribLink* const pLinks,
                          const std::size_t linkCount)
    {
      if (pVertices == nullptr)
      {
        throw std::invalid_argument("pVertices can not be null");
      }
      if ((vertexCount % VERTICES_PER_LINE) != 0u)
      {
        throw std::invalid_argument("vertexCount did not match a exact number of lines");
      }
      if (pLinks == nullptr)
      {
        throw std::invalid_argument("pLinks can not be null");
      }

      if (vertexCount > LineVertexBuffer.GetCapacity())
      {
        std::size_t newCapacity = vertexCount;
        // Warn the developer in debug mode
        FSLLOG3_DEBUG_WARNING("Performance-issue: Resizing LineVertexBuffer from: {} to {}", LineVertexBuffer.GetCapacity(), newCapacity);
        // Dump the existing buffer and then create a new one
        LineVertexBuffer.Reset();
        LineVertexBuffer.Reset(nullptr, newCapacity, VertexPositionColor::GetVertexDeclaration(), GL_DYNAMIC_DRAW);
      }

      // Enable the attrib arrays
      LineVertexBuffer.EnableAttribArrays(pLinks, linkCount);

      glBindBuffer(GL_ARRAY_BUFFER, LineVertexBuffer.Get());
      LineVertexBuffer.SetVertexAttribPointers(pLinks, linkCount);
      LineVertexBuffer.SetDataFast(0, pVertices, vertexCount);

      // verify that the cast below goes ok
      assert(vertexCount <= static_cast<std::make_unsigned<GLsizei>::type>(std::numeric_limits<GLsizei>::max()));
      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertexCount));

      glBindBuffer(GL_ARRAY_BUFFER, 0u);
    }
  }
}
