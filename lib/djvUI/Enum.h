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

#pragma once

#include <djvCore/Enum.h>

namespace djv
{
    //! \todo [1.0 M] Add a dial widget.
    //! \todo [1.0 XL] Add tooltips.
    //! \todo [1.0 XL] Add menus.
    //! \todo [1.0 L] Add a color picker.
    //! \todo [1.0 L] Add keyboard navigation.
    //! \todo [1.0 L] Add UI audio.
    namespace UI
    {
        //! This enumeration provides orientations.
        enum class Orientation
        {
            Horizontal,
            Vertical,

            Count,
            First = Horizontal
        };
        DJV_ENUM_HELPERS(Orientation);
        Orientation getOpposite(Orientation);

        //! This enumeration provides sides.
        enum class Side
        {
            Left,
            Top,
            Right,
            Bottom,

            Count,
            First = Left
        };
        DJV_ENUM_HELPERS(Side);

        //! This enumeration provides layout expansion directions.
        enum class Expand
        {
            None,
            Horizontal,
            Vertical,
            Both,

            Count,
            First = None
        };
        DJV_ENUM_HELPERS(Expand);

        //! This enumeration provides horizontal alignment.
        enum class HAlign
        {
            Center,
            Left,
            Right,
            Fill,
            
            Count,
            First = Center
        };
        DJV_ENUM_HELPERS(HAlign);

        //! This enumeration provides vertical alignment.
        enum class VAlign
        {
            Center,
            Top,
            Bottom,
            Fill,

            Count,
            First = Center
        };
        DJV_ENUM_HELPERS(VAlign);

        //! This enumeration provides horizontal text alignment.
        enum class TextHAlign
        {
            Center,
            Left,
            Right,
            
            Count,
            First = Center
        };
        DJV_ENUM_HELPERS(TextHAlign);

        //! This enumeration provides vertical text alignment.
        enum class TextVAlign
        {
            Center,
            Top,
            Bottom,
            Baseline,

            Count,
            First = Center
        };
        DJV_ENUM_HELPERS(TextVAlign);

        //! This enumeration provides selection types.
        enum class SelectionType
        {
            None,
            Single,
            Radio,
            Multiple,

            Count,
            First = Single
        };
        DJV_ENUM_HELPERS(SelectionType);

        //! This enumeration provides sort orders.
        enum class SortOrder
        {
            Forward,
            Reverse,

            Count,
            First = Forward
        };
        DJV_ENUM_HELPERS(SortOrder);

        //! This enumeration provides button types.
        enum class ButtonType
        {
            Push,
            Toggle,
            Radio,

            Count,
            First = Push
        };
        DJV_ENUM_HELPERS(ButtonType);

        //! This enumeration provides font faces.
        enum class FontFace
        {
            Regular,
            Bold,

            Count,
            First = Regular
        };
        DJV_ENUM_HELPERS(FontFace);

    } // namespace UI

    DJV_ENUM_SERIALIZE_HELPERS(UI::Orientation);
    DJV_ENUM_SERIALIZE_HELPERS(UI::Side);
    DJV_ENUM_SERIALIZE_HELPERS(UI::Expand);
    DJV_ENUM_SERIALIZE_HELPERS(UI::HAlign);
    DJV_ENUM_SERIALIZE_HELPERS(UI::VAlign);
    DJV_ENUM_SERIALIZE_HELPERS(UI::TextHAlign);
    DJV_ENUM_SERIALIZE_HELPERS(UI::TextVAlign);
    DJV_ENUM_SERIALIZE_HELPERS(UI::SelectionType);
    DJV_ENUM_SERIALIZE_HELPERS(UI::SortOrder);
    DJV_ENUM_SERIALIZE_HELPERS(UI::ButtonType);
    DJV_ENUM_SERIALIZE_HELPERS(UI::FontFace);

} // namespace Gp
