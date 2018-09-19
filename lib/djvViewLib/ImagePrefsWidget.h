//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
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

#include <djvViewLib/AbstractPrefsWidget.h>

#include <memory>

class QListWidgetItem;

namespace djv
{
    namespace ViewLib
    {
        //! \class ImagePrefsWidget
        //!
        //! This class provides the image group preferences widget.
        class ImagePrefsWidget : public AbstractPrefsWidget
        {
            Q_OBJECT

        public:
            ImagePrefsWidget(Context *);

            virtual ~ImagePrefsWidget();

            virtual void resetPreferences();

        private Q_SLOTS:
            void frameStoreFileReloadCallback(bool);
            void mirrorHCallback(bool);
            void mirrorVCallback(bool);
            void scaleCallback(int);
            void rotateCallback(int);
            void colorProfileCallback(bool);
            void displayProfileCallback(int);
            void displayProfileCallback(QListWidgetItem *);
            void addDisplayProfileCallback();
            void removeDisplayProfileCallback();
            void moveDisplayProfileUpCallback();
            void moveDisplayProfileDownCallback();
            void channelCallback(int);

            void sizeUpdate();
            void widgetUpdate();

        private:
            DJV_PRIVATE_COPY(ImagePrefsWidget);

            struct Private;
            std::unique_ptr<Private> _p;
        };

    } // namespace ViewLib
} // namespace djv