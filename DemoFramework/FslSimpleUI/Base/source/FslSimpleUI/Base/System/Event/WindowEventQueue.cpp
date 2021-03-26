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

#include "WindowEventQueue.hpp"
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslSimpleUI/Base/Event/WindowEvent.hpp>
#include <FslSimpleUI/Base/Event/WindowEventPool.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <algorithm>
#include <cassert>

//#define LOCAL_LOG_ENABLED 1
#ifdef LOCAL_LOG_ENABLED
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define LOCAL_LOG(X) FSLLOG3_INFO(X)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define LOCAL_LOG_IF(cOND, X) FSLLOG3_INFO_IF((cOND), X)
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define LOCAL_LOG(X) \
  {                  \
  }
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define LOCAL_LOG_IF(cOND, X) \
  {                           \
  }
#endif

namespace Fsl
{
  namespace UI
  {
    namespace
    {
      // const std::size_t MAX_EVENT_LOOPS = 1024;
    }


    WindowEventQueue::WindowEventQueue()
      : m_queue(new queue_type())
    {
    }


    WindowEventQueue::~WindowEventQueue() = default;


    void WindowEventQueue::Push(const std::shared_ptr<WindowEvent>& theEvent, const std::shared_ptr<TreeNode>& source)
    {
      if (!theEvent || !source)
      {
        throw std::invalid_argument("params can not be null");
      }

      LOCAL_LOG("WindowEventQueue.Push(" << static_cast<uint32_t>(theEvent->GetEventTypeId()) << ", " << source << ")")

      assert(m_queue);
      m_queue->push_back(WindowEventQueueRecord(WindowEventQueueRecordType::Event, source, source, theEvent));
    }


    // bool WindowEventQueue::TryPush(const std::shared_ptr<WindowEvent>& theEvent, const std::shared_ptr<TreeNode>& source)
    //{
    //  if (!theEvent || !source)
    //  {
    //    FSLLOG3_DEBUG_WARNING("params can not be null");
    //    return false;
    //  }


    //  assert(m_queue);
    //  m_queue->push_back(WindowEventQueueRecord(WindowEventQueueRecordType::Event, source, source, theEvent));
    //  return true;
    //}

    void WindowEventQueue::Swap(std::unique_ptr<queue_type>& rEmptyQueue)
    {
      if (!rEmptyQueue)
      {
        throw std::invalid_argument("queue can not be null");
      }
      if (!rEmptyQueue->empty())
      {
        FSLLOG3_WARNING("Swap the queue was not empty as expected, force clearing it..")
        rEmptyQueue->clear();
      }
      std::swap(rEmptyQueue, m_queue);

      LOCAL_LOG_IF(rEmptyQueue->size() > 0, "WindowEventQueue.Swap")
    }
  }
}
