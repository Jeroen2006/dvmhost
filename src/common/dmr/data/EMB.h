/**
* Digital Voice Modem - Common Library
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Common Library
*
*/
//
// Based on code from the MMDVMHost project. (https://github.com/g4klx/MMDVMHost)
// Licensed under the GPLv2 License (https://opensource.org/licenses/GPL-2.0)
//
/*
*   Copyright (C) 2015,2016 by Jonathan Naylor G4KLX
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#if !defined(__DMR_DATA__EMB_H__)
#define __DMR_DATA__EMB_H__

#include "common/Defines.h"

namespace dmr
{
    namespace data
    {
        // ---------------------------------------------------------------------------
        //  Class Declaration
        //      Represents a DMR embedded signalling data.
        // ---------------------------------------------------------------------------

        class HOST_SW_API EMB {
        public:
            /// <summary>Initializes a new instance of the EMB class.</summary>
            EMB();
            /// <summary>Finalizes a instance of the EMB class.</summary>
            ~EMB();

            /// <summary>Decodes DMR embedded signalling data.</summary>
            void decode(const uint8_t* data);
            /// <summary>Encodes DMR embedded signalling data.</summary>
            void encode(uint8_t* data) const;

        public:
            /// <summary>DMR access color code.</summary>
            __PROPERTY(uint8_t, colorCode, ColorCode);

            /// <summary>Flag indicating whether the privacy indicator is set or not.</summary>
            __PROPERTY(bool, PI, PI);

            /// <summary>Link control start/stop.</summary>
            __PROPERTY(uint8_t, LCSS, LCSS);
        };
    } // namespace data
} // namespace dmr

#endif // __DMR_DATA__EMB_H__
