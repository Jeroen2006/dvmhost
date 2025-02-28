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
*   Copyright (C) 2021 by Jonathan Naylor G4KLX
*   Copyright (C) 2021 by Bryan Biedenkapp N2PLL
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
#if !defined(__UDP_PORT_H__)
#define __UDP_PORT_H__

#include "Defines.h"
#include "common/network/UDPSocket.h"
#include "common/RingBuffer.h"
#include "modem/port/IModemPort.h"

#include <string>

namespace modem
{
    namespace port
    {
        // ---------------------------------------------------------------------------
        //  Class Declaration
        //      This class implements low-level routines to communicate over UDP.
        // ---------------------------------------------------------------------------

        class HOST_SW_API UDPPort : public IModemPort {
        public:
            /// <summary>Initializes a new instance of the UDPPort class.</summary>
            UDPPort(const std::string& modemAddress, uint16_t modemPort);
            /// <summary>Finalizes a instance of the UDPPort class.</summary>
            virtual ~UDPPort();

            /// <summary>Opens a connection to the serial port.</summary>
            bool open();

            /// <summary>Reads data from the serial port.</summary>
            int read(uint8_t* buffer, uint32_t length);
            /// <summary>Writes data to the serial port.</summary>
            int write(const uint8_t* buffer, uint32_t length);

            /// <summary>Closes the connection to the serial port.</summary>
            void close();

        protected:
            network::UDPSocket m_socket;

            sockaddr_storage m_addr;
            uint32_t m_addrLen;

            RingBuffer<uint8_t> m_buffer;
        };
    } // namespace port
} // namespace Modem

#endif // __UDP_PORT_H__
