// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvUIComponents/IOSettingsWidget.h>

#include <djvUIComponents/IOSettings.h>

#include <djvUI/FormLayout.h>
#include <djvUI/IntSlider.h>
#include <djvUI/SettingsSystem.h>

#include <djvSystem/Context.h>

#include <djvMath/NumericValueModels.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct IOThreadsSettingsWidget::Private
        {
            std::shared_ptr<Numeric::IntSlider> threadCountSlider;
            std::shared_ptr<FormLayout> layout;
            std::shared_ptr<Observer::Value<size_t> > threadCountObserver;
        };

        void IOThreadsSettingsWidget::_init(const std::shared_ptr<System::Context>& context)
        {
            ISettingsWidget::_init(context);

            DJV_PRIVATE_PTR();
            setClassName("djv::UI::IOThreadsSettingsWidget");

            p.threadCountSlider = Numeric::IntSlider::create(context);
            p.threadCountSlider->setRange(Math::IntRange(2, 64));

            p.layout = FormLayout::create(context);
            p.layout->addChild(p.threadCountSlider);
            addChild(p.layout);

            auto weak = std::weak_ptr<IOThreadsSettingsWidget>(std::dynamic_pointer_cast<IOThreadsSettingsWidget>(shared_from_this()));
            auto contextWeak = std::weak_ptr<System::Context>(context);
            p.threadCountSlider->setValueCallback(
                [weak, contextWeak](int value)
                {
                    if (auto context = contextWeak.lock())
                    {
                        if (auto widget = weak.lock())
                        {
                            auto settingsSystem = context->getSystemT<UI::Settings::SettingsSystem>();
                            if (auto ioSettings = settingsSystem->getSettingsT<Settings::IO>())
                            {
                                ioSettings->setThreadCount(static_cast<size_t>(value));
                            }
                        }
                    }
                });

            auto settingsSystem = context->getSystemT<UI::Settings::SettingsSystem>();
            if (auto ioSettings = settingsSystem->getSettingsT<Settings::IO>())
            {
                p.threadCountObserver = Observer::Value<size_t>::create(
                    ioSettings->observeThreadCount(),
                    [weak](size_t value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->threadCountSlider->setValue(static_cast<int>(value));
                        }
                    });
            }
        }

        IOThreadsSettingsWidget::IOThreadsSettingsWidget() :
            _p(new Private)
        {}

        std::shared_ptr<IOThreadsSettingsWidget> IOThreadsSettingsWidget::create(const std::shared_ptr<System::Context>& context)
        {
            auto out = std::shared_ptr<IOThreadsSettingsWidget>(new IOThreadsSettingsWidget);
            out->_init(context);
            return out;
        }

        std::string IOThreadsSettingsWidget::getSettingsGroup() const
        {
            return DJV_TEXT("settings_title_io");
        }

        std::string IOThreadsSettingsWidget::getSettingsSortKey() const
        {
            return "d";
        }

        void IOThreadsSettingsWidget::setLabelSizeGroup(const std::weak_ptr<Text::LabelSizeGroup>& value)
        {
            _p->layout->setLabelSizeGroup(value);
        }

        void IOThreadsSettingsWidget::_initEvent(System::Event::Init & event)
        {
            ISettingsWidget::_initEvent(event);
            DJV_PRIVATE_PTR();
            if (event.getData().text)
            {
                p.layout->setText(p.threadCountSlider, _getText(DJV_TEXT("settings_io_thread_count")) + ":");
            }
        }

    } // namespace UI
} // namespace djv

