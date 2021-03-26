/****************************************************************************************************************************************************
 * Copyright 2018 NXP
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

#include <FslSimpleUI/Base/WindowCollection/WindowCollectionBase.hpp>
#include <FslSimpleUI/Base/BaseWindow.hpp>
#include <FslSimpleUI/Base/IWindowManager.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <cassert>
#include <deque>
#include <memory>

namespace Fsl
{
  namespace UI
  {
    WindowCollectionBase::WindowCollectionBase()
      : m_pOwner(nullptr)
    {
    }


    void WindowCollectionBase::DoInit(BaseWindow* const pOwner, const std::shared_ptr<IWindowManager>& windowManager)
    {
      if (pOwner == nullptr)
      {
        throw std::invalid_argument("pOwner can not be null");
      }
      if (!windowManager)
      {
        throw std::invalid_argument("windowManager can not be null");
      }
      if (IsInitialized())
      {
        throw UsageErrorException("DoInit already called");
      }

      m_pOwner = pOwner;
      m_windowManager = windowManager;
    }

    void WindowCollectionBase::DoAdd(const std::shared_ptr<BaseWindow>& window)
    {
      assert(m_windowManager != nullptr);
      m_windowManager->AddChild(m_pOwner, window);
    }

    void WindowCollectionBase::DoScheduleClose(const std::shared_ptr<BaseWindow>& window)
    {
      assert(m_windowManager != nullptr);
      m_windowManager->ScheduleClose(window);
    }


    void WindowCollectionBase::DoMarkLayoutDirty()
    {
      assert(m_pOwner != nullptr);
      m_pOwner->SYS_MarkLayoutDirty();
    }
  }
}
