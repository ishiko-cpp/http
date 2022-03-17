/*
    Copyright (c) 2022 Xavier Leclercq
    All rights reserved.
*/

#include "HTTPResponseTests.hpp"
#include "Ishiko/HTTP/HTTPResponse.hpp"
#include <Ishiko/FileSystem.hpp>

using namespace boost::filesystem;
using namespace Ishiko;
using namespace Ishiko::Tests;
using namespace std;

HTTPResponseTests::HTTPResponseTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "HTTPResponse tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<FileComparisonTest>("toString test 1", ToStringTest1);
}

void HTTPResponseTests::ConstructorTest1(Test& test)
{
    HTTPResponse response;

    ISHIKO_PASS();
}

void HTTPResponseTests::ToStringTest1(FileComparisonTest& test)
{
    path outputPath(test.context().getTestOutputPath("HTTPResponseTests_ToStringTest1.bin"));

    HTTPResponse response;
    string responseString = response.toString();

    Error error;
    // TODO: should just throw an exception
    BinaryFile file = BinaryFile::Create(outputPath, error);

    ISHIKO_ABORT_IF(error);

    file.write(responseString.c_str(), responseString.length());

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataPath("HTTPResponseTests_ToStringTest1.bin"));

    ISHIKO_PASS();
}
