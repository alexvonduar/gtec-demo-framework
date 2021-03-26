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

#include <FslBase/Math/MathHelper_CalcOptimalSize.hpp>
#include <FslBase/Math/MathHelper.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Math/Pixel/TypeConverter_Math.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>

namespace Fsl
{
  namespace MathHelper
  {
    namespace
    {
      inline PxExtent2D CalcOptimalSizePow2(const uint32_t totalArea, const PxExtent2D& unitSize, const uint32_t unitCount)
      {
        auto newSize = MathHelper::ToPowerOfTwo(static_cast<uint32_t>(std::ceil(::sqrt(totalArea))));

        const auto unitsX = newSize / unitSize.Width;
        const auto unitsY = newSize / unitSize.Height;

        auto newSizeY = newSize;
        if ((unitsX * unitsY) < unitCount)
        {
          newSize = MathHelper::ToPowerOfTwo((unitsX + 1) * unitSize.Width);
        }
        else if (((unitsX * unitsY) / 2) >= unitCount)
        {
          newSizeY /= 2;
        }
        return {newSize, newSizeY};
      }


      inline PxExtent2D CalcOptimalSizeSquare(const uint32_t totalArea, const PxExtent2D& unitSize, const uint32_t unitCount)
      {
        auto newSize = static_cast<uint32_t>(std::ceil(std::sqrt(totalArea)));

        const uint32_t unitsX = newSize / unitSize.Width;
        const uint32_t unitsY = newSize / unitSize.Height;
        if ((unitsX * unitsY) < unitCount)
        {
          uint32_t result1 = (unitsX + 1) * unitSize.Width;
          uint32_t result2 = (unitsY + 1) * unitSize.Height;
          uint32_t result = std::min(result1, result2);
          return {result, result};
        }

        uint32_t result = unitsX * unitSize.Width;
        return {result, result};
      }


      inline PxExtent2D CalcOptimalSizeSquarePow2(const uint32_t totalArea, const PxExtent2D& unitSize, const uint32_t unitCount)
      {
        auto newSize = MathHelper::ToPowerOfTwo(static_cast<uint32_t>(std::ceil(std::sqrt(totalArea))));

        const uint32_t unitsX = newSize / unitSize.Width;
        const uint32_t unitsY = newSize / unitSize.Height;
        if ((unitsX * unitsY) < unitCount)
        {
          newSize = MathHelper::ToPowerOfTwo((unitsX + 1) * unitSize.Width);
        }
        return {newSize, newSize};
      }

      float WrapMax(const float value, const float max)
      {
        return std::fmod(max + std::fmod(value, max), max);
      }
    }

    float DistBetweenAngles(const float from, const float to)
    {
      float delta = to - from;
      const float deltaMod = (std::floor((std::abs(delta) / RADS360))) * RADS360;
      delta += (delta >= 0.0f) ? -deltaMod : deltaMod;
      if (delta > 0)
      {
        return delta <= RADS180 ? delta : -(RADS360 - delta);
      }
      return delta >= -RADS180 ? delta : (RADS360 + delta);
    }


    int32_t ToPowerOfTwo(const int32_t value)
    {
      assert(value >= 0);
      if (value > 0)
      {
        int tmpValue = value;
        --tmpValue;
        tmpValue |= (tmpValue >> 1);
        tmpValue |= (tmpValue >> 2);
        tmpValue |= (tmpValue >> 4);
        tmpValue |= (tmpValue >> 8);
        tmpValue |= (tmpValue >> 16);
        ++tmpValue;    // Val is now the next highest power of 2.
        return tmpValue;
      }
      return 1;
    }


    float Wrap(const float value, const float min, const float max)
    {
      return (value >= min && value < max) ? value : (min + WrapMax(value - min, max - min));
    }

    float WrapAngle(const float angle)
    {
      if ((angle > -MathHelper::PI) && (angle <= MathHelper::PI))
      {
        return angle;
      }

      using namespace std;
      auto result = static_cast<float>(remainder(static_cast<double>(angle), 6.2831854820251465));
      if (result <= -3.14159274f)
      {
        return result + 6.28318548f;
      }
      if (result > 3.14159274f)
      {
        return result - 6.28318548f;
      }
      return result;
    }

    Point2 CalcOptimalSize(const Point2& unitSize, const int32_t unitCount, const RectangleSizeRestrictionFlag restrictionFlags)
    {
      assert(unitSize.X > 0);
      assert(unitSize.Y > 0);
      assert(unitCount > 0);
      auto res = CalcOptimalSize(TypeConverter::UncheckedTo<PxExtent2D>(unitSize), static_cast<uint32_t>(std::max(unitCount, 0)), restrictionFlags);
      return TypeConverter::UncheckedTo<Point2>(res);
    }

    PxSize2D CalcOptimalSize(const PxSize2D& unitSize, const int32_t unitCount, const RectangleSizeRestrictionFlag restrictionFlags)
    {
      assert(unitSize.Width() > 0);
      assert(unitSize.Height() > 0);
      assert(unitCount > 0);
      auto res = CalcOptimalSize(TypeConverter::UncheckedTo<PxExtent2D>(unitSize), static_cast<uint32_t>(std::max(unitCount, 0)), restrictionFlags);
      return TypeConverter::UncheckedTo<PxSize2D>(res);
    }

    PxExtent2D CalcOptimalSize(const PxExtent2D& unitSize, const uint32_t unitCount, const RectangleSizeRestrictionFlag restrictionFlags)
    {
      assert(unitCount > 0);

      const auto areaOfChar = unitSize.Width * unitSize.Height;
      const auto totalArea = areaOfChar * unitCount;

      PxExtent2D result;
      if ((restrictionFlags & RectangleSizeRestrictionFlag::Power2) != RectangleSizeRestrictionFlag::Power2)    // Not pow 2
      {
        if ((restrictionFlags & RectangleSizeRestrictionFlag::Square) != RectangleSizeRestrictionFlag::Square)    // not square
        {
          result = PxExtent2D(unitSize.Width * unitCount, unitSize.Height);
        }
        else
        {
          result = CalcOptimalSizeSquare(totalArea, unitSize, unitCount);
        }
      }
      else
      {
        if ((restrictionFlags & RectangleSizeRestrictionFlag::Square) != RectangleSizeRestrictionFlag::Square)    // not square
        {
          result = CalcOptimalSizePow2(totalArea, unitSize, unitCount);
        }
        else
        {
          result = CalcOptimalSizeSquarePow2(totalArea, unitSize, unitCount);
        }
      }
      assert(((result.Width / unitSize.Width) * (result.Height / unitSize.Height)) >= unitCount);
      return result;
    }
  }
}
