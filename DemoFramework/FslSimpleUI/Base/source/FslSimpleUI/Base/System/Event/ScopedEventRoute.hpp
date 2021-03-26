#ifndef FSLSIMPLEUI_BASE_SYSTEM_EVENT_SCOPEDEVENTROUTE_HPP
#define FSLSIMPLEUI_BASE_SYSTEM_EVENT_SCOPEDEVENTROUTE_HPP
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
#include <stdexcept>
#include "EventRoute.hpp"
#include "EventRoutePool.hpp"

namespace Fsl
{
  namespace UI
  {
    class IEventHandler;
    struct WindowFlags;

    struct ScopedEventRoute
    {
    private:
      std::shared_ptr<EventRoutePool> m_pool;
      std::shared_ptr<EventRoute> m_route;

    public:
      ScopedEventRoute(const ScopedEventRoute&) = delete;
      ScopedEventRoute& operator=(const ScopedEventRoute&) = delete;

      const std::shared_ptr<EventRoute>& Route()
      {
        return m_route;
      }

      ScopedEventRoute(const std::shared_ptr<EventRoutePool>& pool, const std::shared_ptr<IEventHandler>& eventHandler, const WindowFlags& flags)
      {
        FSL_PARAM_NOT_USED(eventHandler);

        if (!pool)
        {
          throw std::invalid_argument("pool can not be null");
        }
        m_pool = pool;
        m_route = pool->Allocate(flags);
      }

      ~ScopedEventRoute()
      {
        if (m_pool)
        {
          m_pool->Release(m_route);
          m_pool.reset();
        }
        m_route.reset();
      }
    };
  }
}

#endif
