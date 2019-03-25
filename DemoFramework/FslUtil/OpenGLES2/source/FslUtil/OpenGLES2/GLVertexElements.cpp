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

#include <FslUtil/OpenGLES2/GLVertexElements.hpp>
#include <FslUtil/OpenGLES2/Exceptions.hpp>
#include <FslUtil/OpenGLES2/GLCheck.hpp>
#include <FslBase/Log/Log.hpp>
#include <algorithm>
#include <cassert>
#include <limits>

namespace Fsl
{
  namespace GLES2
  {
    namespace
    {
      std::size_t CountMatrixElements(const VertexDeclaration& vertexDeclaration)
      {
        std::size_t count = 0;
        const std::size_t elementCount = vertexDeclaration.Count();
        for (std::size_t i = 0; i < elementCount; ++i)
        {
          count += (vertexDeclaration.At(i).Format == VertexElementFormat::Matrix4x4) ? 1 : 0;
        }
        return count;
      }


      // Convert from the platform independent vertex declaration to a GL one
      void ConvertToNative(std::vector<GLVertexElement>& rVertexElements, const VertexDeclaration& vertexDeclaration)
      {
        assert(vertexDeclaration.Count() <= std::numeric_limits<uint32_t>::max());
        const auto numMatrixElements = CountMatrixElements(vertexDeclaration);

        const auto convertedElementCount = (vertexDeclaration.Count() - numMatrixElements) + (numMatrixElements * 4);

        if (convertedElementCount != rVertexElements.size())
        {
          rVertexElements.resize(convertedElementCount);
        }

        const auto srcCount = vertexDeclaration.Count();
        const VertexElementEx* pSrcElements = vertexDeclaration.DirectAccess();
        const VertexElementEx* const pSrcElementsEnd = pSrcElements + srcCount;
        GLVertexElement* pDst = rVertexElements.data();

        auto extendedIndex = srcCount;
        GLVertexElement* pDstAfterNormalData = rVertexElements.data() + extendedIndex;

        while (pSrcElements < pSrcElementsEnd)
        {
          if (pSrcElements->Format != VertexElementFormat::Matrix4x4)
          {
            pDst->Reset(*pSrcElements);
            ++pDst;
          }
          else
          {
            // Expand Matrix4x4 elements to the OpenGL ES representation
            pDst->Reset(*pSrcElements, VertexElementFormat::Vector4, 0);
            pDst->ExtendedIndex = extendedIndex;
            ++pDst;
            extendedIndex += 3;

            // We store the extended attributes after all the normal ones to ensure that the index between the VertexDeclation and our internal ones
            // are the same
            pDstAfterNormalData->Reset(*pSrcElements, VertexElementFormat::Vector4, sizeof(float) * 4);
            ++pDstAfterNormalData;
            pDstAfterNormalData->Reset(*pSrcElements, VertexElementFormat::Vector4, sizeof(float) * 4 * 2);
            ++pDstAfterNormalData;
            pDstAfterNormalData->Reset(*pSrcElements, VertexElementFormat::Vector4, sizeof(float) * 4 * 3);
            ++pDstAfterNormalData;
          }
          ++pSrcElements;
        }
      }


      bool IsEqualTo(const VertexDeclaration& vertexDeclaration, const std::vector<GLVertexElement>& vertexElements,
                     const std::size_t originalVertexElementCount)
      {
        if (vertexDeclaration.Count() != originalVertexElementCount)
        {
          return false;
        }

        const auto srcCount = vertexDeclaration.Count();
        const VertexElementEx* pSrcElements = vertexDeclaration.DirectAccess();
        const VertexElementEx* const pSrcElementsEnd = pSrcElements + srcCount;
        const GLVertexElement* pDst = vertexElements.data();

        while (pSrcElements < pSrcElementsEnd)
        {
          if (pDst->Source != *pSrcElements)
          {
            return false;
          }
          ++pSrcElements;
          ++pDst;
        }
        return true;
      }


