#ifndef FSLSIMPLEUI_BASE_SYSTEM_EVENT_WINDOWEVENTQUEUE_HPP
#define FSLSIMPLEUI_BASE_SYSTEM_EVENT_WINDOWEVENTQUEUE_HPP
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

#include <deque>
#include <memory>
#include "WindowEventQueueRecord.hpp"

namespace Fsl
{
  namespace UI
  {
    class TreeNode;
    class WindowEvent;

    // This only servers as a little container's class that holds the current active event queue instance
    class WindowEventQueue
    {
    public:
      using queue_type = std::deque<WindowEventQueueRecord>;

    protected:
      std::unique_ptr<queue_type> m_queue;

    public:
      WindowEventQueue(const WindowEventQueue&) = delete;
      WindowEventQueue& operator=(const WindowEventQueue&) = delete;

      WindowEventQueue();
      virtual ~WindowEventQueue();

      //! @brief Check if the queue is empty.
      inline bool IsEmpty() const
      {
        return m_queue->empty();
      }

      void Push(const std::shared_ptr<WindowEvent>& theEvent, const std::shared_ptr<TreeNode>& source);
      // bool TryPush(const std::shared_ptr<WindowEvent>& theEvent, const std::shared_ptr<TreeNode>& source);

      //! @brief Swap the used queue with the provided empty queue, return the previously used queue
      void Swap(std::unique_ptr<queue_type>& rEmptyQueue);
    };
  }
}

#endif
