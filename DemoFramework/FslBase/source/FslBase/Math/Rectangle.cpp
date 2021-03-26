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

#include <FslBase/Math/Rectangle.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Log/Log3Core.hpp>
#include <algorithm>

namespace Fsl
{
  Rectangle::Rectangle(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
    : m_x(x)
    , m_y(y)
    , m_width(std::max(width, static_cast<int32_t>(0)))
    , m_height(std::max(height, static_cast<int32_t>(0)))
  {
    FSLLOG3_WARNING_IF(width < 0, "Tried to set width to less than zero");
    FSLLOG3_WARNING_IF(height < 0, "Tried to set height to less than zero");
  }


  Rectangle Rectangle::FromLeftTopRigtBottom(const int32_t left, const int32_t top, const int32_t right, const int32_t bottom)
  {
    FSLLOG3_WARNING_IF((right - left) < 0, "Tried to set width to less than zero");
    FSLLOG3_WARNING_IF((bottom - top) < 0, "Tried to set height to less than zero");
    return {left, top, std::max(right - left, static_cast<int32_t>(0)), std::max(bottom - top, static_cast<int32_t>(0))};
  }


  void Rectangle::SetWidth(const int32_t value)
  {
    if (value < 0)
    {
      m_width = 0;
      FSLLOG3_WARNING("Tried to set width to less than zero");
    }
    else
    {
      m_width = value;
    }
  }


  void Rectangle::SetHeight(const int32_t value)
  {
    if (value < 0)
    {
      m_height = 0;
      FSLLOG3_WARNING("Tried to set height to less than zero");
    }
    else
    {
      m_height = value;
    }
  }
}
