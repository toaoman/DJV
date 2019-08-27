//------------------------------------------------------------------------------
// Copyright (c) 2019 Darby Johnston
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

#pragma once

#include <djvUI/UI.h>

#include <djvCore/INumericValueModel.h>
#include <djvCore/Range.h>

#include <chrono>

namespace djv
{
    namespace UI
    {
        //! This enumeration provides the numeric widget keyboard shortcuts.
        enum class NumericWidgetKey
        {
            None,
            Home,
            End,
            Up,
            Right,
            Down,
            Left,
            PageUp,
            PageDown
        };
        NumericWidgetKey fromGLFWKey(int);

        //! This class provides an interface for numeric widgets.
        template<typename T>
        class INumericWidget
        {
        public:
            inline virtual ~INumericWidget() = 0;

            inline const Core::Range::Range<T>& getRange() const;
            inline void setRange(const Core::Range::Range<T>&);

            inline T getValue() const;
            inline void setValue(T);
            inline void setValueCallback(const std::function<void(T)>&);

            inline const std::shared_ptr<Core::INumericValueModel<T> >& getModel() const;
            inline virtual void setModel(const std::shared_ptr<Core::INumericValueModel<T> >&);

        protected:
            inline void _doCallback();
            inline bool _keyPress(NumericWidgetKey);

            std::shared_ptr<Core::INumericValueModel<T> > _model;
            std::function<void(T)> _callback;
        };

        //! This class provides an interface for numeric editor widgets.
        template<typename T>
        class INumericEdit : public INumericWidget<T>
        {
        public:
            inline virtual ~INumericEdit() = 0;
        };

        //! This class provides an interface for numeric slider widgets.
        template<typename T>
        class INumericSlider : public INumericWidget<T>
        {
        public:
            inline virtual ~INumericSlider() = 0;

        protected:
            inline void _pointerMove(float, std::chrono::milliseconds);
            inline void _buttonPress(float, std::chrono::milliseconds);
            inline void _buttonRelease(std::chrono::milliseconds);
            inline void _valueUpdate();

            virtual float _valueToPos(T) const = 0;
            virtual T _posToValue(float) const = 0;

            T _value = static_cast<T>(0);
        };

    } // namespace UI
} // namespace djv

#include <djvUI/INumericWidgetInline.h>