      inline void EnableAttribArray(const std::vector<GLVertexElement>& vertexElement, const std::size_t elementIndex, const GLuint attribIndex,
                                    const uint32_t vertexStride)
      {
        assert(elementIndex < vertexElement.size());

        glVertexAttribPointer(attribIndex, vertexElement[elementIndex].Size, vertexElement[elementIndex].Type, vertexElement[elementIndex].Normalized,
                              vertexStride, vertexElement[elementIndex].Pointer);
        glEnableVertexAttribArray(attribIndex);
        // if this is a matrix element we need to enable the rest of the components
        if (vertexElement[elementIndex].Source.Format == VertexElementFormat::Matrix4x4)
        {
          auto extendedIndex = vertexElement[elementIndex].ExtendedIndex;
          glVertexAttribPointer(attribIndex + 1, vertexElement[extendedIndex].Size, vertexElement[extendedIndex].Type,
                                vertexElement[extendedIndex].Normalized, vertexStride, vertexElement[extendedIndex].Pointer);
          glEnableVertexAttribArray(attribIndex + 1);
          ++extendedIndex;
          glVertexAttribPointer(attribIndex + 2, vertexElement[extendedIndex].Size, vertexElement[extendedIndex].Type,
                                vertexElement[extendedIndex].Normalized, vertexStride, vertexElement[extendedIndex].Pointer);
          glEnableVertexAttribArray(attribIndex + 2);
          ++extendedIndex;
          glVertexAttribPointer(attribIndex + 3, vertexElement[extendedIndex].Size, vertexElement[extendedIndex].Type,
                                vertexElement[extendedIndex].Normalized, vertexStride, vertexElement[extendedIndex].Pointer);
          glEnableVertexAttribArray(attribIndex + 3);
        }
      }


      inline void DisableAttribArray(const std::vector<GLVertexElement>& vertexElement, const uint32_t elementIndex, const GLuint attribIndex)
      {
        assert(elementIndex < vertexElement.size());

        glDisableVertexAttribArray(attribIndex);

        // if this is a matrix element we need to enable the rest of the components
        if (vertexElement[elementIndex].Source.Format == VertexElementFormat::Matrix4x4)
        {
          glDisableVertexAttribArray(attribIndex + 1);
          glDisableVertexAttribArray(attribIndex + 2);
          glDisableVertexAttribArray(attribIndex + 3);
        }
      }

      inline void EnableAttribPointer(const std::vector<GLVertexElement>& vertexElement, const uint32_t elementIndex, const GLuint attribIndex,
                                      const uint32_t vertexStride)
      {
        assert(elementIndex < vertexElement.size());

        glVertexAttribPointer(attribIndex, vertexElement[elementIndex].Size, vertexElement[elementIndex].Type, vertexElement[elementIndex].Normalized,
                              vertexStride, vertexElement[elementIndex].Pointer);
        // if this is a matrix element we need to enable the rest of the components
        if (vertexElement[elementIndex].Source.Format == VertexElementFormat::Matrix4x4)
        {
          auto extendedIndex = vertexElement[elementIndex].ExtendedIndex;
          glVertexAttribPointer(attribIndex + 1, vertexElement[extendedIndex].Size, vertexElement[extendedIndex].Type,
                                vertexElement[extendedIndex].Normalized, vertexStride, vertexElement[extendedIndex].Pointer);
          ++extendedIndex;
          glVertexAttribPointer(attribIndex + 2, vertexElement[extendedIndex].Size, vertexElement[extendedIndex].Type,
                                vertexElement[extendedIndex].Normalized, vertexStride, vertexElement[extendedIndex].Pointer);
          ++extendedIndex;
          glVertexAttribPointer(attribIndex + 3, vertexElement[extendedIndex].Size, vertexElement[extendedIndex].Type,
                                vertexElement[extendedIndex].Normalized, vertexStride, vertexElement[extendedIndex].Pointer);
        }
      }
    }


    GLVertexElements::GLVertexElements(const VertexDeclaration& vertexDeclaration)
      : m_originalVertexElementCount(vertexDeclaration.Count())
      , m_vertexElementStride(vertexDeclaration.VertexStride())
    {
      ConvertToNative(m_vertexElements, vertexDeclaration);
    }


