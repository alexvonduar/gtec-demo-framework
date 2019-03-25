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

#include <FslGraphics/Exceptions.hpp>
#include <FslGraphics/Vertices/VertexDeclaration.hpp>
#include <FslGraphics/Vertices/VertexElementFormatUtil.hpp>
#include <algorithm>
#include <cassert>
#include <limits>
#include <string>

namespace Fsl
{
  namespace
  {
    bool Contains(const std::vector<VertexElementEx>& elements, const VertexElementUsage usage, const uint32_t usageIndex, const uint32_t ignoreIndex)
    {
      assert(elements.size() <= std::numeric_limits<uint32_t>::max());
      const auto count = static_cast<uint32_t>(elements.size());

      for (uint32_t i = 0; i < count; ++i)
      {
        if (i != ignoreIndex && elements[i].Usage == usage && elements[i].UsageIndex == usageIndex)
        {
          return true;
        }
      }
      return false;
    }

    void VerifyElements(const std::vector<VertexElementEx>& elements, const uint32_t vertexStride)
    {
      // We expect the element offsets to be in order: smallest -> largest
      uint32_t maxOffset = 0;
      const auto count = uint32_t(elements.size());
      for (uint32_t i = 0; i < count; ++i)
      {
        auto offset = elements[i].Offset + VertexElementFormatUtil::GetBytesPerElement(elements[i].Format);
        if (offset < maxOffset)
        {
          throw NotSupportedException("The offsets should be in order: smallest -> largest");
        }

        maxOffset = offset;

        if (Contains(elements, elements[i].Usage, elements[i].UsageIndex, i))
        {
          throw NotSupportedException("Duplicated elements: A usage and usageIndex can only be listed once");
        }
      }
      // FIX: implement a proper validation check of the elements here

      if (maxOffset > vertexStride)
      {
        throw NotSupportedException("At least one of the elements is outside of the vertex stride");
      }
    }


    void CopyElements(std::vector<VertexElementEx>& rDstElements, const VertexElementEx* pSrcElement, const uint32_t elementCount)
    {
      assert(rDstElements.size() == std::size_t(elementCount));

      uint32_t maxOffset = 0;
      uint32_t dstIndex = 0;
      for (uint32_t i = 0; i < elementCount; ++i)
      {
        const auto currentOffset = pSrcElement[i].Offset;
        if (currentOffset >= maxOffset)
        {
          maxOffset = currentOffset;
          rDstElements[dstIndex] = pSrcElement[i];
        }
        else
        {
          // back track and make room until we find a offset that is smaller
          int tmpIndex = dstIndex - 1;
          while (tmpIndex >= 0 && currentOffset < rDstElements[tmpIndex].Offset)
          {
            rDstElements[tmpIndex + 1] = rDstElements[tmpIndex];
            --tmpIndex;
          }
          if (tmpIndex < 0)
          {
            tmpIndex = 0;
          }
          rDstElements[tmpIndex] = pSrcElement[i];
        }
        ++dstIndex;
      }
    }
  }

  // move assignment operator
  VertexDeclaration& VertexDeclaration::operator=(VertexDeclaration&& other) noexcept
  {
    if (this != &other)
    {
      // Claim ownership here
      m_elements = std::move(other.m_elements);
      m_vertexStride = other.m_vertexStride;

      // Remove the data from other
      other.m_vertexStride = 0;
    }
    return *this;
  }


  // Transfer ownership from other to this
  VertexDeclaration::VertexDeclaration(VertexDeclaration&& other) noexcept
    : m_elements(std::move(other.m_elements))
    , m_vertexStride(other.m_vertexStride)
  {
    // Remove the data from other
    other.m_vertexStride = 0;
  }


  VertexDeclaration::VertexDeclaration() = default;


  VertexDeclaration::VertexDeclaration(const VertexElementEx* const pElements, const std::size_t elementCount, const uint32_t vertexStride)
    : m_elements(elementCount)
    , m_vertexStride(vertexStride)
  {
    if (pElements == nullptr)
    {
      throw std::invalid_argument("pElements can not be null");
    }
    if (elementCount > std::numeric_limits<uint32_t>::max())
    {
      throw NotSupportedException("We only support 32bit of elements");
    }

    CopyElements(m_elements, pElements, static_cast<uint32_t>(elementCount));
    VerifyElements(m_elements, m_vertexStride);
  }


  void VertexDeclaration::Reset(const VertexElementEx* const pElements, const std::size_t elementCount, const uint32_t vertexStride)
  {
    if (pElements == nullptr)
    {
      throw std::invalid_argument("pElements can not be null");
    }
    if (elementCount > std::numeric_limits<uint32_t>::max())
    {
      throw NotSupportedException("We only support 32bit of elements");
    }

    m_elements.resize(elementCount);

    CopyElements(m_elements, pElements, static_cast<uint32_t>(elementCount));
    m_vertexStride = vertexStride;
    VerifyElements(m_elements, m_vertexStride);
  }


  uint32_t VertexDeclaration::Count() const
  {
    return static_cast<uint32_t>(m_elements.size());
  }


  const VertexElementEx* VertexDeclaration::DirectAccess() const
  {
    return m_elements.data();
  }


  int32_t VertexDeclaration::VertexElementGetIndexOf(const VertexElementUsage usage, const uint32_t usageIndex) const
  {
    const int32_t index = VertexElementIndexOf(usage, usageIndex);
    if (index < 0)
    {
      throw NotFoundException("Could not locate a vertex element of the requested type");
    }
    return index;
  }


  int32_t VertexDeclaration::VertexElementIndexOf(const VertexElementUsage usage, const uint32_t usageIndex) const
  {
    for (std::size_t i = 0; i < m_elements.size(); ++i)
    {
      if (m_elements[i].Usage == usage && m_elements[i].UsageIndex == usageIndex)
      {
        return static_cast<int32_t>(i);
      }
    }
    return -1;
  }


  VertexElementEx VertexDeclaration::VertexElementGet(const VertexElementUsage usage, const uint32_t usageIndex) const
  {
    for (auto element : m_elements)
    {
      if (element.Usage == usage && element.UsageIndex == usageIndex)
      {
        return element;
      }
    }
    throw NotFoundException("Could not locate a vertex element of the requested type");
  }


  bool VertexDeclaration::operator==(const VertexDeclaration& rhs) const
  {
    if (m_vertexStride != rhs.m_vertexStride || m_elements.size() != rhs.m_elements.size())
    {
      return false;
    }

    for (std::size_t i = 0; i < m_elements.size(); ++i)
    {
      if (m_elements[i] != rhs.m_elements[i])
      {
        return false;
      }
    }
    return true;
  }


  bool VertexDeclaration::operator!=(const VertexDeclaration& rhs) const
  {
    return !(*this == rhs);
  }
}
