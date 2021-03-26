#ifndef FSLNATIVEWINDOW_PLATFORM_QNX_PLATFORMNATIVEWINDOWQNX_HPP
#define FSLNATIVEWINDOW_PLATFORM_QNX_PLATFORMNATIVEWINDOWQNX_HPP
#if defined(__QNXNTO__)
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

#include <FslNativeWindow/Platform/PlatformNativeWindow.hpp>
#include <screen/screen.h>

namespace Fsl
{
  class NativeWindowSetup;

  class PlatformNativeWindowQNX : public PlatformNativeWindow
  {
    screen_context_t screen_ctx;

    PlatformNativeDisplayType m_hDisplayType;
    PlatformNativeWindowType m_hWindow;

    int SelectDisplay(screen_context_t ctx, screen_window_t win, int id);

  public:
    PlatformNativeWindowQNX(const NativeWindowSetup& nativeWindowSetup, const PlatformNativeWindowParams& platformWindowParams,
                            const PlatformNativeWindowAllocationParams* const pPlatformCustomWindowAllocationParams);
    virtual ~PlatformNativeWindowQNX();

  protected:
    bool TryGetNativeSize(PxPoint2& rSize) const override;
  };
}    // namespace Fsl

#endif
#endif
