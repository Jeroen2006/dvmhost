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
#if !defined(__DMR_SYNC_H__)
#define __DMR_SYNC_H__

#include "common/Defines.h"

namespace dmr
{
    // ---------------------------------------------------------------------------
    //  Class Declaration
    //      Helper class for generating DMR sync data.
    // ---------------------------------------------------------------------------

    class HOST_SW_API Sync {
    public:
        /// <summary>Helper to append DMR data sync bytes to the passed buffer.</summary>
        static void addDMRDataSync(uint8_t* data, bool duplex);
        /// <summary>Helper to append DMR voice sync bytes to the passed buffer.</summary>
        static void addDMRAudioSync(uint8_t* data, bool duplex);
    };
} // namespace dmr

#endif // __DMR_SYNC_H__
