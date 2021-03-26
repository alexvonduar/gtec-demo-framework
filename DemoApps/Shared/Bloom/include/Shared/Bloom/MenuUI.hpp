#ifndef SHARED_BLOOM_MENUUI_HPP
#define SHARED_BLOOM_MENUUI_HPP
/****************************************************************************************************************************************************
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
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

#include <FslDemoApp/Base/DemoAppConfig.hpp>
#include <FslSimpleUI/App/UIDemoAppExtension.hpp>
#include <FslSimpleUI/Base/Control/SimpleImageButton.hpp>
#include <FslSimpleUI/Base/Control/SliderAndFmtValueLabel.hpp>
#include <FslSimpleUI/Base/Control/Switch.hpp>
#include <FslSimpleUI/Base/Layout/FillLayout.hpp>
#include <FslSimpleUI/Base/Layout/StackLayout.hpp>


namespace Fsl
{
  class OptionParser;

  class MenuUI : public UI::EventListener
  {
    UI::CallbackEventListenerScope m_uiEventListener;
    std::shared_ptr<UIDemoAppExtension> m_uiExtension;

    std::shared_ptr<OptionParser> m_optionParser;

    std::shared_ptr<UI::FillLayout> m_rootLayout;

    std::shared_ptr<UI::Switch> m_cbMenuRotate;
    std::shared_ptr<UI::Switch> m_cbMenuBlur;
    std::shared_ptr<UI::Switch> m_cbMenuBright;
    std::shared_ptr<UI::Switch> m_cbMenuFinalScene;
    std::shared_ptr<UI::Switch> m_cbMenuFinalBloom;
    std::shared_ptr<UI::Switch> m_cbMenuShowBuffers;
    std::shared_ptr<UI::Switch> m_cbScaleInputSequentially;

    std::shared_ptr<UI::SliderAndFmtValueLabel<float>> m_sliderBlur;
    std::shared_ptr<UI::SliderAndFmtValueLabel<float>> m_sliderBlend;

    int32_t m_sceneId;
    bool m_rotateEnabled;
    bool m_blurEnabled;
    bool m_brightPassEnabled;
    bool m_renderFinalScene;
    bool m_renderFinalBloom;
    bool m_showBuffersEnabled;
    bool m_scaleInputSequentially;
    float m_kernelWeightMod;
    float m_blendLevel;

  public:
    explicit MenuUI(const DemoAppConfig& config);
    ~MenuUI() override;

    std::shared_ptr<UIDemoAppExtension> GetUIDemoAppExtension() const
    {
      return m_uiExtension;
    }

    void OnKeyEvent(const KeyEvent& event);

    void OnSelect(const UI::RoutedEventArgs& args, const std::shared_ptr<UI::WindowSelectEvent>& theEvent) override;
    void OnContentChanged(const UI::RoutedEventArgs& args, const std::shared_ptr<UI::WindowContentChangedEvent>& theEvent) override;

    int32_t GetSceneId() const
    {
      return m_sceneId;
    }

    bool IsRotateEnabled() const
    {
      return m_rotateEnabled;
    }
    void SetRotateEnabled(bool enabled);

    bool IsBlurEnabled() const
    {
      return m_blurEnabled;
    }
    void SetBlurEnabled(bool enabled);

    bool IsBrightPassEnabled() const
    {
      return m_brightPassEnabled;
    }
    void SetBrightPassEnabled(bool enabled);

    bool IsFinalSceneEnabled() const
    {
      return m_renderFinalScene;
    }
    void SetFinalSceneEnabled(bool enabled);

    bool IsFinalBloomEnabled() const
    {
      return m_renderFinalBloom;
    }
    void SetFinalBloomEnabled(bool enabled);

    bool IsShowBuffersEnabled() const
    {
      return m_showBuffersEnabled;
    }
    void SetShowBuffersEnabled(bool enabled);

    bool IsScaleInputSequentiallyEnabled() const
    {
      return m_scaleInputSequentially;
    }
    void SetScaleInputSequentiallyEnabled(bool enabled);

    float GetKernelWeightRange() const
    {
      return 1.0f;
    }

    float GetKernelWeightMod() const
    {
      return m_kernelWeightMod;
    }
    float GetBlendLevel() const
    {
      return m_blendLevel;
    }

    void Draw();

  private:
    void BuildUI();
    void UpdateControls();
  };
}

#endif
