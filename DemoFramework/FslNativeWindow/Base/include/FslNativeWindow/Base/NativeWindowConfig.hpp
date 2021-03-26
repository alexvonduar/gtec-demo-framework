#ifndef FSLNATIVEWINDOW_BASE_NATIVEWINDOWCONFIG_HPP
#define FSLNATIVEWINDOW_BASE_NATIVEWINDOWCONFIG_HPP
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

#include <memory>
#include <FslBase/Math/Point2U.hpp>
#include <FslBase/Math/Rectangle.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslBase/Optional.hpp>
#include <FslNativeWindow/Base/WindowMode.hpp>

namespace Fsl
{
  class NativeWindowConfig
  {
  private:
    WindowMode m_windowMode;
    Rectangle m_windowRectangle;
    int32_t m_displayId;
    Optional<uint32_t> m_forcedDensityDpi;
    Optional<Point2U> m_forcedActualDpi;

  public:
    NativeWindowConfig();
    NativeWindowConfig(const WindowMode windowMode, const Rectangle& windowRectangle);

    WindowMode GetWindowMode() const;
    void SetWindowMode(const WindowMode windowMode);

    Rectangle GetWindowRectangle() const;
    void SetWindowRectangle(const Rectangle& rect);

    int32_t GetDisplayId() const;
    void SetDisplayId(const int32_t displayId);

    //! A negative value means no value has been provided
    Optional<uint32_t> GetForcedDensityDpi() const;

    void SetForcedDensityDpi(const Optional<uint32_t>& densityDpi);

    //! A negative value means no value has been provided
    Optional<Point2U> GetForcedActualDpi() const;

    void SetForcedActualDpi(const Optional<Point2U>& actualDpi);
  };
}

#endif
