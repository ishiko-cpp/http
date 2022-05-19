/*
    Copyright (c) 2022 Xavier Leclercq
    All rights reserved.
*/

#include "HTTPMessagePushParserTests.hpp"
#include "helpers/HTTPMessagePushParserTestCallbacks.hpp"
#include "Ishiko/HTTP/HTTPMessagePushParser.hpp"
#include <boost/filesystem.hpp>
#include <Ishiko/FileSystem.hpp>
#include <string>

using namespace boost;
using namespace boost::filesystem;
using namespace Ishiko;
using namespace Ishiko::Tests;
using namespace std;

HTTPMessagePushParserTests::HTTPMessagePushParserTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "HTTPMessagePushParser tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("onData request test 1", OnDataRequestTest1);
    append<HeapAllocationErrorsTest>("onData request test 2", OnDataRequestTest2);
    append<HeapAllocationErrorsTest>("onData request test 3", OnDataRequestTest3);
    append<HeapAllocationErrorsTest>("onData request test 4", OnDataRequestTest4);
    append<HeapAllocationErrorsTest>("onData request test 5", OnDataRequestTest5);
    append<HeapAllocationErrorsTest>("onData request test 6", OnDataRequestTest6);
    append<HeapAllocationErrorsTest>("onData response test 1", OnDataResponseTest1);
    append<HeapAllocationErrorsTest>("onData response test 2", OnDataResponseTest2);
    append<HeapAllocationErrorsTest>("onData response test 3", OnDataResponseTest3);
}

void HTTPMessagePushParserTests::ConstructorTest1(Test& test)
{
    HTTPMessagePushParser::Callbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
     
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataRequestTest1(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataRequestTest1.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    bool complete = parser.onData(message);

    ISHIKO_TEST_FAIL_IF_NOT(complete);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::request);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.method(), "GET");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.requestURI(), "/");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers().size(), 0);
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataRequestTest2(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataRequestTest2.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    bool complete = parser.onData(message);

    ISHIKO_TEST_FAIL_IF_NOT(complete);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::request);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.method(), "GET");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.requestURI(), "/");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_ABORT_IF_NEQ(callbacks.headers().size(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].first, "Host");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].second, "example.org");
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataRequestTest3(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataRequestTest3.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    bool complete = parser.onData(message);

    ISHIKO_TEST_FAIL_IF_NOT(complete);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::request);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.method(), "GET");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.requestURI(), "/");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_ABORT_IF_NEQ(callbacks.headers().size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].first, "Host");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].second, "example.org");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[1].first, "Connection");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[1].second, "keep-alive");
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataRequestTest4(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataRequestTest1.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);

    // Feed the message byte per byte to test correct behaviour when the message is fragmented across multiple packets
    for (size_t i = 0; i < message.size(); ++i)
    {
        parser.onData(string_view(&message[i], 1));
    }

    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::request);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.method(), "GET");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.requestURI(), "/");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers().size(), 0);
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataRequestTest5(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataRequestTest2.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);

    // Feed the message byte per byte to test correct behaviour when the message is fragmented across multiple packets
    for (size_t i = 0; i < message.size(); ++i)
    {
        parser.onData(string_view(&message[i], 1));
    }

    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::request);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.method(), "GET");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.requestURI(), "/");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_ABORT_IF_NEQ(callbacks.headers().size(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].first, "Host");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].second, "example.org");
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataRequestTest6(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataRequestTest3.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    
    // Feed the message byte per byte to test correct behaviour when the message is fragmented across multiple packets
    for (size_t i = 0; i < message.size(); ++i)
    {
        parser.onData(string_view(&message[i], 1));
    }

    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::request);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.method(), "GET");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.requestURI(), "/");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_ABORT_IF_NEQ(callbacks.headers().size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].first, "Host");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].second, "example.org");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[1].first, "Connection");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[1].second, "keep-alive");
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataResponseTest1(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataResponseTest1.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    bool complete = parser.onData(message);

    ISHIKO_TEST_FAIL_IF_NOT(complete);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::response);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.statusCode(), "200");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.reasonPhrase(), "OK");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers().size(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.body(), "");
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataResponseTest2(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataResponseTest2.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    bool complete = parser.onData(message);

    ISHIKO_TEST_FAIL_IF_NOT(complete);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::response);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.statusCode(), "200");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.reasonPhrase(), "OK");
    ISHIKO_TEST_ABORT_IF_NEQ(callbacks.headers().size(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].first, "Content-Length");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].second, "0");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.body(), "");
    ISHIKO_TEST_PASS();
}

void HTTPMessagePushParserTests::OnDataResponseTest3(Test& test)
{
    path inputPath(test.context().getTestDataPath("HTTPMessagePushParserTests_OnDataResponseTest3.bin"));

    Error error;
    string message = FileSystem::ReadFile(inputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    HTTPMessagePushParserTestCallbacks callbacks;
    HTTPMessagePushParser parser(callbacks);
    bool complete = parser.onData(message);

    ISHIKO_TEST_FAIL_IF_NOT(complete);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.messageType(), HTTPMessagePushParserTestCallbacks::MessageType::response);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.httpVersion(), "HTTP/1.1");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.statusCode(), "200");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.reasonPhrase(), "OK");
    ISHIKO_TEST_ABORT_IF_NEQ(callbacks.headers().size(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].first, "Content-Length");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.headers()[0].second, "4");
    ISHIKO_TEST_FAIL_IF_NEQ(callbacks.body(), "body");
    ISHIKO_TEST_PASS();
}
