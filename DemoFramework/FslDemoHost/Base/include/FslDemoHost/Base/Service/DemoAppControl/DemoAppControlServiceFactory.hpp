#ifndef FSLDEMOHOST_BASE_SERVICE_DEMOAPPCONTROL_DEMOAPPCONTROLSERVICEFACTORY_HPP
#define FSLDEMOHOST_BASE_SERVICE_DEMOAPPCONTROL_DEMOAPPCONTROLSERVICEFACTORY_HPP
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

#include <FslService/Impl/ServiceType/Local/ThreadLocalSingletonServiceFactoryTemplate.hpp>
#include <FslDemoHost/Base/Service/DemoAppControl/DemoAppControlService.hpp>

namespace Fsl
{
  class DemoAppControlServiceFactory : public IThreadLocalSingletonServiceFactory
  {
    int m_defaultExitCode;

  public:
    DemoAppControlServiceFactory(const int defaultExitCode)
      : m_defaultExitCode(defaultExitCode)
    {
    }


    std::shared_ptr<AServiceOptionParser> GetOptionParser() const override
    {
      return std::shared_ptr<AServiceOptionParser>();
    }


    ServiceCaps::Flags GetFlags() const override
    {
      return ServiceCaps::Default;
    }

    void FillInterfaceType(ServiceSupportedInterfaceDeque& rServiceInterfaceTypeDeque) const override
    {
      rServiceInterfaceTypeDeque.push_back(std::type_index(typeid(IDemoAppControl)));
      rServiceInterfaceTypeDeque.push_back(std::type_index(typeid(IDemoAppControlEx)));
    }

    std::shared_ptr<IService> Allocate(ServiceProvider& provider) override
    {
      return std::make_shared<DemoAppControlService>(provider, m_defaultExitCode);
    }
  };
}

#endif
