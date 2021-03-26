#ifndef FSLGRAPHICS_SPRITE_MATERIAL_SPRITEMATERIALID_HPP
#define FSLGRAPHICS_SPRITE_MATERIAL_SPRITEMATERIALID_HPP
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

namespace Fsl
{
  struct SpriteMaterialId
  {
    uint32_t Value{};

    constexpr SpriteMaterialId() noexcept = default;

    constexpr explicit SpriteMaterialId(const uint32_t value) noexcept
      : Value(value)
    {
    }

    constexpr bool operator==(const SpriteMaterialId& rhs) const noexcept
    {
      return Value == rhs.Value;
    }

    constexpr bool operator!=(const SpriteMaterialId& rhs) const noexcept
    {
      return !(*this == rhs);
    }
  };


  // Operator <
  constexpr inline bool operator<(const SpriteMaterialId& lhs, const SpriteMaterialId& rhs) noexcept
  {
    return lhs.Value < rhs.Value;
  }

  // Operator <=
  constexpr inline bool operator<=(const SpriteMaterialId& lhs, const SpriteMaterialId& rhs) noexcept
  {
    return lhs.Value <= rhs.Value;
  }

  // Operator >
  constexpr inline bool operator>(const SpriteMaterialId& lhs, const SpriteMaterialId& rhs) noexcept
  {
    return lhs.Value > rhs.Value;
  }

  // Operator >=
  constexpr inline bool operator>=(const SpriteMaterialId& lhs, const SpriteMaterialId& rhs) noexcept
  {
    return lhs.Value >= rhs.Value;
  }

  inline constexpr SpriteMaterialId operator+(const SpriteMaterialId& lhs, const uint32_t& rhs) noexcept
  {
    return SpriteMaterialId(lhs.Value + rhs);
  }

  inline constexpr SpriteMaterialId operator+(const uint32_t& lhs, const SpriteMaterialId& rhs) noexcept
  {
    return SpriteMaterialId(lhs + rhs.Value);
  }

}

#endif
