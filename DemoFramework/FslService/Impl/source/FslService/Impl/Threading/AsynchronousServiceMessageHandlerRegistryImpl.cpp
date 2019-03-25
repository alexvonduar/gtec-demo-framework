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

#include "AsynchronousServiceMessageHandlerRegistryImpl.hpp"
#include <FslBase/Exceptions.hpp>
#include <FslService/Impl/Foundation/Message/Message.hpp>
#include <cassert>
#include <typeinfo>

namespace Fsl
{
  bool AsynchronousServiceMessageHandlerRegistryImpl::TryProcessMessage(const std::shared_ptr<Message>& message)
  {
    if (!message)
    {
      return false;
    }

    // Lookup the actual type of the message, then determine if we have a handler registered for it
    const auto pMessage = message.get();
    const auto messageTypeInfo = std::type_index(typeid(*pMessage));

    // Try to locate a handler for the message type
    auto itrFind = m_lookup.find(messageTypeInfo);
    if (itrFind == m_lookup.end())
    {
      return false;
    }

    assert(itrFind->second.Handler);
    if (itrFind->second.Handler)
    {
      itrFind->second.Handler(*message);
    }
    return true;
  }


  void AsynchronousServiceMessageHandlerRegistryImpl::Register(const std::type_index& messageType,
                                                               const std::function<void(Message& message)>& handler)
  {
    if (m_lookup.find(messageType) != m_lookup.end())
    {
      throw UsageErrorException("Can only register a message type once");
    }

    m_lookup[messageType] = AsyncServiceMessageHandler(handler);
  }
}
