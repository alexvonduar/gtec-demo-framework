#ifndef FSLBASE_SYSTEM_PLATFORM_PLATFORMTHREAD_HPP
#define FSLBASE_SYSTEM_PLATFORM_PLATFORMTHREAD_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
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
#include <functional>
#include <memory>

namespace Fsl
{
  class IThreadContext;
  class PlatformThreadImpl;

  //! @brief Extremely simple platform independent thread.
  //! @note Be very careful with what is used here as its the bottom layer.
  //! @note WARNING: this interface is experimental
  class PlatformThread
  {
    std::shared_ptr<PlatformThreadImpl> m_impl;

  public:
    PlatformThread(const PlatformThread&) = delete;
    PlatformThread& operator=(const PlatformThread&) = delete;

    //! @brief Prepare a new thread
    //! @param fnRun the function that should be executed on the new thread.
    //! @param threadContext a user defined 'context' object that is supplied to the function.
    PlatformThread(const std::function<void(const std::shared_ptr<IThreadContext>)>& fnRun, const std::shared_ptr<IThreadContext>& threadContext);
    virtual ~PlatformThread();

    //! @brief Blocks the calling thread until 'this thread' terminates
    void Join();


    //! @brief Sleep the currently active thread the given amount of milliseconds
    static void SleepMilliseconds(const uint32_t milliseconds);
  };
}

#endif
