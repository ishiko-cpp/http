/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/http/blob/main/LICENSE.txt
*/

#include "HTTPMessagePushParser.hpp"

using namespace boost;

namespace Ishiko
{

void HTTPMessagePushParser::Callbacks::onMethod(string_view data)
{
}

void HTTPMessagePushParser::Callbacks::onRequestURI(boost::string_view data)
{
}

void HTTPMessagePushParser::Callbacks::onHTTPVersion(boost::string_view data)
{
}

void HTTPMessagePushParser::Callbacks::onHeader(boost::string_view data)
{
}

void HTTPMessagePushParser::Callbacks::onBody(boost::string_view data)
{
}

HTTPMessagePushParser::HTTPMessagePushParser(Callbacks& callbacks)
    : m_callbacks(callbacks)
{
}

void HTTPMessagePushParser::onData(string_view data)
{
    const char* previous = data.data();
    if (!m_fragmentedData.empty())
    {
        previous = m_fragmentedData.data();
    }
    const char* current = data.data();
    const char* end = current + data.length();
    while (current < end)
    {
        switch (m_parsingMode)
        {
        case ParsingMode::method:
            while (current < end)
            {
                if (*current == ' ')
                {
                    if (m_fragmentedData.empty())
                    {
                        m_callbacks.onMethod(string_view(previous, (current - previous)));
                    }
                    else
                    {
                        m_fragmentedData.append(data.data(), current - data.data());
                        m_callbacks.onMethod(m_fragmentedData);
                        m_fragmentedData.clear();
                    }
                    ++current;
                    m_parsingMode = ParsingMode::requestURI;
                    break;
                }
                ++current;
            }
            break;

        case ParsingMode::requestURI:
            while (current < end)
            {
                if (*current == ' ')
                {
                    if (m_fragmentedData.empty())
                    {
                        m_callbacks.onRequestURI(string_view(previous, (current - previous)));
                    }
                    else
                    {
                        m_fragmentedData.append(data.data(), current - data.data());
                        m_callbacks.onRequestURI(m_fragmentedData);
                        m_fragmentedData.clear();
                    }
                    ++current;
                    m_parsingMode = ParsingMode::httpVersion;
                    break;
                }
                ++current;
            }
            break;

        case ParsingMode::httpVersion:
            while (current < end)
            {
                if (*current == '\r')
                {
                    if (m_fragmentedData.empty())
                    {
                        m_callbacks.onHTTPVersion(string_view(previous, (current - previous)));
                    }
                    else
                    {
                        m_fragmentedData.append(data.data(), current - data.data());
                        m_callbacks.onHTTPVersion(m_fragmentedData);
                        m_fragmentedData.clear();
                    }
                }
                else if (*current == '\n')
                {
                    m_parsingMode = ParsingMode::headers;
                    ++current;
                    break;
                }
                ++current;
            }
            break;

        case ParsingMode::headers:
            if (*current == '\r')
            {
                m_parsingMode = ParsingMode::body;
            }
            else if (*current != '\r')
            {
                m_parsingMode = ParsingMode::headerName;
                // TODO
                //m_headerName = current;
            }
            break;

        case ParsingMode::headerName:
            while (current < end)
            {
                if (*current == ':')
                {
                    ++current;
                    m_parsingMode = ParsingMode::headerValue;
                    // TODO
                    //m_headerValue = current;
                    break;
                }
                ++current;
            }
            break;

        case ParsingMode::headerValue:
            while (current < end)
            {
                if (*current == '\r')
                {
                    notifyHeader();
                    // TODO
                    //*current = 0;
                }
                else if (*current == '\n')
                {
                    m_parsingMode = ParsingMode::headers;
                    ++current;
                    break;
                }
                ++current;
            }
            break;

        case ParsingMode::body:
            while (current < end)
            {
                ++current;
            }
            break;
        }
    }
}

void HTTPMessagePushParser::notifyHeader()
{
    /*
    if (strncmp(m_headerName, "Accept", 7) == 0)
    {
        m_accept = m_headerValue;
        while ((*m_accept == ' ') || (*m_accept == '\t'))
        {
            ++m_accept;
        }
    }
    else if (strncmp(m_parsingState.m_headerName, "Accept-Charset", 15) == 0)
    {
        m_acceptCharset = m_headerValue;
        while ((*m_acceptCharset == ' ') || (*m_acceptCharset == '\t'))
        {
            ++m_acceptCharset;
        }
    }
    else if (strncmp(m_parsingState.m_headerName, "Accept-Encoding", 16) == 0)
    {
        m_acceptEncoding = m_headerValue;
        while ((*m_acceptEncoding == ' ') || (*m_acceptEncoding == '\t'))
        {
            ++m_acceptEncoding;
        }
    }
    else if (strncmp(m_parsingState.m_headerName, "Accept-Language", 16) == 0)
    {
        m_acceptLanguage = m_headerValue;
        while ((*m_acceptLanguage == ' ') || (*m_acceptLanguage == '\t'))
        {
            ++m_acceptLanguage;
        }
    }
    else if (strncmp(m_parsingState.m_headerName, "Content-Length", 15) == 0)
    {
        m_contentLength = m_headerValue;
        while ((*m_contentLength == ' ') || (*m_contentLength == '\t'))
        {
            ++m_contentLength;
        }
    }
    else if (strncmp(m_parsingState.m_headerName, "Host", 5) == 0)
    {
        m_host = m_headerValue;
        while ((*m_host == ' ') || (*m_host == '\t'))
        {
            ++m_host;
        }
    }
    else if (strncmp(m_parsingState.m_headerName, "Transfer-Encoding", 18) == 0)
    {
        m_transferEncoding = m_headerValue;
        while ((*m_transferEncoding == ' ') || (*m_transferEncoding == '\t'))
        {
            ++m_transferEncoding;
        }
    }*/
}

}