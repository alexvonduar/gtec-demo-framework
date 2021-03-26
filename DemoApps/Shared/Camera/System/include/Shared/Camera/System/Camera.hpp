#ifndef SHARED_CAMERA_SYSTEM_CAMERA_HPP
#define SHARED_CAMERA_SYSTEM_CAMERA_HPP
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

#include <FslBase/Math/Pixel/PxExtent2D.hpp>
#include <FslBase/Math/Pixel/TypeConverter.hpp>
#include <FslBase/Math/Point2.hpp>
#include <FslGraphics/Bitmap/RawBitmapEx.hpp>
#include <FslGraphics/PixelFormat.hpp>
#include <Shared/Camera/System/CameraConfig.hpp>
#include <memory>

namespace Fsl
{
  namespace Helios
  {
    class ICameraAdapter;

    class Camera
    {
      CameraConfig m_cameraConfig;
      std::shared_ptr<ICameraAdapter> m_nativeCamera;

    public:
      Camera(const CameraConfig& cameraConfig, std::shared_ptr<ICameraAdapter> nativeCamera);
      ~Camera();

      CameraType GetType() const
      {
        return m_cameraConfig.Type;
      }

      //! @brief Get the camera buffer stride
      uint32_t GetStride() const
      {
        return m_cameraConfig.NativeStride;
      }

      //! @brief Returns the camera size (unsigned)
      PxExtent2D GetExtent() const
      {
        return m_cameraConfig.Extent;
      }

      //! @brief Returns the camera size (signed)
      PxSize2D GetSize() const
      {
        return TypeConverter::UncheckedTo<PxSize2D>(m_cameraConfig.Extent);
      }

      PixelFormat GetPixelFormat() const
      {
        return m_cameraConfig.NativePixelFormat;
      }

      //! @brief Try to render a frame to the rTargetBitmap.
      //! @param rTargetBitmap a RawBitmapEx that is compatible with this camera
      //! @param rFrameId the id of the frame. It will only be modified if this method returns true (but it will never be zero),
      //!                 if the method fails this will be zero.
      //! @return true if a new frame was rendered, false if it failed to render (if it fails the content written to rTargetBitmap is undefined)
      bool TryRender(RawBitmapEx& rTargetBitmap, uint32_t& rFrameId);
    };
  }
}
#endif
