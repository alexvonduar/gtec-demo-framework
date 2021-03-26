#ifndef PS_GLES3_PARTICLESYSTEM_PARTICLESYSTEMTWOARRAYS_HPP
#define PS_GLES3_PARTICLESYSTEM_PARTICLESYSTEMTWOARRAYS_HPP
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

#include "Draw/IParticleDraw.hpp"
#include "Emit/IParticleEmitter.hpp"
#include "IParticleContainer.hpp"
#include "IParticleSystem.hpp"
#include "Particle.hpp"
#include <deque>
#include <memory>
#include <vector>

namespace Fsl
{
  struct DemoTime;

  class ParticleSystemTwoArrays
    : public IParticleSystem
    , public IParticleContainer
  {
    struct ParticleRecord
    {
      Particle Data;
      ParticleRecord() = default;
    };

    std::vector<ParticleRecord> m_particles1;
    std::vector<ParticleRecord> m_particles2;
    ParticleRecord* m_pCurrent;
    ParticleRecord* m_pOld;

    std::deque<std::shared_ptr<IParticleEmitter>> m_emitters;
    std::shared_ptr<IParticleDraw> m_particleDraw;
    Vector3 m_gravity;
    uint32_t m_particleCount;

  public:
    static constexpr uint32_t ParticleRecordSize()
    {
      return static_cast<uint32_t>(sizeof(ParticleRecord));
    }

    ParticleSystemTwoArrays(std::shared_ptr<IParticleDraw> particleDraw, const std::size_t capacity);

    uint32_t GetParticleCount() const override;
    void AddEmitter(const std::shared_ptr<IParticleEmitter>& emitter) override;
    void Update(const DemoTime& demoTime) override;
    void Draw(const ParticleDrawContext& context) override;

    //! Added by IParticleContainer
    void AddParticles(const Particle* pParticles, const std::size_t& count) override;
  };
}

#endif
