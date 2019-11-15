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

#include <FslBase/Exceptions.hpp>
#include <FslBase/IO/Path.hpp>
#include <FslBase/Log/Log.hpp>
#include <FslDemoApp/Base/Service/Persistent/IPersistentDataManager.hpp>
#include <FslDemoHost/Base/Service/DemoAppControl/IDemoAppControlEx.hpp>
#include <FslDemoHost/Base/Service/Test/TestService.hpp>
#include <FslDemoService/Graphics/IGraphicsService.hpp>
#include <FslGraphics/ImageFormatUtil.hpp>
#include <cassert>
#include <iomanip>
#include <sstream>

namespace Fsl
{
  TestService::TestService(const ServiceProvider& serviceProvider)
    : ThreadLocalService(serviceProvider)
    , m_config(TestScreenshotNameScheme::FrameNumber, ImageFormat::Png, 0, "Screenshot")
    , m_frameCounter(0)
    , m_screenshotNameCounter(0)
    , m_userScreenshotCount(0)

  {
    m_demoAppControlService = serviceProvider.Get<IDemoAppControlEx>();
    m_persistentDataManager = serviceProvider.Get<IPersistentDataManager>();
    m_graphicsService = serviceProvider.TryGet<IGraphicsService>();
  }


  TestService::~TestService() = default;


  uint32_t TestService::GetScreenshotFrequency() const
  {
    return m_config.Frequency;
  }


  void TestService::SetScreenshotFrequency(const uint32_t frequency)
  {
    m_config.Frequency = frequency;
  }


  TestScreenshotNameScheme TestService::GetScreenshotNameScheme() const
  {
    return m_config.NamingScheme;
  }


  void TestService::SetScreenshotNameScheme(const TestScreenshotNameScheme scheme)
  {
    m_config.NamingScheme = scheme;
  }


  void TestService::SetScreenshotConfig(const TestScreenshotConfig& config)
  {
    m_config = config;
  }


  void TestService::OnFrameSwapCompleted()
  {
    ++m_frameCounter;

    const bool saveNow = (m_config.Frequency > 0 && (m_frameCounter % m_config.Frequency) == 0);
    const bool hasRequest = m_demoAppControlService->HasScreenshotRequest();

    if ((saveNow || hasRequest) && m_graphicsService)
    {
      if (hasRequest)
      {
        m_demoAppControlService->ClearScreenshotRequestRequest();
      }

      // Reset the update timer since the screenshot functionality is slow
      // We do this to allow a perfect timed capture of the frames
      m_demoAppControlService->RequestUpdateTimerReset();

      // Capture a screenshot and save it as a bmp
      m_graphicsService->Capture(m_screenshot, PixelFormat::B8G8R8A8_UINT);

      if (m_screenshot.IsValid())
      {
        if (saveNow)
        {
          SaveBitmap();
        }

        if (hasRequest)
        {
          // When not auto saving we use the same routine to save images
          if (m_config.Frequency <= 0)
          {
            SaveBitmap();
          }
          else
          {
            // If we are auto saving images then the user screenshots get a different name
            ++m_userScreenshotCount;
            std::stringstream stringStream;
            stringStream << "UserScreenshot-" << std::setw(10) << std::setfill('0') << m_userScreenshotCount
                         << ImageFormatUtil::GetDefaultExtension(m_config.Format);
            m_persistentDataManager->Write(stringStream.str(), m_screenshot, m_config.Format);
          }
        }
      }
    }
  }


  void TestService::SaveBitmap()
  {
    std::stringstream stringStream;

    if (m_config.NamingScheme == TestScreenshotNameScheme::Sequential)
    {
      ++m_screenshotNameCounter;
      stringStream << m_config.FilenamePrefix << '-' << std::setw(10) << std::setfill('0') << m_screenshotNameCounter
                   << ImageFormatUtil::GetDefaultExtension(m_config.Format);
    }
    else if (m_config.NamingScheme == TestScreenshotNameScheme::FrameNumber)
    {
      stringStream << m_config.FilenamePrefix << '-' << std::setw(10) << std::setfill('0') << m_frameCounter
                   << ImageFormatUtil::GetDefaultExtension(m_config.Format);
    }
    else
    {
      stringStream << m_config.FilenamePrefix << ImageFormatUtil::GetDefaultExtension(m_config.Format);
    }

    m_persistentDataManager->Write(stringStream.str(), m_screenshot, m_config.Format);
  }
}
