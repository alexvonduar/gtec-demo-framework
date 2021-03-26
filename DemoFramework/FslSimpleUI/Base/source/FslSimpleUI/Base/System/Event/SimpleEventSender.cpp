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

#include "SimpleEventSender.hpp"
#include "EventRoute.hpp"
#include "../TreeNode.hpp"
#include <FslBase/Exceptions.hpp>
#include <FslBase/Math/Rect.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslSimpleUI/Base/Event/EventDescription.hpp>
#include <FslSimpleUI/Base/Event/WindowEvent.hpp>
#include <FslSimpleUI/Base/Event/WindowEventPool.hpp>
#include "../ITreeContextInfo.hpp"
#include <cassert>
#include <utility>

namespace Fsl
{
  namespace UI
  {
    SimpleEventSender::SimpleEventSender(std::shared_ptr<ITreeContextInfo> treeContextInfo, std::shared_ptr<WindowEventPool> windowEventPool,
                                         std::shared_ptr<IEventHandler> eventHandler, const std::shared_ptr<TreeNode>& rootNode,
                                         const std::shared_ptr<ITreeNodeClickInputTargetLocater>& clickTargetLocater)
      : m_treeContextInfo(std::move(treeContextInfo))
      , m_windowEventPool(std::move(windowEventPool))
      , m_eventHandler(std::move(eventHandler))
      , m_eventRouter(rootNode, clickTargetLocater)
    {
      if (!m_treeContextInfo || !m_windowEventPool || !rootNode || !clickTargetLocater || !m_eventHandler)
      {
        throw std::invalid_argument("arguments can not be null");
      }
    }


    SimpleEventSender::~SimpleEventSender() = default;


    SendResult SimpleEventSender::Send(const std::shared_ptr<WindowEvent>& theEvent, const std::shared_ptr<TreeNode>& target, const bool manageEvent)
    {
      if (!m_treeContextInfo->IsInSystemContext())
      {
        throw UsageErrorException("Method call from invalid context");
      }

      if (!theEvent)
      {
        FSLLOG3_WARNING("Can not send a null event, request ignored.");
        return SendResult::Error;
      }
      if (!target)
      {
        FSLLOG3_WARNING("can not send to a null target, request ignored.");
        return SendResult::Error;
      }

      try
      {
        const EventDescription eventDesc = theEvent->GetDescription();
        EventRoute::StackScopedInit scopedInit(m_eventRoute, eventDesc.RequiredFlags);
        m_eventRouter.CreateRoute(m_eventRoute, eventDesc.RoutingStrategy, target);
        m_eventRoute.Send(m_eventHandler.get(), theEvent);
        const auto result = (theEvent->IsHandled() ? SendResult::Handled : SendResult::Unhandled);

        m_eventRoute.Clear();
        if (manageEvent)
        {
          m_windowEventPool->Release(theEvent);
        }
        return result;
      }
      catch (...)
      {
        m_eventRoute.Clear();
        if (manageEvent)
        {
          m_windowEventPool->Release(theEvent);
        }
      }
      return SendResult::Error;
    }


    SendResult SimpleEventSender::Send(const std::shared_ptr<WindowEvent>& theEvent, const PxPoint2& screenHitPositionPx, const bool manageEvent)
    {
      if (!m_treeContextInfo->IsInSystemContext())
      {
        throw UsageErrorException("Method call from invalid context");
      }

      if (!theEvent)
      {
        FSLLOG3_WARNING("Can not send a null event, request ignored.");
        return SendResult::Error;
      }

      try
      {
        const EventDescription eventDesc = theEvent->GetDescription();
        EventRoute::StackScopedInit scopedInit(m_eventRoute, eventDesc.RequiredFlags);
        m_eventRouter.CreateRoute(m_eventRoute, eventDesc.RoutingStrategy, screenHitPositionPx);
        m_eventRoute.Send(m_eventHandler.get(), theEvent);
        const auto result = (theEvent->IsHandled() ? SendResult::Handled : SendResult::Unhandled);

        m_eventRoute.Clear();
        if (manageEvent)
        {
          m_windowEventPool->Release(theEvent);
        }
        return result;
      }
      catch (...)
      {
        m_eventRoute.Clear();
        if (manageEvent)
        {
          m_windowEventPool->Release(theEvent);
        }
      }
      return SendResult::Error;
    }
  }
}
