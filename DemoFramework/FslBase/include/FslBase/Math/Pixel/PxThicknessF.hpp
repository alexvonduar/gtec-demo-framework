#ifndef FSLBASE_MATH_PIXEL_PXTHICKNESSF_HPP
#define FSLBASE_MATH_PIXEL_PXTHICKNESSF_HPP
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
#include <FslBase/Math/Pixel/PxSize2DF.hpp>
#include <FslBase/Math/Pixel/PxVector2.hpp>
#include <FslBase/OptimizationFlag.hpp>
#include <algorithm>
#include <cassert>

namespace Fsl
{
  //! A PxThickness can not contain negative values
  struct PxThicknessF
  {
    using value_type = float;

  private:
    value_type m_left{};
    value_type m_top{};
    value_type m_right{};
    value_type m_bottom{};

  public:
    constexpr PxThicknessF() noexcept = default;

    constexpr PxThicknessF(const value_type left, const value_type top, const value_type right, const value_type bottom) noexcept
      : m_left(left >= 0.0f ? left : 0.0f)
      , m_top(top >= 0.0f ? top : 0.0f)
      , m_right(right >= 0.0f ? right : 0.0f)
      , m_bottom(bottom >= 0.0f ? bottom : 0.0f)
    {
      assert(IsValid());
    }

    constexpr PxThicknessF(const value_type left, const value_type top, const value_type right, const value_type bottom,
                           const OptimizationCheckFlag /*unused*/) noexcept
      : m_left(left)
      , m_top(top)
      , m_right(right)
      , m_bottom(bottom)
    {
      assert(IsValid());
    }

    constexpr value_type Left() const noexcept
    {
      return m_left;
    }

    constexpr value_type Top() const noexcept
    {
      return m_top;
    }

    constexpr value_type Right() const noexcept
    {
      return m_right;
    }

    constexpr value_type Bottom() const noexcept
    {
      return m_bottom;
    }

    constexpr PxVector2 TopLeft() const
    {
      return {m_left, m_top};
    }

    constexpr PxVector2 TopRight() const
    {
      return {m_right, m_top};
    }

    constexpr PxVector2 BottomLeft() const
    {
      return {m_left, m_bottom};
    }

    constexpr PxVector2 BottomRight() const
    {
      return {m_right, m_bottom};
    }

    constexpr PxSize2DF Sum() const noexcept
    {
      assert(m_left >= 0 && m_right >= 0);
      assert(m_top >= 0 && m_bottom >= 0);
      return {m_left + m_right, m_top + m_bottom, OptimizationCheckFlag::NoCheck};
    }

    constexpr value_type SumX() const noexcept
    {
      assert(m_left >= 0 && m_right >= 0);
      return m_left + m_right;
    }

    constexpr value_type SumY() const noexcept
    {
      assert(m_top >= 0 && m_bottom >= 0);
      return m_top + m_bottom;
    }

    constexpr bool IsValid() const
    {
      return m_left >= 0 && m_right >= 0 && m_top >= 0 && m_bottom >= 0;
    }

    constexpr bool operator==(const PxThicknessF& rhs) const noexcept
    {
      return m_left == rhs.m_left && m_top == rhs.m_top && m_right == rhs.m_right && m_bottom == rhs.m_bottom;
    }
    constexpr bool operator!=(const PxThicknessF& rhs) const noexcept
    {
      return !(*this == rhs);
    }

    static constexpr PxThicknessF Min(const PxThicknessF& val0, const PxThicknessF& val1)
    {
      return {std::min(val0.Left(), val1.Left()), std::min(val0.Top(), val1.Top()), std::min(val0.Right(), val1.Right()),
              std::min(val0.Bottom(), val1.Bottom()), OptimizationCheckFlag::NoCheck};
    }

    static constexpr PxThicknessF Max(const PxThicknessF& val0, const PxThicknessF& val1)
    {
      return {std::max(val0.Left(), val1.Left()), std::max(val0.Top(), val1.Top()), std::max(val0.Right(), val1.Right()),
              std::max(val0.Bottom(), val1.Bottom()), OptimizationCheckFlag::NoCheck};
    }
  };
}


#endif
