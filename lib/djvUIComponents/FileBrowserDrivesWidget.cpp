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

#include <djvUIComponents/FileBrowserPrivate.h>

#include <djvUI/Action.h>
#include <djvUI/GroupBox.h>
#include <djvUI/ListButton.h>
#include <djvUI/RowLayout.h>

#include <djvCore/DrivesModel.h>
#include <djvCore/FileInfo.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace FileBrowser
        {
            struct DrivesWidget::Private
            {
                std::vector<FileSystem::Path> drives;
                std::shared_ptr<VerticalLayout> layout;
                std::shared_ptr<VerticalLayout> itemLayout;
                std::function<void(const FileSystem::Path &)> callback;
                std::shared_ptr<ListObserver<FileSystem::Path> > drivesObserver;
            };

            void DrivesWidget::_init(const std::shared_ptr<FileSystem::DrivesModel>& model, const std::shared_ptr<Context>& context)
            {
                UI::Widget::_init(context);

                setClassName("djv::UI::FileBrowser::DrivesWidget");

                DJV_PRIVATE_PTR();

                p.itemLayout = VerticalLayout::create(context);
                p.itemLayout->setSpacing(MetricsRole::None);

                p.layout = VerticalLayout::create(context);
                p.layout->addChild(p.itemLayout);
                p.layout->setStretch(p.itemLayout, RowStretch::Expand);
                addChild(p.layout);

                auto weak = std::weak_ptr<DrivesWidget>(std::dynamic_pointer_cast<DrivesWidget>(shared_from_this()));
                auto contextWeak = std::weak_ptr<Context>(context);
                p.drivesObserver = ListObserver<FileSystem::Path>::create(
                    model->observeDrives(),
                    [weak, contextWeak](const std::vector<FileSystem::Path> & value)
                    {
                        if (auto context = contextWeak.lock())
                        {
                            if (auto widget = weak.lock())
                            {
                                widget->_p->itemLayout->clearChildren();
                                for (const auto& i : value)
                                {
                                    auto button = ListButton::create(context);
                                    std::string s = i.getFileName();
                                    if (s.empty())
                                    {
                                        s = i;
                                    }
                                    button->setText(s);
                                    button->setInsideMargin(MetricsRole::Margin);

                                    widget->_p->itemLayout->addChild(button);

                                    const auto path = i;
                                    button->setClickedCallback(
                                        [weak, path]
                                        {
                                            if (auto widget = weak.lock())
                                            {
                                                if (widget->_p->callback)
                                                {
                                                    widget->_p->callback(path);
                                                }
                                            }
                                        });
                                }
                            }
                        }
                    });
            }

            DrivesWidget::DrivesWidget() :
                _p(new Private)
            {}

            DrivesWidget::~DrivesWidget()
            {}

            std::shared_ptr<DrivesWidget> DrivesWidget::create(const std::shared_ptr<FileSystem::DrivesModel>& model, const std::shared_ptr<Context>& context)
            {
                auto out = std::shared_ptr<DrivesWidget>(new DrivesWidget);
                out->_init(model, context);
                return out;
            }

            void DrivesWidget::setCallback(const std::function<void(const FileSystem::Path &)> & value)
            {
                _p->callback = value;
            }

            void DrivesWidget::_preLayoutEvent(Event::PreLayout & event)
            {
                _setMinimumSize(_p->layout->getMinimumSize());
            }

            void DrivesWidget::_layoutEvent(Event::Layout & event)
            {
                _p->layout->setGeometry(getGeometry());
            }

        } // namespace FileBrowser
    } // namespace UI
} // namespace djv
