/**
* Digital Voice Modem - Common Library
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Common Library
*
*/
/*
*   Copyright (C) 2022 by Bryan Biedenkapp N2PLL
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
#include "Defines.h"
#include "p25/lc/tsbk/OSP_MOT_GRG_VCH_GRANT.h"
#include "Log.h"
#include "Utils.h"

using namespace p25::lc::tsbk;
using namespace p25::lc;
using namespace p25;

#include <cassert>
#include <cmath>

// ---------------------------------------------------------------------------
//  Public Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Initializes a new instance of the OSP_MOT_GRG_VCH_GRANT class.
/// </summary>
OSP_MOT_GRG_VCH_GRANT::OSP_MOT_GRG_VCH_GRANT() : TSBK()
{
    m_lco = TSBK_OSP_MOT_GRG_VCH_GRANT;
}

/// <summary>
/// Decode a trunking signalling block.
/// </summary>
/// <param name="data"></param>
/// <param name="rawTSBK"></param>
/// <returns>True, if TSBK was decoded, otherwise false.</returns>
bool OSP_MOT_GRG_VCH_GRANT::decode(const uint8_t* data, bool rawTSBK)
{
    assert(data != NULL);

    /* stub */

    return true;
}

/// <summary>
/// Encode a trunking signalling block.
/// </summary>
/// <param name="data"></param>
/// <param name="rawTSBK"></param>
/// <param name="noTrellis"></param>
void OSP_MOT_GRG_VCH_GRANT::encode(uint8_t* data, bool rawTSBK, bool noTrellis)
{
    assert(data != NULL);

    ulong64_t tsbkValue = 0U;

    m_mfId = P25_MFG_MOT;

    if (m_patchSuperGroupId != 0U) {
        tsbkValue = 0U;                                                             // Priority
        tsbkValue = (tsbkValue << 4) + m_siteData.channelId();                      // Channel ID
        tsbkValue = (tsbkValue << 12) + m_siteData.channelNo();                     // Channel Number
        tsbkValue = (tsbkValue << 16) + m_patchSuperGroupId;                        // Patch Supergroup Address
        tsbkValue = (tsbkValue << 24) + m_srcId;                                    // Source Radio Address
    }
    else {
        LogError(LOG_P25, "TSBK::encode(), invalid values for TSBK_OSP_MOT_GRG_VCH_GRANT, patchSuperGroupId = $%02X", m_patchSuperGroupId);
        return; // blatently ignore creating this TSBK
    }

    std::unique_ptr<uint8_t[]> tsbk = TSBK::fromValue(tsbkValue);
    TSBK::encode(data, tsbk.get(), rawTSBK, noTrellis);
}

/// <summary>
/// Returns a string that represents the current TSBK.
/// </summary>
/// <param name="isp"></param>
/// <returns></returns>
std::string OSP_MOT_GRG_VCH_GRANT::toString(bool isp)
{
    return std::string("TSBK_OSP_MOT_GRG_VCH_GRANT (Group Regroup Voice Channel Grant)");
}

// ---------------------------------------------------------------------------
//  Private Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Internal helper to copy the the class.
/// </summary>
/// <param name="data"></param>
void OSP_MOT_GRG_VCH_GRANT::copy(const OSP_MOT_GRG_VCH_GRANT& data)
{
    TSBK::copy(data);

    m_patchSuperGroupId = data.m_patchSuperGroupId;
}
