/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/http/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_HTTP_TESTS_HELPERS_HTTPREQUESTPUSHPARSERTESTCALLBACKS_HPP_
#define _ISHIKO_CPP_HTTP_TESTS_HELPERS_HTTPREQUESTPUSHPARSERTESTCALLBACKS_HPP_

#include "Ishiko/HTTP/HTTPRequestPushParser.hpp"
#include <string>
#include <utility>
#include <vector>

class HTTPRequestPushParserTestCallbacks : public Ishiko::HTTPRequestPushParser::Callbacks
{
public:
    void onMethod(boost::string_view data) override;
    void onRequestURI(boost::string_view data) override;
    void onHTTPVersion(boost::string_view data) override;
    void onHeader(boost::string_view name, boost::string_view value) override;
    void onBodyFragment(boost::string_view data) override;

    const std::string& method() const;
    const std::string& requestURI() const;
    const std::string& httpVersion() const;
    const std::vector<std::pair<std::string, std::string>>& headers() const;
    const std::string& body() const;

private:
    std::string m_method;
    std::string m_requestURI;
    std::string m_httpVersion;
    std::vector<std::pair<std::string, std::string>> m_headers;
    std::string m_body;
};

#endif
