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

#include <FslDemoApp/Shared/Host/DemoHostFeatureUtil.hpp>
#include <limits>

namespace Fsl
{
  uint32_t DemoHostFeatureUtil::EncodeOpenGLESVersion(const uint16_t major)
  {
    // a minor value of 0xFFFF means we only care about the major version
    return (major << 16) | 0xFFFF;
  }

  uint32_t DemoHostFeatureUtil::EncodeOpenGLESVersion(const uint16_t major, const uint16_t minor)
  {
    return (major << 16) | minor;
  }


  void DemoHostFeatureUtil::DecodeOpenGLESVersion(uint32_t encodedValue, int32_t& rMajor, int32_t& rMinor)
  {
    rMajor = (encodedValue >> 16) & 0xFFFF;
    uint16_t minor = encodedValue & 0xFFFF;
    rMinor = minor != 0xFFFF ? minor : -1;
  }


  uint32_t DemoHostFeatureUtil::EncodeOpenVGVersion(const uint16_t major)
  {
    // a minor value of 0xFFFF means we only care about the major version
    return (major << 16) | 0xFFFF;
  }

  uint32_t DemoHostFeatureUtil::EncodeOpenVGVersion(const uint16_t major, const uint16_t minor)
  {
    return (major << 16) | minor;
  }


  void DemoHostFeatureUtil::DecodeOpenVGVersion(uint32_t encodedValue, int32_t& rMajor, int32_t& rMinor)
  {
    rMajor = (encodedValue >> 16) & 0xFFFF;
    uint16_t minor = encodedValue & 0xFFFF;
    rMinor = minor != 0xFFFF ? minor : -1;
  }


  uint32_t DemoHostFeatureUtil::EncodeG2DVersion(const uint16_t major)
  {
    // a minor value of 0xFFFF means we only care about the major version
    return (major << 16) | 0xFFFF;
  }

  uint32_t DemoHostFeatureUtil::EncodeG2DVersion(const uint16_t major, const uint16_t minor)
  {
    return (major << 16) | minor;
  }


  void DemoHostFeatureUtil::DecodeG2DVersion(uint32_t encodedValue, int32_t& rMajor, int32_t& rMinor)
  {
    rMajor = (encodedValue >> 16) & 0xFFFF;
    uint16_t minor = encodedValue & 0xFFFF;
    rMinor = minor != 0xFFFF ? minor : -1;
  }


  uint32_t DemoHostFeatureUtil::EncodeVersion(const uint16_t major)
  {
    // a minor value of 0xFFFF means we only care about the major version
    return (major << 16) | 0xFFFF;
  }

  uint32_t DemoHostFeatureUtil::EncodeVersion(const uint16_t major, const uint16_t minor)
  {
    return (major << 16) | minor;
  }


  void DemoHostFeatureUtil::DecodeVersion(uint32_t encodedValue, int32_t& rMajor, int32_t& rMinor)
  {
    rMajor = (encodedValue >> 16) & 0xFFFF;
    uint16_t minor = encodedValue & 0xFFFF;
    rMinor = minor != 0xFFFF ? minor : -1;
  }
}
