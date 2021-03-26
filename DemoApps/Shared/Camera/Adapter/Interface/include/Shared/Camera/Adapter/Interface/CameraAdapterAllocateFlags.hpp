#ifndef SHARED_CAMERA_ADAPTER_INTERFACE_CAMERAADAPTERALLOCATEFLAGS_HPP
#define SHARED_CAMERA_ADAPTER_INTERFACE_CAMERAADAPTERALLOCATEFLAGS_HPP
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

#include <FslBase/BasicTypes.hpp>

namespace Fsl
{
  namespace Helios
  {
    struct CameraAdapterAllocateFlags
    {
      enum Enum
      {
        // A empty flag
        NotDefined = 0x00,

        // Utilize the custom extent (if not specified then the camera default size will be used)
        CustomExtent = 0x01,
      };

      uint32_t Value{0};

      CameraAdapterAllocateFlags() = default;

      explicit CameraAdapterAllocateFlags(const uint32_t flags)
        : Value(flags)
      {
      }

      CameraAdapterAllocateFlags(const Enum flag)    // NOLINT(google-explicit-constructor)
        : Value(static_cast<uint32_t>(flag))
      {
      }


      inline bool IsEnabled(Enum flag) const
      {
        return (Value & static_cast<uint32_t>(flag)) != 0;
      }

      inline void Enable(Enum flag)
      {
        Value |= static_cast<uint32_t>(flag);
      }


      inline void Disable(Enum flag)
      {
        Value &= ~static_cast<uint32_t>(flag);
      }

      inline void Set(Enum flag, const bool enabled)
      {
        Value = enabled ? (Value | static_cast<uint32_t>(flag)) : (Value & ~static_cast<uint32_t>(flag));
      }
    };
  }
}
#endif
