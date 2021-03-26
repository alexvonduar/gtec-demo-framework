#ifndef FSLDEMOSERVICE_PROFILER_SCOPEDPROFILERCUSTOMCOUNTERHANDLE_HPP
#define FSLDEMOSERVICE_PROFILER_SCOPEDPROFILERCUSTOMCOUNTERHANDLE_HPP
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

#include <FslBase/BasicTypes.hpp>
#include <FslBase/Exceptions.hpp>
#include <FslDemoService/Profiler/IProfilerService.hpp>
#include <FslDemoService/Profiler/ProfilerCustomCounterHandle.hpp>
#include <memory>
#include <utility>

namespace Fsl
{
  class IProfilerService;

  //! @brief Used to manage a ProfilerCustomCounterHandle so that it is destroyed once this object goes out of scope
  class ScopedProfilerCustomCounterHandle
  {
    std::shared_ptr<IProfilerService> m_service;
    ProfilerCustomCounterHandle m_handle;

  public:
    ScopedProfilerCustomCounterHandle(const ScopedProfilerCustomCounterHandle&) = delete;
    ScopedProfilerCustomCounterHandle& operator=(const ScopedProfilerCustomCounterHandle&) = delete;

    ScopedProfilerCustomCounterHandle() = default;
    ScopedProfilerCustomCounterHandle(std::shared_ptr<IProfilerService> service, const ProfilerCustomCounterHandle& handle)
      : m_service(std::move(service))
      , m_handle(handle)
    {
      if (!m_service)
      {
        throw std::invalid_argument("service can no be null");
      }
    }

    ~ScopedProfilerCustomCounterHandle()
    {
      Reset();
    }

    bool IsValid() const
    {
      return m_handle.IsValid();
    }

    void Reset()
    {
      if (m_handle.IsValid())
      {
        m_service->DestroyCustomCounter(m_handle);
        m_handle = ProfilerCustomCounterHandle::Invalid();
      }
    }

    void Reset(const std::shared_ptr<IProfilerService>& service, const ProfilerCustomCounterHandle& handle)
    {
      Reset();
      if (!service)
      {
        throw std::invalid_argument("service can no be null");
      }
      m_service = service;
      m_handle = handle;
    }

    // implicit conversion
    operator ProfilerCustomCounterHandle() const    // NOLINT(google-explicit-constructor)
    {
      return m_handle;
    }
  };
}

#endif
