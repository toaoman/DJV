//------------------------------------------------------------------------------
// Copyright (c) 2004-2019 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvViewLib/SettingsSystem.h>

#include <djvViewLib/SettingsDialog.h>

#include <djvUIComponents/DisplaySettingsWidget.h>
#include <djvUIComponents/LanguageSettingsWidget.h>
#include <djvUIComponents/PPMSettingsWidget.h>
#include <djvUIComponents/PaletteSettingsWidget.h>
#if defined(JPEG_FOUND)
#include <djvUIComponents/JPEGSettingsWidget.h>
#endif
#if defined(TIFF_FOUND)
#include <djvUIComponents/TIFFSettingsWidget.h>
#endif

#include <djvUI/Action.h>
#include <djvUI/ButtonGroup.h>
#include <djvUI/EventSystem.h>
#include <djvUI/RowLayout.h>
#include <djvUI/SoloLayout.h>
#include <djvUI/ScrollWidget.h>
#include <djvUI/Window.h>

#include <djvCore/Context.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct SettingsSystem::Private
        {
            std::map<std::string, std::shared_ptr<UI::Action> > actions;
            std::vector<std::shared_ptr<UI::ISettingsWidget> > settingsWidgets;
            std::shared_ptr<SettingsDialog> settingsDialog;
            std::map<std::string, std::shared_ptr<ValueObserver<bool> > > clickedObservers;
        };

        void SettingsSystem::_init(Context * context)
        {
            IViewSystem::_init("djv::ViewLib::SettingsSystem", context);

            DJV_PRIVATE_PTR();
            p.settingsWidgets.push_back(UI::DisplaySettingsWidget::create(context));
            p.settingsWidgets.push_back(UI::LanguageSettingsWidget::create(context));
            p.settingsWidgets.push_back(UI::PPMSettingsWidget::create(context));
            p.settingsWidgets.push_back(UI::PaletteSettingsWidget::create(context));
#if defined(JPEG_FOUND)
            p.settingsWidgets.push_back(UI::JPEGSettingsWidget::create(context));
#endif
#if defined(TIFF_FOUND)
            p.settingsWidgets.push_back(UI::TIFFSettingsWidget::create(context));
#endif
        }

        SettingsSystem::SettingsSystem() :
            _p(new Private)
        {}

        SettingsSystem::~SettingsSystem()
        {}

        std::shared_ptr<SettingsSystem> SettingsSystem::create(Context * context)
        {
            auto out = std::shared_ptr<SettingsSystem>(new SettingsSystem);
            out->_init(context);
            return out;
        }

        void SettingsSystem::showSettings()
        {
            DJV_PRIVATE_PTR();
            auto context = getContext();
            if (auto eventSystem = context->getSystemT<UI::EventSystem>())
            {
                if (auto window = eventSystem->observeCurrentWindow()->get())
                {
                    if (!p.settingsDialog)
                    {
                        p.settingsDialog = SettingsDialog::create(context);
                        auto weak = std::weak_ptr<SettingsSystem>(std::dynamic_pointer_cast<SettingsSystem>(shared_from_this()));
                        p.settingsDialog->setCloseCallback(
                            [weak, context]
                        {
                            if (auto system = weak.lock())
                            {
                                system->_p->settingsDialog->hide();
                            }
                        });
                    }
                    window->addChild(p.settingsDialog);
                    p.settingsDialog->show();
                }
            }
        }
        
        std::map<std::string, std::shared_ptr<UI::Action> > SettingsSystem::getActions()
        {
            return _p->actions;
        }

        std::vector<std::shared_ptr<UI::ISettingsWidget> > SettingsSystem::getSettingsWidgets()
        {
            return _p->settingsWidgets;
        }
        
    } // namespace ViewLib
} // namespace djv

