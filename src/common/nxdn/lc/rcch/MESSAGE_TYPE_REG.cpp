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
#include "nxdn/lc/rcch/MESSAGE_TYPE_REG.h"
#include "Log.h"
#include "Utils.h"

using namespace nxdn::lc::rcch;
using namespace nxdn::lc;
using namespace nxdn;

#include <cassert>
#include <cmath>

// ---------------------------------------------------------------------------
//  Public Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Initializes a new instance of the MESSAGE_TYPE_REG class.
/// </summary>
MESSAGE_TYPE_REG::MESSAGE_TYPE_REG() : RCCH()
{
    m_messageType = RCCH_MESSAGE_TYPE_REG;
}

/// <summary>
/// Decode layer 3 data.
/// </summary>
/// <param name="data"></param>
/// <param name="length"></param>
/// <param name="offset"></param>
void MESSAGE_TYPE_REG::decode(const uint8_t* data, uint32_t length, uint32_t offset)
{
    assert(data != NULL);

    uint8_t rcch[NXDN_RCCH_LC_LENGTH_BYTES + 4U];
    ::memset(rcch, 0x00U, NXDN_RCCH_LC_LENGTH_BYTES + 4U);

    RCCH::decode(data, rcch, length, offset);

    m_regOption = rcch[1U] >> 3;                                                    // Registration Option
    m_locId = (uint16_t)((rcch[2U] << 8) | rcch[3U]) & 0xFFFFU;                     // Location ID
    m_srcId = (uint16_t)((rcch[4U] << 8) | rcch[5U]) & 0xFFFFU;                     // Source Radio Address
    m_dstId = (uint16_t)((rcch[6U] << 8) | rcch[7U]) & 0xFFFFU;                     // Target Radio Address
    // bryanb: maybe process subscriber type? (byte 8 and 9)
    m_version = rcch[10U];                                                          // Version
}

/// <summary>
/// Encode a control signalling block.
/// </summary>
/// <param name="data"></param>
/// <param name="length"></param>
/// <param name="offset"></param>
void MESSAGE_TYPE_REG::encode(uint8_t* data, uint32_t length, uint32_t offset)
{
    assert(data != NULL);

    uint8_t rcch[NXDN_RCCH_LC_LENGTH_BYTES + 4U];
    ::memset(rcch, 0x00U, NXDN_RCCH_LC_LENGTH_BYTES + 4U);

    rcch[2U] = (m_siteData.locId() >> 8) & 0xFFU;                                   // ...
    rcch[3U] = (m_siteData.locId() >> 0) & 0xFFU;                                   // ...
    rcch[4U] = (m_srcId >> 8U) & 0xFFU;                                             // Source Radio Address
    rcch[5U] = (m_srcId >> 0U) & 0xFFU;                                             // ...
    rcch[6U] = (m_dstId >> 8U) & 0xFFU;                                             // Target Radio Address
    rcch[7U] = (m_dstId >> 0U) & 0xFFU;                                             // ...
    rcch[8U] = m_causeRsp;                                                          // Cause (MM)

    RCCH::encode(data, rcch, length, offset);
}

/// <summary>
/// Returns a string that represents the current RCCH.
/// </summary>
/// <param name="isp"></param>
/// <returns></returns>
std::string MESSAGE_TYPE_REG::toString(bool isp)
{
    if (isp) return std::string("RCCH_MESSAGE_TYPE_REG (Registration Request)");
    else return std::string("RCCH_MESSAGE_TYPE_REG (Registration Response)");
}
