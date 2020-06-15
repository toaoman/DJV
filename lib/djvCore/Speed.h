// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvCore/Enum.h>
#include <djvCore/Rational.h>

namespace djv
{
    namespace Core
    {
        namespace Time
        {
            //! This enumeration provides common speeds.
            enum class FPS
            {
                _1,
                _3,
                _6,
                _12,
                _15,
                _16,
                _18,
                _23_976,
                _24,
                _25,
                _29_97,
                _30,
                _50,
                _59_94,
                _60,
                _120,
                _240,

                Count,
                First = _1
            };
            DJV_ENUM_HELPERS(FPS);

            //! \name Default Speed
            ///@{

            FPS getDefaultSpeed();
            void setDefaultSpeed(FPS);

            ///@}

            //! \name Utilities
            ///@{

            Math::Rational fromSpeed(FPS);
            Math::Rational fromSpeed(float);
            bool toSpeed(const Math::Rational&, FPS&);

            ///@}

        } // namespace Time
    } // namespace Core

    DJV_ENUM_SERIALIZE_HELPERS(Core::Time::FPS);

    rapidjson::Value toJSON(Core::Time::FPS, rapidjson::Document::AllocatorType&);

    //! Throws:
    //! - std::exception
    void fromJSON(const rapidjson::Value&, Core::Time::FPS&);

} // namespace djv
