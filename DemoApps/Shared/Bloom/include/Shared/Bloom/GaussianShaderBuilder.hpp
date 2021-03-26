#ifndef SHARED_BLOOM_GAUSSIANSHADERBUILDER_HPP
#define SHARED_BLOOM_GAUSSIANSHADERBUILDER_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
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

#include <string>

namespace Fsl
{
  struct Gaussian5
  {
    float Weight0{};
    float Weight1{};
    float Offset0{};

    constexpr Gaussian5() = default;

    constexpr Gaussian5(const float weight0, const float weight1, const float offset0)
      : Weight0(weight0)
      , Weight1(weight1)
      , Offset0(offset0)
    {
    }
  };

  struct Gaussian9
  {
    float Weight0{};
    float Weight1{};
    float Weight2{};
    float Offset0{};
    float Offset1{};

    constexpr Gaussian9() = default;

    constexpr Gaussian9(const float weight0, const float weight1, const float weight2, const float offset0, const float offset1)
      : Weight0(weight0)
      , Weight1(weight1)
      , Weight2(weight2)
      , Offset0(offset0)
      , Offset1(offset1)
    {
    }
  };

  class GaussianShaderBuilder
  {
  public:
    static Gaussian5 Build5x5(const float kernelWeightMod);
    static Gaussian9 Build9x9(const float kernelWeightMod);
    static std::string Build5x5(const std::string& strTemplate, const float kernelWeightMod);
    static std::string Build9x9(const std::string& strTemplate, const float kernelWeightMod);
  };
}

#endif
