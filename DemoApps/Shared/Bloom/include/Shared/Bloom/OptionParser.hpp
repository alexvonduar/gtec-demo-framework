#ifndef SHARED_BLOOM_OPTIONPARSER_HPP
#define SHARED_BLOOM_OPTIONPARSER_HPP
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

#include <FslDemoApp/Base/ADemoOptionParser.hpp>

namespace Fsl
{
  class OptionParser : public ADemoOptionParser
  {
    int32_t m_sceneId;
    bool m_rotateEnabled;
    bool m_blurPassEnabled;
    bool m_brightPassEnabled;
    bool m_renderFinalScene;
    bool m_renderFinalBloom;
    bool m_showBuffers;
    bool m_scaleInputSequentially;

  public:
    OptionParser();
    ~OptionParser() override;

    int32_t GetSceneId() const
    {
      return m_sceneId;
    }
    bool IsRotateEnabled() const
    {
      return m_rotateEnabled;
    }
    bool IsBlurPassEnabled() const
    {
      return m_blurPassEnabled;
    }
    bool IsBrightPassEnabled() const
    {
      return m_brightPassEnabled;
    }
    bool IsFinalSceneEnabled() const
    {
      return m_renderFinalScene;
    }
    bool IsFinalBloomEnabled() const
    {
      return m_renderFinalBloom;
    }
    bool IsShowBuffersEnabled() const
    {
      return m_showBuffers;
    }
    bool IsScaleInputSequentiallyEnabled() const
    {
      return m_scaleInputSequentially;
    }

  protected:
    void OnArgumentSetup(std::deque<Option>& rOptions) override;
    OptionParseResult OnParse(const int32_t cmdId, const StringViewLite& strOptArg) override;
    bool OnParsingComplete() override;
  };
}

#endif
