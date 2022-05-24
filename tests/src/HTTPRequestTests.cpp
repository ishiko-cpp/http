/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/http/blob/main/LICENSE.txt
*/

#include "HTTPRequestTests.hpp"
#include "Ishiko/HTTP/HTTPRequest.hpp"
#include <Ishiko/FileSystem.hpp>
#include <string>

using namespace Ishiko;

HTTPRequestTests::HTTPRequestTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "HTTPRequest tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<FileComparisonTest>("toString test 1", ToStringTest1);
    append<FileComparisonTest>("toString test 2", ToStringTest2);
}

void HTTPRequestTests::ConstructorTest1(Test& test)
{
    HTTPRequest request(HTTPMethod::get, URL("http://example.org"));

    ISHIKO_TEST_FAIL_IF_NEQ(request.method(), HTTPMethod::get);
    ISHIKO_TEST_FAIL_IF_NEQ(request.requestURI(), "http://example.org");
    ISHIKO_TEST_PASS();
}

void HTTPRequestTests::ToStringTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.context().getTestOutputPath("HTTPRequestTests_ToStringTest1.bin"));

    HTTPRequest request(HTTPMethod::get, URL("http://example.org"));
    std::string requestString = request.toString();

    Error error;
    // TODO: should just throw an exception
    BinaryFile file = BinaryFile::Create(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    file.write(requestString.c_str(), requestString.length());

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataPath("HTTPRequestTests_ToStringTest1.bin"));

    ISHIKO_TEST_PASS();
}

void HTTPRequestTests::ToStringTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.context().getTestOutputPath("HTTPRequestTests_ToStringTest2.bin"));

    HTTPRequest request(HTTPMethod::get, URL("http://example.org"));
    request.setConnectionHeader(HTTPHeader::ConnectionMode::keepAlive);
    std::string requestString = request.toString();

    Error error;
    // TODO: should just throw an exception
    BinaryFile file = BinaryFile::Create(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    file.write(requestString.c_str(), requestString.length());

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataPath("HTTPRequestTests_ToStringTest2.bin"));

    ISHIKO_TEST_PASS();
}
