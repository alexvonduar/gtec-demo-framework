#ifndef FSLDEMOAPP_BASE_SERVICE_EVENTS_BASIC_BASICEVENT_HPP
#define FSLDEMOAPP_BASE_SERVICE_EVENTS_BASIC_BASICEVENT_HPP
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

#include <FslBase/BasicTypes.hpp>
#include <FslDemoApp/Base/Service/Events/IEvent.hpp>

namespace Fsl
{
  //! @brief The basic event is used for small often send messages to prevent memory allocation / deallocation
  //!        The basic event is not meant to be used directly, it should be transformed to the specialized type using m_type before being used
  class BasicEvent : public IEvent
  {
    struct Flags
    {
      enum Enum
      {
        Handled = 0x01
      };
    };
    mutable uint32_t m_flags{0};

  public:
    BasicEvent() = default;

    EventType GetEventType() const override
    {
      return m_type;
    }


    //! @brief Check if something has handled the event
    bool IsHandled() const
    {
      return (m_flags & Flags::Handled) != 0;
    }

    //! @brief mark the event as handled
    void Handled() const
    {
      m_flags |= Flags::Handled;
    }

  protected:
    explicit BasicEvent(const EventType type)
      : m_type(type)
    {
    }


    BasicEvent(const EventType type, const int32_t arg1)
      : m_type(type)
      , m_arg1(arg1)
    {
    }


    BasicEvent(const EventType type, const int32_t arg1, const int32_t arg2)
      : m_type(type)
      , m_arg1(arg1)
      , m_arg2(arg2)
    {
    }


    BasicEvent(const EventType type, const int32_t arg1, const int32_t arg2, const int32_t arg3)
      : m_type(type)
      , m_arg1(arg1)
      , m_arg2(arg2)
      , m_arg3(arg3)
    {
    }

    EventType m_type{EventType::Invalid};
    int32_t m_arg1{0};
    int32_t m_arg2{0};
    int32_t m_arg3{0};
  };
}

#endif
