#ifndef FSLBASE_MATH_PIXEL_PXSIZE2D_HPP
#define FSLBASE_MATH_PIXEL_PXSIZE2D_HPP
/****************************************************************************************************************************************************
 * Copyright 2020 NXP
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
#include <FslBase/OptimizationFlag.hpp>
#include <algorithm>
#include <cassert>
#include <limits>

namespace Fsl
{
  //! A immutable int32_t based size that will never be negative!
  struct PxSize2D
  {
    using value_type = int32_t;

  private:
    value_type m_width{};
    value_type m_height{};

  public:
    constexpr PxSize2D() noexcept = default;

    constexpr PxSize2D(const value_type width, const value_type height) noexcept
      : m_width(width >= 0 ? width : 0)
      , m_height(height >= 0 ? height : 0)
    {
    }

    //! If this constructor is used is extremely important to be 100% sure the width and height are positive.
    constexpr PxSize2D(const value_type width, const value_type height, const OptimizationCheckFlag /*unused*/) noexcept
      : m_width(width)
      , m_height(height)
    {
      assert(width >= 0);
      assert(height >= 0);
    }

    constexpr inline value_type Width() const
    {
      return m_width;
    }

    constexpr inline value_type Height() const
    {
      return m_height;
    }

    constexpr inline void SetWidth(const value_type width)
    {
      m_width = std::max(width, 0);
    }

    constexpr inline void SetHeight(const value_type height)
    {
      m_height = std::max(height, 0);
    }

    constexpr inline void SetWidth(const value_type width, const OptimizationCheckFlag /*unused*/)
    {
      assert(width >= 0);
      m_width = width;
    }

    constexpr inline void SetHeight(const value_type height, const OptimizationCheckFlag /*unused*/)
    {
      assert(height >= 0);
      m_height = height;
    }

    constexpr inline void AddWidth(const value_type width)
    {
      m_width = std::max(m_width + width, 0);
    }

    constexpr inline void AddHeight(const value_type height)
    {
      m_height = std::max(m_height + height, 0);
    }

    constexpr inline void SetMax(const PxSize2D& value)
    {
      m_width = std::max(m_width, value.m_width);
      m_height = std::max(m_height, value.m_height);
    }


    constexpr bool operator==(const PxSize2D& rhs) const noexcept
    {
      return m_width == rhs.m_width && m_height == rhs.m_height;
    }

    constexpr bool operator!=(const PxSize2D& rhs) const noexcept
    {
      return m_width != rhs.m_width || m_height != rhs.m_height;
    }

    static constexpr PxSize2D Add(const PxSize2D& sizePx, const PxSize2D& valuePx)
    {
      // check for overflow
      assert(sizePx.m_width <= (std::numeric_limits<value_type>::max() - valuePx.m_width));
      assert(sizePx.m_height <= (std::numeric_limits<value_type>::max() - valuePx.m_height));
      return {sizePx.m_width + valuePx.m_width, sizePx.m_height + valuePx.m_height, OptimizationCheckFlag::NoCheck};
    }

    static constexpr PxSize2D Subtract(const PxSize2D& sizePx, const PxSize2D& valuePx)
    {
      return {std::max(sizePx.m_width - valuePx.m_width, 0), std::max(sizePx.m_height - valuePx.m_height, 0), OptimizationCheckFlag::NoCheck};
    }

    static constexpr PxSize2D Divide(const PxSize2D& sizePx, const value_type value)
    {
      assert(value != 0);
      return {sizePx.m_width / value, sizePx.m_height / value, OptimizationCheckFlag::NoCheck};
    }

    constexpr PxSize2D& operator+=(const PxSize2D& valuePx) noexcept
    {
      assert(m_width <= (std::numeric_limits<value_type>::max() - valuePx.m_width));
      assert(m_height <= (std::numeric_limits<value_type>::max() - valuePx.m_height));
      m_width += valuePx.m_width;
      m_height += valuePx.m_height;
      return *this;
    }

    constexpr PxSize2D& operator-=(const PxSize2D& valuePx) noexcept
    {
      m_width = std::max(m_width - valuePx.m_width, 0);
      m_height = std::max(m_height - valuePx.m_height, 0);
      return *this;
    }

    static constexpr PxSize2D Min(const PxSize2D& val0, const PxSize2D& val1)
    {
      return {std::min(val0.m_width, val1.m_width), std::min(val0.m_height, val1.m_height)};
    }

    static constexpr PxSize2D Max(const PxSize2D& val0, const PxSize2D& val1)
    {
      return {std::max(val0.m_width, val1.m_width), std::max(val0.m_height, val1.m_height)};
    }
  };

  inline constexpr PxSize2D operator+(const PxSize2D& lhs, const PxSize2D& rhs) noexcept
  {
    return PxSize2D::Add(lhs, rhs);
  }

  inline constexpr PxSize2D operator-(const PxSize2D& lhs, const PxSize2D& rhs) noexcept
  {
    return PxSize2D::Subtract(lhs, rhs);
  }

  inline constexpr PxSize2D operator/(const PxSize2D& lhs, const PxSize2D::value_type rhs) noexcept
  {
    return PxSize2D::Divide(lhs, rhs);
  }
}

#endif
