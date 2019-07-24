/****************************************************************************************************************************************************
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
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

#include <Shared/VulkanCustom/VulkanWindowSystemHelper.hpp>
#include <FslDemoHost/Base/Service/WindowHost/IWindowHostInfo.hpp>

namespace Fsl
{
  namespace VulkanWindowSystemHelper
  {
    std::shared_ptr<VulkanWindowSystem> GetWindowSystem(const std::shared_ptr<IWindowHostInfo>& windowHostInfo)
    {
      auto baseActiveWindowSystem = windowHostInfo->GetWindowSystem().lock();
      if (!baseActiveWindowSystem)
      {
        throw std::runtime_error("Failed to get the active window system");
      }
      auto ptr = std::dynamic_pointer_cast<VulkanWindowSystem>(baseActiveWindowSystem);
      if (!ptr)
      {
        throw std::runtime_error("The window system is not of the expected type");
      }
      return ptr;
    }


    std::shared_ptr<IVulkanNativeWindow> GetActiveWindow(const std::shared_ptr<IWindowHostInfo>& windowHostInfo)
    {
      auto windows = windowHostInfo->GetWindows();
      if (windows.size() != 1)
      {
        throw NotSupportedException("One active window required");
      }

      auto baseWindow = windows.front().lock();
      if (!baseWindow)
      {
        throw std::runtime_error("Failed to get the active window");
      }

      auto ptr = std::dynamic_pointer_cast<IVulkanNativeWindow>(baseWindow);
      if (!ptr)
      {
        throw std::runtime_error("The window is not of the expected type");
      }
      return ptr;
    }
  }
}
