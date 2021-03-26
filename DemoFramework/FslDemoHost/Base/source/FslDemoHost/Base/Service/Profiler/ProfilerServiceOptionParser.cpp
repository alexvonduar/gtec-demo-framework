/****************************************************************************************************************************************************
 * Copyright 2017 NXP
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

#include <FslDemoHost/Base/Service/Profiler/ProfilerServiceOptionParser.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/String/StringParseUtil.hpp>
#include <fmt/format.h>
#include <algorithm>
#include <cassert>
#include <cstring>

namespace Fsl
{
  namespace
  {
    const uint16_t DEFAULT_ENTRIES = 60;

    struct CommandId
    {
      enum Enum
      {
        AverageEntries,
      };
    };
  }


  ProfilerServiceOptionParser::ProfilerServiceOptionParser()
    : m_averageEntries(DEFAULT_ENTRIES)
  {
  }


  void ProfilerServiceOptionParser::OnArgumentSetup(std::deque<Option>& rOptions)
  {
    rOptions.emplace_back("Profiler.AverageEntries", OptionArgument::OptionRequired, CommandId::AverageEntries,
                          fmt::format("The number of frames used to calculate the average frame-time. Defaults to: {}", DEFAULT_ENTRIES));
  }


  OptionParseResult ProfilerServiceOptionParser::OnParse(const int32_t cmdId, const StringViewLite& strOptArg)
  {
    switch (cmdId)
    {
    case CommandId::AverageEntries:
    {
      StringParseUtil::Parse(m_averageEntries, strOptArg);
      m_averageEntries = std::max(m_averageEntries, 1u);
      return OptionParseResult::Parsed;
    }
    default:
      return OptionParseResult::NotHandled;
    }
  }


  bool ProfilerServiceOptionParser::OnParsingComplete()
  {
    return true;
  }
}
