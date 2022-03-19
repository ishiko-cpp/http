/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/http/blob/main/LICENSE.txt
*/

#include "HTTPHeader.hpp"

using namespace std;

namespace Ishiko
{

HTTPHeader::HTTPHeader(string name, string value)
    : m_name(move(name)), m_value(move(value))
{
}

const string& HTTPHeader::name() const
{
    return m_name;
}

const string& HTTPHeader::value() const
{
    return m_value;
}

}
