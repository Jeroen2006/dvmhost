/**
* Digital Voice Modem - Common Library
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Common Library
*
*/
/*
*   Copyright (C) 2023 by Bryan Biedenkapp N2PLL
*
*   Permission is hereby granted, free of charge, to any person or organization
*   obtaining a copy of the software and accompanying documentation covered by
*   this license (the “Software”) to use, reproduce, display, distribute, execute,
*   and transmit the Software, and to prepare derivative works of the Software, and
*   to permit third-parties to whom the Software is furnished to do so, all subject
*   to the following:
*
*   The copyright notices in the Software and this entire statement, including the
*   above license grant, this restriction and the following disclaimer, must be included
*   in all copies of the Software, in whole or in part, and all derivative works of the
*   Software, unless such copies or derivative works are solely in the form of
*   machine-executable object code generated by a source language processor.
*
*   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
*   PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE
*   DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
*   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
*   OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#if !defined(__REST_HTTP__HTTP_CLIENT_H__)
#define __REST_HTTP__HTTP_CLIENT_H__

#include "common/Defines.h"
#include "common/network/rest/http/ClientConnection.h"
#include "common/network/rest/http/HTTPRequestHandler.h"
#include "common/Thread.h"

#include <asio.hpp>

#include <thread>
#include <string>
#include <signal.h>
#include <utility>
#include <memory>
#include <mutex>

namespace network
{
    namespace rest
    {
        namespace http
        {

            // ---------------------------------------------------------------------------
            //  Class Declaration
            //      This class implements top-level routines of the HTTP client.
            // ---------------------------------------------------------------------------

            template<typename RequestHandlerType, template<class> class ConnectionImpl = ClientConnection>
            class HTTPClient : private Thread {
            public:
                /// <summary>Initializes a new instance of the HTTPClient class.</summary>
                HTTPClient(const std::string& address, uint16_t port) :
                    m_address(address),
                    m_port(port),
                    m_connection(nullptr),
                    m_ioContext(),
                    m_socket(m_ioContext),
                    m_requestHandler()
                {
                    /* stub */
                }
                /// <summary>Initializes a copy instance of the HTTPClient class.</summary>
                HTTPClient(const HTTPClient&) = delete;
                /// <summary>Finalizes a instance of the HTTPClient class.</summary>
                ~HTTPClient()
                {
                    if (m_connection != nullptr) {
                        close();
                    }
                }

                /// <summary></summary>
                HTTPClient& operator=(const HTTPClient&) = delete;

                /// <summary>Helper to set the HTTP request handlers.</summary>
                template<typename Handler>
                void setHandler(Handler&& handler)
                {
                    m_requestHandler = RequestHandlerType(std::forward<Handler>(handler));
                }

                /// <summary>Send HTTP request to HTTP server.</summary>
                bool request(HTTPPayload& request)
                {
                    if (m_completed) {
                        return false;
                    }

                    asio::post(m_ioContext, [this, request]() {
                        std::lock_guard<std::mutex> guard(m_lock);
                        {
                            if (m_connection != nullptr) {
                                m_connection->send(request);
                            }
                        }
                    });

                    return true;
                }

                /// <summary>Opens connection to the network.</summary>
                bool open()
                {
                    if (m_completed) {
                        return false;
                    }

                    return run();
                }

                /// <summary>Closes connection to the network.</summary>
                void close()
                {
                    if (m_completed) {
                        return;
                    }

                    m_completed = true;
                    m_ioContext.stop();

                    wait();
                }

            private:
                /// <summary></summary>
                virtual void entry()
                {
                    if (m_completed) {
                        return;
                    }

                    asio::ip::tcp::resolver resolver(m_ioContext);
                    auto endpoints = resolver.resolve(m_address, std::to_string(m_port));

                    try {
                        connect(endpoints);

                        // the entry() call will block until all asynchronous operations
                        // have finished
                        m_ioContext.run();
                    }
                    catch (std::exception&) { /* stub */ }

                    if (m_connection != nullptr) {
                        m_connection->stop();
                    }
                }

                /// <summary>Perform an asynchronous connect operation.</summary>
                void connect(asio::ip::basic_resolver_results<asio::ip::tcp>& endpoints)
                {
                    asio::connect(m_socket, endpoints);

                    m_connection = new_unique(ConnectionType, std::move(m_socket), m_requestHandler);
                    m_connection->start();
                }

                std::string m_address;
                uint16_t m_port;

                typedef ConnectionImpl<RequestHandlerType> ConnectionType;

                std::unique_ptr<ConnectionType> m_connection;

                bool m_completed = false;
                asio::io_context m_ioContext;

                asio::ip::tcp::socket m_socket;

                RequestHandlerType m_requestHandler;

                std::mutex m_lock;
            };
        } // namespace http
    } // namespace rest
} // namespace network

#endif // __REST_HTTP__HTTP_CLIENT_H__
