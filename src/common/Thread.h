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
*   Copyright (C) 2023 by Bryan Biedenkapp N2PLL
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
#if !defined(__THREAD_H__)
#define __THREAD_H__

#include "common/Defines.h"

#include <string>
#include <pthread.h>

// ---------------------------------------------------------------------------
//  Class Declaration
//      Implements a simple threading mechanism.
// ---------------------------------------------------------------------------

class HOST_SW_API Thread {
public:
    /// <summary>Initializes a new instance of the Thread class.</summary>
    Thread();
    /// <summary>Finalizes a instance of the Thread class.</summary>
    virtual ~Thread();

    /// <summary>Starts the thread execution.</summary>
    virtual bool run();

    /// <summary>User-defined function to run for the thread main.</summary>
    virtual void entry() = 0;

    /// <summary></summary>
    virtual void wait();

    /// <summary></summary>
    virtual void setName(std::string name);

    /// <summary></summary>
    static void sleep(uint32_t ms);

private:
    pthread_t m_thread;

    /// <summary></summary>
    static void* helper(void* arg);

public:
    /// <summary>Flag indicating if the thread was started.</summary>
    __PROTECTED_READONLY_PROPERTY_PLAIN(bool, started);
};

#endif // __THREAD_H__
