#if !defined(__ANDROID__) && defined(__linux__) && defined(FSL_WINDOWSYSTEM_X11)
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

#include "EGLNativeWindowSystemX11.hpp"
#include <FslNativeWindow/EGL/EGLNativeWindowTemplate.hpp>
#include <FslNativeWindow/Platform/X11/PlatformNativeWindowX11.hpp>
#include <FslBase/Log/Log3Fmt.hpp>
#include <FslUtil/EGL/Exceptions.hpp>
#include <FslUtil/EGL/EGLCheck.hpp>

namespace Fsl
{
  namespace
  {
    std::shared_ptr<INativeWindow> AllocateWindow(const NativeWindowSetup& nativeWindowSetup, const PlatformNativeWindowParams& windowParams,
                                                  const PlatformNativeWindowAllocationParams* const pPlatformCustomWindowAllocationParams)
    {
      const auto* const pNativeEglSetup = dynamic_cast<const NativeEGLSetup*>(pPlatformCustomWindowAllocationParams);
      if (pNativeEglSetup == nullptr)
      {
        throw NotSupportedException("NativeEGLSetup pointer expected");
      }

      // Get the native visual id that matches the EGL config
      EGLint nativeVisualId{};
      EGL_CHECK(eglGetConfigAttrib(pNativeEglSetup->Display, pNativeEglSetup->Config, EGL_NATIVE_VISUAL_ID, &nativeVisualId));

      // Patch the params and forward
      PlatformNativeWindowParams customWindowParams(windowParams);
      customWindowParams.VisualId = nativeVisualId;

      return std::make_shared<EGLNativeWindowTemplate<PlatformNativeWindowX11>>(nativeWindowSetup, customWindowParams,
                                                                                pPlatformCustomWindowAllocationParams);
    }
  }


  EGLNativeWindowSystemX11::EGLNativeWindowSystemX11(const NativeWindowSystemSetup& setup)
    : EGLNativeWindowSystemTemplate<PlatformNativeWindowSystemX11>(setup, AllocateWindow)
  {
  }
}
#endif
