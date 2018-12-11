//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
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

#include <djvUI/Icon.h>

#include <djvUI/System.h>

#include <djvAV/IconSystem.h>
#include <djvAV/Image.h>
#include <djvAV/PixelData.h>
#include <djvAV/Render2DSystem.h>

#include <djvCore/Path.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct Icon::Private
        {
            Path path;
            ColorRole iconColorRole = ColorRole::Foreground;
            std::future<AV::Pixel::Info> infoFuture;
            std::future<std::shared_ptr<AV::Image> > imageFuture;
            AV::Pixel::Info info;
            std::shared_ptr<AV::Image> image;
            size_t hash = 0;
        };

        void Icon::_init(Context * context)
        {
            Widget::_init(context);

            setName("djv::UI::Icon");
        }

        Icon::Icon() :
            _p(new Private)
        {}

        Icon::~Icon()
        {}

        std::shared_ptr<Icon> Icon::create(Context * context)
        {
            auto out = std::shared_ptr<Icon>(new Icon);
            out->_init(context);
            return out;
        }

        std::shared_ptr<Icon> Icon::create(const Path& path, Context * context)
        {
            auto out = Icon::create(context);
            out->setIcon(path);
            return out;
        }

        const Path& Icon::getIcon() const
        {
            return _p->path;
        }

        void Icon::setIcon(const Path& value)
        {
            if (value == _p->path)
                return;
            _p->path = value;
            _p->infoFuture = _getIconSystem()->getInfo(_p->path);
            _p->imageFuture = _getIconSystem()->getImage(_p->path);
            _p->hash = 0;
            Memory::hashCombine(_p->hash, _p->path.get());
        }

        ColorRole Icon::getIconColorRole() const
        {
            return _p->iconColorRole;
        }

        void Icon::setIconColorRole(ColorRole value)
        {
            _p->iconColorRole = value;
        }

        void Icon::_preLayoutEvent(PreLayoutEvent& event)
        {
            const auto style = _getStyle();
            if (_p->infoFuture.valid())
            {
                _p->info = _p->infoFuture.get();
            }
            _setMinimumSize(glm::vec2(_p->info.size) + getMargin().getSize(style));
        }

        void Icon::_paintEvent(PaintEvent& event)
        {
            Widget::_paintEvent(event);

            auto renderSystem = _getRenderSystem();
            const auto style = _getStyle();
            const BBox2f& g = getMargin().bbox(getGeometry(), style);
            const glm::vec2 c = g.getCenter();

            // Draw the icon.
            if (_p->imageFuture.valid())
            {
                _p->image = _p->imageFuture.get();
            }
            if (_p->image->isValid())
            {
                const glm::vec2& size = _p->info.size;
                glm::vec2 pos = glm::vec2(0.f, 0.f);
                switch (getHAlign())
                {
                case HAlign::Center:
                case HAlign::Fill:   pos.x = c.x - size.x / 2.f; break;
                case HAlign::Left:   pos.x = g.min.x; break;
                case HAlign::Right:  pos.x = g.max.x - size.x; break;
                default: break;
                }
                switch (getVAlign())
                {
                case VAlign::Center:
                case VAlign::Fill:   pos.y = c.y - size.y / 2.f; break;
                case VAlign::Top:    pos.y = g.min.y; break;
                case VAlign::Bottom: pos.y = g.max.y - size.y; break;
                default: break;
                }
                if (_p->iconColorRole != ColorRole::None)
                {
                    renderSystem->setFillColor(style->getColor(_p->iconColorRole));
                    renderSystem->drawFilledImage(_p->image, pos, false, _p->hash);
                }
                else
                {
                    renderSystem->setFillColor(AV::Color(1.f, 1.f, 1.f));
                    renderSystem->drawImage(_p->image, pos, false, _p->hash);
                }
            }
        }

    } // namespace UI
} // namespace djv