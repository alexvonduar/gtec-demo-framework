#ifndef FSLSIMPLEUI_BASE_CONTROL_BUTTON_HPP
#define FSLSIMPLEUI_BASE_CONTROL_BUTTON_HPP
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

#include <FslSimpleUI/Base/Control/ContentControl.hpp>

namespace Fsl
{
  namespace UI
  {
    //! A more generic implementation of a button
    //! For most purposes the 'button' code is identical to the ButtonBase implementation
    class Button : public ContentControl
    {
      bool m_isClickable;
      bool m_isDown;

    public:
      Button(const std::shared_ptr<BaseWindowContext>& context);
      virtual bool IsClickable() const;
      virtual void SetClickable(const bool value);

    protected:
      void OnClickInput(const RoutedEventArgs& args, const std::shared_ptr<WindowInputClickEvent>& theEvent) override;

      //! @brief Check if the button is down at the moment
      bool IsDown() const
      {
        return m_isDown;
      }

      //! @brief Called when the button is pressed
      virtual void Down()
      {
      }

      //! @brief Called when the button is released
      //! @param wasCanceled true if the press was canceled.
      virtual void Up(bool wasCanceled)
      {
        FSL_PARAM_NOT_USED(wasCanceled);
      }
    };
  }
}

#endif
