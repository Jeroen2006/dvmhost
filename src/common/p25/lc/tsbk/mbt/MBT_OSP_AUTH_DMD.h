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
#if !defined(__P25_LC_TSBK__MBT_OSP_AUTH_DMD_H__)
#define  __P25_LC_TSBK__MBT_OSP_AUTH_DMD_H__

#include "common/Defines.h"
#include "common/p25/lc/AMBT.h"

namespace p25
{
    namespace lc
    {
        namespace tsbk
        {
            // ---------------------------------------------------------------------------
            //  Class Declaration
            //      Implements AUTH DMD - Authentication Demand
            // ---------------------------------------------------------------------------

            class HOST_SW_API MBT_OSP_AUTH_DMD : public AMBT {
            public:
                /// <summary>Initializes a new instance of the MBT_OSP_AUTH_DMD class.</summary>
                MBT_OSP_AUTH_DMD();
                /// <summary>Finalizes a instance of the MBT_OSP_AUTH_DMD class.</summary>
                ~MBT_OSP_AUTH_DMD();

                /// <summary>Decode a alternate trunking signalling block.</summary>
                bool decodeMBT(const data::DataHeader& dataHeader, const data::DataBlock* blocks) override;
                /// <summary>Encode a alternate trunking signalling block.</summary>
                void encodeMBT(data::DataHeader& dataHeader, uint8_t* pduUserData) override;

                /// <summary>Returns a string that represents the current TSBK.</summary>
                virtual std::string toString(bool isp = false) override;

                /// <summary>Sets the authentication random seed.</summary>
                void setAuthRS(const uint8_t* rs);
                /// <summary>Gets the authentication random seed.</summary>
                void getAuthRS(uint8_t* rs) const;

                /// <summary>Sets the authentication random challenge.</summary>
                void setAuthRC(const uint8_t* rc);
                /// <summary>Gets the authentication random challenge.</summary>
                void getAuthRC(uint8_t* rc) const;

            private:
                /** Authentication data */
                uint8_t* m_authRS;
                uint8_t* m_authRC;

                __COPY(MBT_OSP_AUTH_DMD);
            };
        } // namespace tsbk
    } // namespace lc
} // namespace p25

#endif // __P25_LC_TSBK__MBT_OSP_AUTH_DMD_H__
