#ifndef FSLDEMOAPP_BASE_OVERLAY_DEMOAPPPROFILEROVERLAY_HPP
#define FSLDEMOAPP_BASE_OVERLAY_DEMOAPPPROFILEROVERLAY_HPP
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

#include <FslDemoApp/Base/Overlay/DemoAppProfilerGraph.hpp>
#include <FslDemoApp/Base/DemoAppStatsFlags.hpp>
#include <FslDemoApp/Shared/Host/DemoWindowMetrics.hpp>
#include <FslDemoService/Profiler/ProfilerCustomCounterHandle.hpp>
#include <FslGraphics/Color.hpp>
#include <fmt/format.h>
#include <list>
#include <memory>
#include <string>
#include <array>

namespace Fsl
{
  class IBasic2D;
  class ICpuStatsService;
  class IGraphicsService;
  class IProfilerService;
  struct Point2;
  struct ProfilerCustomCounterDesc;
  class ServiceProvider;

  class DemoAppProfilerOverlay
  {
    struct CustomRecord
    {
      ProfilerCustomCounterHandle Handle;
      std::string Name;
      Color TheColor;
      std::shared_ptr<DemoAppProfilerGraph> Graph;
      // 10 so we can hold the largest positive int32_t and +3 for '%','d' and terminating zero;
      std::array<char, 10 + 3> FormatString{};

      CustomRecord()
      {
        FormatString[0] = '{';
        FormatString[1] = '}';
        FormatString[2] = 0;
      }

      CustomRecord(const ProfilerCustomCounterHandle& handle, const ProfilerCustomCounterDesc& desc, std::shared_ptr<DemoAppProfilerGraph> graph);
    };

    std::shared_ptr<IProfilerService> m_profilerService;
    std::shared_ptr<IGraphicsService> m_graphicsService;
    std::shared_ptr<ICpuStatsService> m_cpuStatsService;

    std::weak_ptr<IBasic2D> m_basic2D;

    DemoAppStatsFlags m_logStatsFlags;

    DemoAppProfilerGraph m_graphTotal;
    DemoAppProfilerGraph m_graphUpdate;
    DemoAppProfilerGraph m_graphDraw;
    DemoAppProfilerGraph m_graphCPU;

    uint32_t m_customConfigurationRevision;
    std::list<CustomRecord> m_customCounters;

    fmt::memory_buffer m_scracthpad;

  public:
    DemoAppProfilerOverlay(const ServiceProvider& serviceProvider, const DemoAppStatsFlags& logStatsFlags);
    ~DemoAppProfilerOverlay();

    void Draw(const DemoWindowMetrics& windowMetrics);

  private:
    void MaintainCachedCustomEntries();
    bool IsNewHandle(const ProfilerCustomCounterHandle& handle) const;
    void UpdateAndDrawCustomCounters(const std::shared_ptr<IBasic2D>& basic2D, const Vector2& dstPos);
  };
}

#endif
