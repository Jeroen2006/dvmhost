/**
* Digital Voice Modem - Host Software
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Host Software
*
*/
//
// Based on code from the CRUD project. (https://github.com/venediktov/CRUD)
// Licensed under the BPL-1.0 License (https://opensource.org/license/bsl1-0-html)
//
/*
*   Copyright (c) 2003-2013 Christopher M. Kohlhoff
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
#include "Defines.h"
#include "network/rest/http/HTTPReply.h"

using namespace network::rest::http;

#include <string>

namespace status_strings {
    const std::string ok = "HTTP/1.0 200 OK\r\n";
    const std::string created = "HTTP/1.0 201 Created\r\n";
    const std::string accepted = "HTTP/1.0 202 Accepted\r\n";
    const std::string no_content = "HTTP/1.0 204 No Content\r\n";
    const std::string multiple_choices = "HTTP/1.0 300 Multiple Choices\r\n";
    const std::string moved_permanently = "HTTP/1.0 301 Moved Permanently\r\n";
    const std::string moved_temporarily = "HTTP/1.0 302 Moved Temporarily\r\n";
    const std::string not_modified = "HTTP/1.0 304 Not Modified\r\n";
    const std::string bad_request = "HTTP/1.0 400 Bad Request\r\n";
    const std::string unauthorized = "HTTP/1.0 401 Unauthorized\r\n";
    const std::string forbidden = "HTTP/1.0 403 Forbidden\r\n";
    const std::string not_found = "HTTP/1.0 404 Not Found\r\n";
    const std::string internal_server_error = "HTTP/1.0 500 Internal Server Error\r\n";
    const std::string not_implemented = "HTTP/1.0 501 Not Implemented\r\n";
    const std::string bad_gateway = "HTTP/1.0 502 Bad Gateway\r\n";
    const std::string service_unavailable = "HTTP/1.0 503 Service Unavailable\r\n";

    asio::const_buffer toBuffer(HTTPReply::StatusType status)
    {
        switch (status)
        {
        case HTTPReply::OK:
            return asio::buffer(ok);
        case HTTPReply::CREATED:
            return asio::buffer(created);
        case HTTPReply::ACCEPTED:
            return asio::buffer(accepted);
        case HTTPReply::NO_CONTENT:
            return asio::buffer(no_content);
        case HTTPReply::MULTIPLE_CHOICES:
            return asio::buffer(multiple_choices);
        case HTTPReply::MOVED_PERMANENTLY:
            return asio::buffer(moved_permanently);
        case HTTPReply::MOVED_TEMPORARILY:
            return asio::buffer(moved_temporarily);
        case HTTPReply::NOT_MODIFIED:
            return asio::buffer(not_modified);
        case HTTPReply::BAD_REQUEST:
            return asio::buffer(bad_request);
        case HTTPReply::UNAUTHORIZED:
            return asio::buffer(unauthorized);
        case HTTPReply::FORBIDDEN:
            return asio::buffer(forbidden);
        case HTTPReply::NOT_FOUND:
            return asio::buffer(not_found);
        case HTTPReply::INTERNAL_SERVER_ERROR:
            return asio::buffer(internal_server_error);
        case HTTPReply::NOT_IMPLEMENTED:
            return asio::buffer(not_implemented);
        case HTTPReply::BAD_GATEWAY:
            return asio::buffer(bad_gateway);
        case HTTPReply::SERVICE_UNAVAILABLE:
            return asio::buffer(service_unavailable);
        default:
            return asio::buffer(internal_server_error);
        }
    }
} // namespace status_strings

namespace misc_strings {
    const char name_value_separator[] = { ':', ' ' };
    const char crlf[] = { '\r', '\n' };
} // namespace misc_strings

namespace stock_replies {
    const char ok[] = "";
    const char created[] =
        "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created</h1></body>"
        "</html>";
    const char accepted[] =
        "<html>"
        "<head><title>Accepted</title></head>"
        "<body><h1>202 Accepted</h1></body>"
        "</html>";
    const char no_content[] =
        "<html>"
        "<head><title>No Content</title></head>"
        "<body><h1>204 Content</h1></body>"
        "</html>";
    const char multiple_choices[] =
        "<html>"
        "<head><title>Multiple Choices</title></head>"
        "<body><h1>300 Multiple Choices</h1></body>"
        "</html>";
    const char moved_permanently[] =
        "<html>"
        "<head><title>Moved Permanently</title></head>"
        "<body><h1>301 Moved Permanently</h1></body>"
        "</html>";
    const char moved_temporarily[] =
        "<html>"
        "<head><title>Moved Temporarily</title></head>"
        "<body><h1>302 Moved Temporarily</h1></body>"
        "</html>";
    const char not_modified[] =
        "<html>"
        "<head><title>Not Modified</title></head>"
        "<body><h1>304 Not Modified</h1></body>"
        "</html>";
    const char bad_request[] =
        "<html>"
        "<head><title>Bad Request</title></head>"
        "<body><h1>400 Bad Request</h1></body>"
        "</html>";
    const char unauthorized[] =
        "<html>"
        "<head><title>Unauthorized</title></head>"
        "<body><h1>401 Unauthorized</h1></body>"
        "</html>";
    const char forbidden[] =
        "<html>"
        "<head><title>Forbidden</title></head>"
        "<body><h1>403 Forbidden</h1></body>"
        "</html>";
    const char not_found[] =
        "<html>"
        "<head><title>Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
        "</html>";
    const char internal_server_error[] =
        "<html>"
        "<head><title>Internal Server Error</title></head>"
        "<body><h1>500 Internal Server Error</h1></body>"
        "</html>";
    const char not_implemented[] =
        "<html>"
        "<head><title>Not Implemented</title></head>"
        "<body><h1>501 Not Implemented</h1></body>"
        "</html>";
    const char bad_gateway[] =
        "<html>"
        "<head><title>Bad Gateway</title></head>"
        "<body><h1>502 Bad Gateway</h1></body>"
        "</html>";
    const char service_unavailable[] =
        "<html>"
        "<head><title>Service Unavailable</title></head>"
        "<body><h1>503 Service Unavailable</h1></body>"
        "</html>";

    std::string to_string(HTTPReply::StatusType status)
    {
        switch (status)
        {
        case HTTPReply::OK:
            return ok;
        case HTTPReply::CREATED:
            return created;
        case HTTPReply::ACCEPTED:
            return accepted;
        case HTTPReply::NO_CONTENT:
            return no_content;
        case HTTPReply::MULTIPLE_CHOICES:
            return multiple_choices;
        case HTTPReply::MOVED_PERMANENTLY:
            return moved_permanently;
        case HTTPReply::MOVED_TEMPORARILY:
            return moved_temporarily;
        case HTTPReply::NOT_MODIFIED:
            return not_modified;
        case HTTPReply::BAD_REQUEST:
            return bad_request;
        case HTTPReply::UNAUTHORIZED:
            return unauthorized;
        case HTTPReply::FORBIDDEN:
            return forbidden;
        case HTTPReply::NOT_FOUND:
            return not_found;
        case HTTPReply::INTERNAL_SERVER_ERROR:
            return internal_server_error;
        case HTTPReply::NOT_IMPLEMENTED:
            return not_implemented;
        case HTTPReply::BAD_GATEWAY:
            return bad_gateway;
        case HTTPReply::SERVICE_UNAVAILABLE:
            return service_unavailable;
        default:
            return internal_server_error;
        }
    }
} // namespace stock_replies

// ---------------------------------------------------------------------------
//  Public Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Convert the reply into a vector of buffers. The buffers do not own the
/// underlying memory blocks, therefore the reply object must remain valid and
/// not be changed until the write operation has completed.
/// </summary>
std::vector<asio::const_buffer> HTTPReply::toBuffers()
{
    std::vector<asio::const_buffer> buffers;
    buffers.push_back(status_strings::toBuffer(status));

    for (std::size_t i = 0; i < headers.size(); ++i) {
        HTTPHeader& h = headers[i];
        buffers.push_back(asio::buffer(h.name));
        buffers.push_back(asio::buffer(misc_strings::name_value_separator));
        buffers.push_back(asio::buffer(h.value));
        buffers.push_back(asio::buffer(misc_strings::crlf));
    }

    buffers.push_back(asio::buffer(misc_strings::crlf));
    buffers.push_back(asio::buffer(content));
    return buffers;
}

/// <summary>
///
/// </summary>
/// <param name="obj"></param>
/// <param name="s"></param>
void HTTPReply::reply(json::object obj, HTTPReply::StatusType s)
{
    json::value v = json::value(obj);
    std::string json = v.serialize();
    reply(json, s, "text/json");
}

/// <summary>
///
/// </summary>
/// <param name="c"></param>
/// <param name="s"></param>
/// <param name="contentType"></param>
void HTTPReply::reply(std::string c, HTTPReply::StatusType s, std::string contentType)
{
    content = c;
    status = s;
    ensureDefaultHeaders(contentType);
}

// ---------------------------------------------------------------------------
//  Static Members
// ---------------------------------------------------------------------------

/// <summary>
/// Get a stock reply.
/// </summary>
/// <param name="status"></param>
/// <param name="contentType"></param>
HTTPReply HTTPReply::stockReply(HTTPReply::StatusType status, const std::string contentType)
{
    HTTPReply rep;
    rep.status = status;

    if (status != HTTPReply::NO_CONTENT) {
        rep.content = stock_replies::to_string(status);
        rep.ensureDefaultHeaders(contentType);
    }
    
    return rep;
}

// ---------------------------------------------------------------------------
//  Private Members
// ---------------------------------------------------------------------------

/// <summary>
///
/// </summary>
/// <param name="contentType"></param>
void HTTPReply::ensureDefaultHeaders(std::string contentType)
{
    headers.push_back(HTTPHeader("Content-Length", std::to_string(content.size())));
    headers.push_back(HTTPHeader("Content-Type", contentType));
    headers.push_back(HTTPHeader("Server", std::string((__EXE_NAME__ "/" __VER__))));
}
