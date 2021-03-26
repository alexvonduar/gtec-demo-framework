#ifndef FSLSIMPLEUI_BASE_SYSTEM_MODULES_IMODULEHOST_HPP
#define FSLSIMPLEUI_BASE_SYSTEM_MODULES_IMODULEHOST_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
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
#include <FslSimpleUI/Base/WindowFlags.hpp>
#include <memory>
#include "../Event/FunctionCreateTargetWindowDeathEvent.hpp"

namespace Fsl
{
  namespace UI
  {
    class IStateEventCreator;
    class IStateEventSender;
    class IStateEventSenderGroup;
    class ITreeNodeClickInputTargetLocater;
    class SimpleEventSender;
    struct StateEventInfo;
    class WindowEventPool;
    class WindowEventSender;

    class IModuleHost
    {
    public:
      virtual ~IModuleHost() = default;

      //! @brief Get the click target locater
      virtual std::shared_ptr<ITreeNodeClickInputTargetLocater> GetTargetLocater() const = 0;

      //! @brief Get the window event pool
      virtual std::shared_ptr<WindowEventPool> GetWindowEventPool() const = 0;

      //! @brief Get the window event sender (used for queuing events)
      virtual std::shared_ptr<WindowEventSender> GetWindowEventSender() const = 0;

      //! @brief Get the simple event sender (used for sending non-state-full events directly)
      virtual std::shared_ptr<SimpleEventSender> GetSimpleEventSender() const = 0;

      //! @brief Create a state event sender
      virtual std::shared_ptr<IStateEventSender>
        CreateStateEventSender(const WindowFlags::Enum inputType, const FunctionCreateTargetWindowDeathEvent& fnCreateTargetWindowDeathEvent) = 0;
    };
  }
}

#endif
