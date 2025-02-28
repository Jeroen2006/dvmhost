/**
* Digital Voice Modem - Host Software
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Host Software
*
*/
//
// Based on code from the MMDVMHost project. (https://github.com/g4klx/MMDVMHost)
// Licensed under the GPLv2 License (https://opensource.org/licenses/GPL-2.0)
//
/*
*   Copyright (C) 2017-2023 by Bryan Biedenkapp N2PLL
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
#include "Host.h"

using namespace modem;

// ---------------------------------------------------------------------------
//  Private Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Helper to interrupt a running NXDN control channel.
/// </summary>
/// <param name="control"></param>
void Host::interruptNXDNControl(nxdn::Control* control)
{
    if (control != nullptr) {
        LogDebug(LOG_HOST, "interrupt NXDN control, m_state = %u", m_state);
        control->setCCHalted(true);         

        if (m_nxdnBcastDurationTimer.isRunning() && !m_nxdnBcastDurationTimer.isPaused()) {
            m_nxdnBcastDurationTimer.pause();
        }
    }
}

/// <summary>
/// Helper to read NXDN frames from modem.
/// </summary>
/// <param name="control"></param>
/// <param name="afterReadCallback"></param>
void Host::readFramesNXDN(nxdn::Control* control, std::function<void()>&& afterReadCallback)
{
    uint8_t data[nxdn::NXDN_FRAME_LENGTH_BYTES * 2U];

    if (control != nullptr) {
        uint32_t len = m_modem->readNXDNFrame(data);
        if (len > 0U) {
            if (m_state == STATE_IDLE) {
                // process NXDN frames
                bool ret = control->processFrame(data, len);
                if (ret) {
                    m_modeTimer.setTimeout(m_rfModeHang);
                    setState(STATE_NXDN);

                    if (afterReadCallback != nullptr) {
                        afterReadCallback();
                    }
                }
            }
            else if (m_state == STATE_NXDN) {
                // process NXDN frames
                bool ret = control->processFrame(data, len);
                if (ret) {
                    m_modeTimer.start();
                }
            }
            else if (m_state != HOST_STATE_LOCKOUT) {
                LogWarning(LOG_HOST, "NXDN modem data received, state = %u", m_state);
            }
        }
    }
}

/// <summary>
/// Helper to write NXDN frames to modem.
/// </summary>
/// <param name="control"></param>
/// <param name="afterWriteCallback"></param>
void Host::writeFramesNXDN(nxdn::Control* control, std::function<void()>&& afterWriteCallback)
{
    uint8_t data[nxdn::NXDN_FRAME_LENGTH_BYTES * 2U];

    // check if there is space on the modem for NXDN frames,
    // if there is read frames from the NXDN controller and write it
    // to the modem
    if (control != nullptr) {
        bool ret = m_modem->hasNXDNSpace();
        if (ret) {
            uint32_t len = control->getFrame(data);
            if (len > 0U) {
                // if the state is idle; set to NXDN and start mode timer
                if (m_state == STATE_IDLE) {
                    m_modeTimer.setTimeout(m_netModeHang);
                    setState(STATE_NXDN);
                }

                // if the state is NXDN; write NXDN data
                if (m_state == STATE_NXDN) {
                    m_modem->writeNXDNFrame(data, len);

                    if (afterWriteCallback != nullptr) {
                        afterWriteCallback();
                    }

                    m_modeTimer.start();
                }

                m_lastDstId = control->getLastDstId();
                m_lastSrcId = control->getLastSrcId();
            }
        }
    }
}
