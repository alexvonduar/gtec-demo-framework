#ifndef FSLSIMPLEUI_BASE_EVENT_WINDOWSELECTEVENT_HPP
#define FSLSIMPLEUI_BASE_EVENT_WINDOWSELECTEVENT_HPP
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
#include <FslBase/ITag.hpp>
#include <FslSimpleUI/Base/Event/WindowEvent.hpp>

namespace Fsl
{
  namespace UI
  {
    class WindowSelectEvent : public WindowEvent
    {
      uint32_t m_contentId;
      std::shared_ptr<ITag> m_payload;

    public:
      WindowSelectEvent();

      uint32_t GetContentId() const;
      const std::shared_ptr<ITag>& GetPayload() const;

    protected:
      void SYS_Construct(const uint32_t contentId, const std::shared_ptr<ITag>& payload)
      {
        WindowEvent::SYS_DoConstruct();
        m_contentId = contentId;
        m_payload = payload;
      }

      void SYS_Destruct() override
      {
        m_contentId = 0;
        m_payload.reset();
        WindowEvent::SYS_Destruct();
      }

      friend class WindowEventPool;
    };
  }
}

#endif