    void GLVertexElements::Reset(const VertexDeclaration& vertexDeclaration)
    {
      m_originalVertexElementCount = vertexDeclaration.Count();
      m_vertexElementStride = vertexDeclaration.VertexStride();

      ConvertToNative(m_vertexElements, vertexDeclaration);
    }

    bool GLVertexElements::IsEqual(const VertexDeclaration& vertexDeclaration) const
    {
      return IsEqualTo(vertexDeclaration, m_vertexElements, m_originalVertexElementCount);
    }


    void GLVertexElements::EnableAttribArrays() const
    {
      const auto vertexStride = VertexStride();
      const GLVertexElement* pVertexElements = m_vertexElements.data();
      const GLVertexElement* const pVertexElementsEnd = pVertexElements + m_vertexElements.size();

      GLuint attribIndex = 0;
      while (pVertexElements < pVertexElementsEnd)
      {
        // We dont need special handling of the 'extended' elements here since we enable everything
        glVertexAttribPointer(attribIndex, pVertexElements->Size, pVertexElements->Type, pVertexElements->Normalized, vertexStride,
                              pVertexElements->Pointer);
        glEnableVertexAttribArray(attribIndex);
        ++pVertexElements;
        ++attribIndex;
      }
    }


    void GLVertexElements::DisableAttribArrays() const
    {
      const auto count = static_cast<GLuint>(m_vertexElements.size());
      GLuint index = 0;
      while (index < count)
      {
        // We don't need special handling of the 'extended' elements here since we disable everything
        glDisableVertexAttribArray(index);
        ++index;
      }
    }


    void GLVertexElements::EnableAttribArrays(const GLuint* const pAttributeIndices, const std::size_t count) const
    {
      if (pAttributeIndices == nullptr)
      {
        throw std::invalid_argument("pAttributeIndices can not be null");
      }
      if (count > std::numeric_limits<uint32_t>::max())
      {
        throw NotSupportedException("We only support 32bit of elements");
      }
      FSLLOG_WARNING_IF(count > m_originalVertexElementCount, "Attribute indices is larger than the vertex element count, extra elements ignored!");

      const auto vertexStride = VertexStride();

      const uint32_t elementCount = std::min(m_originalVertexElementCount, static_cast<uint32_t>(count));
      for (uint32_t elementIndex = 0; elementIndex < elementCount; ++elementIndex)
      {
        EnableAttribArray(m_vertexElements, elementIndex, pAttributeIndices[elementIndex], vertexStride);
        ++elementIndex;
      }
    }


    void GLVertexElements::DisableAttribArrays(const GLuint* const pAttributeIndices, const std::size_t count) const
    {
      if (pAttributeIndices == nullptr)
      {
        throw std::invalid_argument("pAttributeIndices can not be null");
      }
      if (count > std::numeric_limits<uint32_t>::max())
      {
        throw NotSupportedException("We only support 32bit of elements");
      }
      FSLLOG_WARNING_IF(count > m_originalVertexElementCount, "Attribute indices is larger than the vertex element count, extra elements ignored!");

      const uint32_t elementCount = std::min(m_originalVertexElementCount, static_cast<uint32_t>(count));
      for (uint32_t elementIndex = 0; elementIndex < elementCount; ++elementIndex)
      {
        DisableAttribArray(m_vertexElements, elementIndex, pAttributeIndices[elementIndex]);
      }
    }


    void GLVertexElements::EnableAttribArrays(const GLVertexAttribLink* const pLinks, const std::size_t count) const
    {
      if (pLinks == nullptr)
      {
        throw std::invalid_argument("pLinks can not be null");
      }
      if (count > std::numeric_limits<uint32_t>::max())
      {
        throw NotSupportedException("We only support 32bit of elements");
      }

      const auto vertexStride = VertexStride();
      assert(count <= std::numeric_limits<uint32_t>::max());
      for (uint32_t i = 0; i < static_cast<uint32_t>(count); ++i)
      {
        if (pLinks[i].AttribIndex >= 0)
        {
          EnableAttribArray(m_vertexElements, pLinks[i].VertexElementIndex, pLinks[i].AttribIndex, vertexStride);
        }
      }
    }


