/**
* Digital Voice Modem - Common Library
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Common Library
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
#include "network/rest/http/HTTPLexer.h"
#include "network/rest/http/HTTPPayload.h"
#include "Log.h"

using namespace network::rest::http;

#include <cctype>

// ---------------------------------------------------------------------------
//  Public Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Initializes a new instance of the HTTPLexer class.
/// </summary>
/// <param name="clientLexer"></param>
HTTPLexer::HTTPLexer(bool clientLexer) :
    m_headers(),
    m_clientLexer(clientLexer),
    m_state(METHOD_START)
{
    if (m_clientLexer) {
        m_state = HTTP_VERSION_H;
    }
}

/// <summary>Reset to initial parser state.</summary>
void HTTPLexer::reset()
{
    m_state = METHOD_START;
    if (m_clientLexer) {
        m_state = HTTP_VERSION_H;
    }

    m_headers = std::vector<LexedHeader>();
}

// ---------------------------------------------------------------------------
//  Private Class Members
// ---------------------------------------------------------------------------

/// <summary>
/// Handle the next character of input.
/// </summary>
/// <param name="req"></param>
/// <param name="input"></param>
/// <returns></returns>
HTTPLexer::ResultType HTTPLexer::consume(HTTPPayload& req, char input)
{
    switch (m_state)
    {
    /*
    ** HTTP Method
    */

    case METHOD_START:
        if (!isChar(input) || isControl(input) || isSpecial(input))
            return BAD;
        else {
            m_state = METHOD;
            req.method.push_back(input);
            return INDETERMINATE;
        }
    case METHOD:
        if (input == ' ') {
            m_state = URI;
            return INDETERMINATE;
        }
        else if (!isChar(input) || isControl(input) || isSpecial(input)) {
            return BAD;
        }
        else {
            req.method.push_back(input);
            return INDETERMINATE;
        }

    /*
    ** URI
    */

    case URI:
        if (input == ' ') {
            m_state = HTTP_VERSION_H;
            return INDETERMINATE;
        }
        else if (isControl(input)) {
            return BAD;
        }
        else {
            req.uri.push_back(input);
            return INDETERMINATE;
        }

    /*
    ** HTTP/1.0
    ** HTTP/1.0 200 OK
    */
    case HTTP_VERSION_H:
        if (input == 'H') {
            m_state = HTTP_VERSION_T_1;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_T_1:
        if (input == 'T') {
            m_state = HTTP_VERSION_T_2;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_T_2:
        if (input == 'T') {
            m_state = HTTP_VERSION_P;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_P:
        if (input == 'P') {
            m_state = HTTP_VERSION_SLASH;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_SLASH:
        if (input == '/') {
            req.httpVersionMajor = 0;
            req.httpVersionMinor = 0;
            m_state = HTTP_VERSION_MAJOR_START;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_MAJOR_START:
        if (isDigit(input)) {
            req.httpVersionMajor = req.httpVersionMajor * 10 + input - '0';
            m_state = HTTP_VERSION_MAJOR;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_MAJOR:
        if (input == '.') {
            m_state = HTTP_VERSION_MINOR_START;
            return INDETERMINATE;
        }
        else if (isDigit(input)) {
            req.httpVersionMajor = req.httpVersionMajor * 10 + input - '0';
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_MINOR_START:
        if (isDigit(input)) {
            req.httpVersionMinor = req.httpVersionMinor * 10 + input - '0';
            m_state = HTTP_VERSION_MINOR;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_VERSION_MINOR:
        if (input == '\r') {
            m_state = EXPECTING_NEWLINE_1;
            if (m_clientLexer) {
                return BAD;
            }
            else {
                return INDETERMINATE;
            }
        }
        else if (input == ' ') {
            if (m_clientLexer) {
                m_state = HTTP_STATUS_1;
                return INDETERMINATE;
            }
            else {
                return BAD;
            }
        }
        else if (isDigit(input)) {
            req.httpVersionMinor = req.httpVersionMinor * 10 + input - '0';
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_STATUS_1:
        if (isDigit(input)) {
            m_state = HTTP_STATUS_2;
            m_status = m_status * 10 + input - '0';
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_STATUS_2:
        if (isDigit(input)) {
            m_state = HTTP_STATUS_3;
            m_status = m_status * 10 + input - '0';
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_STATUS_3:
        if (isDigit(input)) {
            m_state = HTTP_STATUS_END;
            m_status = m_status * 10 + input - '0';
            req.status = (HTTPPayload::StatusType)m_status;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_STATUS_END:
        if (input == ' ') {
            m_state = HTTP_STATUS_MESSAGE;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }
    case HTTP_STATUS_MESSAGE_START:
        if (!isChar(input) || isControl(input) || isSpecial(input)) {
            return BAD;
        }
        else {
            m_state = HTTP_STATUS_MESSAGE;
            return INDETERMINATE;
        }
    case HTTP_STATUS_MESSAGE:
        if (input == '\r') {
            m_state = EXPECTING_NEWLINE_1;
            return INDETERMINATE;
        }
        else if (input == ' ') {
            m_state = HTTP_STATUS_MESSAGE;
            return INDETERMINATE;
        }
        else if (!isChar(input) || isControl(input) || isSpecial(input)) {
            return BAD;
        }
        else {
            return INDETERMINATE;
        }

    case EXPECTING_NEWLINE_1:
        if (input == '\n') {
            m_state = HEADER_LINE_START;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }

    /*
    ** Headers
    */

    case HEADER_LINE_START:
        if (input == '\r') {
            m_state = EXPECTING_NEWLINE_3;
            return INDETERMINATE;
        }
        else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
            m_state = HEADER_LWS;
            return INDETERMINATE;
        }
        else if (!isChar(input) || isControl(input) || isSpecial(input)) {
            return BAD;
        }
        else {
            m_headers.push_back(LexedHeader());
            m_headers.back().name.push_back(std::tolower(input));
            m_state = HEADER_NAME;
            return INDETERMINATE;
        }

    case HEADER_LWS:
        if (input == '\r') {
            m_state = EXPECTING_NEWLINE_2;
            return INDETERMINATE;
        }
        else if (input == ' ' || input == '\t') {
            return INDETERMINATE;
        }
        else if (isControl(input)) {
            return BAD;
        }
        else {
            m_state = HEADER_VALUE;
            m_headers.back().value.push_back(input);
            return INDETERMINATE;
        }

    case HEADER_NAME:
        if (input == ':') {
            m_state = SPACE_BEFORE_HEADER_VALUE;
            return INDETERMINATE;
        }
        else if (!isChar(input) || isControl(input) || isSpecial(input)) {
            return BAD;
        }
        else
        {
            m_headers.back().name.push_back(std::tolower(input));
            return INDETERMINATE;
        }

    case SPACE_BEFORE_HEADER_VALUE:
        if (input == ' ') {
            m_state = HEADER_VALUE;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }

    case HEADER_VALUE:
        if (input == '\r') {
            m_state = EXPECTING_NEWLINE_2;
            return INDETERMINATE;
        }
        else if (isControl(input)) {
            return BAD;
        }
        else {
            m_headers.back().value.push_back(input);
            return INDETERMINATE;
        }

    case EXPECTING_NEWLINE_2:
        if (input == '\n') {
            m_state = HEADER_LINE_START;
            return INDETERMINATE;
        }
        else {
            return BAD;
        }

    case EXPECTING_NEWLINE_3:
        if (input == '\n') {
            for (auto header : m_headers) {
                //::LogDebug(LOG_REST, "HTTPLexer::consume(), header = %s, value = %s", header.name.c_str(), header.value.c_str());
                req.headers.add(header.name, header.value);
            }

            return GOOD;
        } else {
            return BAD;
        }

    default:
        return BAD;
    }
}

/// <summary>
/// Check if a byte is an HTTP character.
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
bool HTTPLexer::isChar(int c)
{
    return c >= 0 && c <= 127;
}

/// <summary>
/// Check if a byte is an HTTP control character.
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
bool HTTPLexer::isControl(int c)
{
    return (c >= 0 && c <= 31) || (c == 127);
}

/// <summary>
/// Check if a byte is an HTTP special character.
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
bool HTTPLexer::isSpecial(int c)
{
    switch (c)
    {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
        return true;
    default:
        return false;
    }
}

/// <summary>
/// Check if a byte is an digit.
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
bool HTTPLexer::isDigit(int c)
{
    return c >= '0' && c <= '9';
}
