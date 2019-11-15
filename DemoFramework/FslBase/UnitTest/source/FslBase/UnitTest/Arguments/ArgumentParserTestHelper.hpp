#ifndef FSLBASE_UNITTEST_ARGUMENTS_FSLBASE_UNITTEST_ARGUMENTPARSERTESTHELPER_HPP
#define FSLBASE_UNITTEST_ARGUMENTS_FSLBASE_UNITTEST_ARGUMENTPARSERTESTHELPER_HPP
/****************************************************************************************************************************************************
 * Copyright 2019 NXP
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

#include <FslBase/Arguments/ArgumentParser.hpp>
#include <fmt/format.h>
#include <array>
#include <limits>
#include <ostream>
#include <vector>


namespace Fsl
{
  namespace Arguments
  {
    inline std::ostream& operator<<(std::ostream& o, const ParseResult value)
    {
      switch (value)
      {
      case ParseResult::InternalError:
        o << "ParseResult::InternalError";
        break;
      case ParseResult::Completed:
        o << "ParseResult::Completed";
        break;
      case ParseResult::InvalidArguments:
        o << "ParseResult::InvalidArguments";
        break;
      case ParseResult::DuplicatedSwitchArgumentError:
        o << "ParseResult::DuplicatedSwitchArgumentError";
        break;
      case ParseResult::DuplicatedValueArgumentError:
        o << "ParseResult::DuplicatedValueArgumentError";
        break;
      case ParseResult::UnknownArgumentError:
        o << "ParseResult::UnknownArgumentError";
        break;
      case ParseResult::ArgumentMissingValueError:
        o << "ParseResult::ArgumentMissingValueError";
        break;
      case ParseResult::CombinedValueArgumentMustBeLastError:
        o << "ParseResult::CombinedValueArgumentMustBeLastError";
        break;
      case ParseResult::ArgumentFormatError:
        o << "ParseResult::ArgumentFormatError";
        break;
      case ParseResult::RequiredArgumentNotFound:
        o << "ParseResult::RequiredArgumentNotFound";
        break;
      case ParseResult::ArgumentEmptyError:
        o << "ParseResult::ArgumentEmptyError";
        break;
      case ParseResult::ArgumentListContainedNullError:
        o << "ParseResult::ArgumentListContainedNullError";
        break;
      case ParseResult::CommandListIsInvalidError:
        o << "ParseResult::CommandListIsInvalidError";
        break;
      default:
        o << static_cast<uint32_t>(value);
        break;
      }
      return o;
    }


    class ArgumentParserException : public std::runtime_error
    {
    public:
      ArgumentParserException(const std::string& msg)
        : std::runtime_error(msg)
      {
      }
    };


    class InternalException : public ArgumentParserException
    {
    public:
      InternalException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class DuplicatedSwitchArgumentException : public ArgumentParserException
    {
    public:
      DuplicatedSwitchArgumentException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class DuplicatedValueArgumentException : public ArgumentParserException
    {
    public:
      DuplicatedValueArgumentException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class UnknownArgumentException : public ArgumentParserException
    {
    public:
      UnknownArgumentException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class ArgumentMissingValueException : public ArgumentParserException
    {
    public:
      ArgumentMissingValueException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class CombinedValueArgumentMustBeLastException : public ArgumentParserException
    {
    public:
      CombinedValueArgumentMustBeLastException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class ArgumentFormatException : public ArgumentParserException
    {
    public:
      ArgumentFormatException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class RequiredArgumentNotFoundException : public ArgumentParserException
    {
    public:
      RequiredArgumentNotFoundException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class ArgumentEmptyException : public ArgumentParserException
    {
    public:
      ArgumentEmptyException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class ArgumentListContainedNullException : public ArgumentParserException
    {
    public:
      ArgumentListContainedNullException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    class CommandListIsInvalidErrorException : public ArgumentParserException
    {
    public:
      CommandListIsInvalidErrorException(const std::string& msg)
        : ArgumentParserException(msg)
      {
      }
    };


    template <std::size_t TSize>
    std::deque<EncodedCommand> ParseNow(const std::array<const char*, TSize>& testArgs, const std::deque<Command>& commands,
                                        ParseErrorInfo* pErrorInfo = nullptr)
    {
      std::deque<EncodedCommand> encodedArguments;
      auto res = ArgumentParser::TryParse(encodedArguments, static_cast<int>(testArgs.size()), testArgs.data(), commands, pErrorInfo);
      switch (res)
      {
      case ParseResult::InternalError:
        throw InternalException("InternalError");
      case ParseResult::Completed:
        return encodedArguments;
      case ParseResult::InvalidArguments:
        throw std::invalid_argument("InvalidArguments");
      case ParseResult::DuplicatedSwitchArgumentError:
        throw DuplicatedSwitchArgumentException("DuplicatedSwitchArgumentError");
      case ParseResult::DuplicatedValueArgumentError:
        throw DuplicatedValueArgumentException("DuplicatedValueArgumentError");
      case ParseResult::UnknownArgumentError:
        throw UnknownArgumentException("UnknownArgumentError");
      case ParseResult::ArgumentMissingValueError:
        throw ArgumentMissingValueException("ArgumentMissingValueError");
      case ParseResult::CombinedValueArgumentMustBeLastError:
        throw CombinedValueArgumentMustBeLastException("CombinedValueArgumentMustBeLastError");
      case ParseResult::ArgumentFormatError:
        throw ArgumentFormatException("ArgumentFormatError");
      case ParseResult::RequiredArgumentNotFound:
        throw RequiredArgumentNotFoundException("RequiredArgumentNotFound");
      case ParseResult::ArgumentEmptyError:
        throw ArgumentEmptyException("ArgumentEmptyError");
      case ParseResult::ArgumentListContainedNullError:
        throw ArgumentListContainedNullException("ArgumentListContainedNullError");
      case ParseResult::CommandListIsInvalidError:
        throw CommandListIsInvalidErrorException("CommandListIsInvalidError");
      default:
        throw std::runtime_error(fmt::format("Unhandled ParseResult {0}", res));
      }
    }


    template <std::size_t TSize>
    ParseResult TryParseNow(std::deque<EncodedCommand>& rEncodedArguments, const std::array<const char*, TSize>& testArgs,
                            const std::deque<Command>& commands, ParseErrorInfo* pErrorInfo = nullptr)
    {
      return ArgumentParser::TryParse(rEncodedArguments, static_cast<int>(testArgs.size()), testArgs.data(), commands, pErrorInfo);
    }
  }
}

#endif