    void GLVertexElements::DisableAttribArrays(const GLVertexAttribLink* const pLinks, const std::size_t count) const
    {
      if (pLinks == nullptr)
      {
        throw std::invalid_argument("pLinks can not be null");
      }
      if (count > std::numeric_limits<uint32_t>::max())
      {
        throw NotSupportedException("We only support 32bit of elements");
      }

      for (uint32_t i = 0; i < static_cast<uint32_t>(count); ++i)
      {
        if (pLinks[i].AttribIndex >= 0)
        {
          DisableAttribArray(m_vertexElements, pLinks[i].VertexElementIndex, pLinks[i].AttribIndex);
        }
      }
    }


    void GLVertexElements::SetVertexAttribPointers() const
    {
      const auto vertexStride = VertexStride();
      const GLVertexElement* pVertexElements = m_vertexElements.data();
      const GLVertexElement* const pVertexElementsEnd = pVertexElements + m_vertexElements.size();

      GLuint attribIndex = 0;
      while (pVertexElements < pVertexElementsEnd)
      {
        // We dont need special handling of the 'extended' elements here since we enable everything
        glVertexAttribPointer(attribIndex, pVertexElements->Size, pVertexElements->Type, pVertexElements->Normalized, vertexStride,
                              pVertexElements->Pointer);
        ++pVertexElements;
        ++attribIndex;
      }
    }


    void GLVertexElements::SetVertexAttribPointers(const GLuint* const pAttributeIndices, const std::size_t count) const
    {
      if (count > std::numeric_limits<uint32_t>::max())
      {
        throw NotSupportedException("We only support 32bit of elements");
      }

      FSLLOG_WARNING_IF(count > m_originalVertexElementCount, "Attribute indices is larger than the vertex element count, extra elements ignored!");

      const auto vertexStride = VertexStride();

      const uint32_t elementCount = std::min(m_originalVertexElementCount, static_cast<uint32_t>(count));
      for (uint32_t elementIndex = 0; elementIndex < elementCount; ++elementIndex)
      {
        EnableAttribPointer(m_vertexElements, elementIndex, pAttributeIndices[elementIndex], vertexStride);
        ++elementIndex;
      }
    }


    void GLVertexElements::SetVertexAttribPointers(const GLVertexAttribLink* const pLinks, const std::size_t count) const
    {
      if (count > std::numeric_limits<uint32_t>::max())
      {
        throw NotSupportedException("We only support 32bit of elements");
      }

      const auto vertexStride = VertexStride();
      for (uint32_t i = 0; i < static_cast<uint32_t>(count); ++i)
      {
        if (pLinks[i].AttribIndex >= 0)
        {
          EnableAttribPointer(m_vertexElements, pLinks[i].VertexElementIndex, pLinks[i].AttribIndex, vertexStride);
        }
      }
    }


    int32_t GLVertexElements::GetVertexElementIndex(const VertexElementUsage usage, const uint32_t usageIndex) const
    {
      const auto index = VertexElementIndexOf(usage, usageIndex);
      if (index < 0)
      {
        throw UsageErrorException("The supplied usage and usageIndex combo was not found");
      }
      return index;
    }


    int32_t GLVertexElements::VertexElementIndexOf(const VertexElementUsage usage, const uint32_t usageIndex) const
    {
      assert(m_vertexElements.size() <= static_cast<uint32_t>(std::numeric_limits<int32_t>::max()));
      for (std::size_t i = 0; i < m_vertexElements.size(); ++i)
      {
        if (m_vertexElements[i].Source.Usage == usage && m_vertexElements[i].Source.UsageIndex == usageIndex)
        {
          return static_cast<int32_t>(i);
        }
      }
      return -1;
    }
  }
}
