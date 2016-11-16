#include <iostream>
#include <thread>
#include <cassert>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <string>

#include "../include/Utility.hpp"
#include "../lib/Utility.cpp"
#include "../include/SerialXbee.hpp"
#include "../include/Xbee.hpp"

#include "../include/TransmitRequest.hpp"

using namespace XBEE;
using namespace std;

int TestHexString() {
    // Test cases for HexString function

    string hexString;
    string expected;

    // uint8_t
    uint8_t val1 = 123;
    expected = "7B";
    hexString = HexString(val1, true, false, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val1 << ", Hex Representation: " << hexString << std::endl;

    hexString = HexString(val1, false, false, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val1 << ", No Space Hex Representation: " << hexString << std::endl;

    hexString = HexString(val1);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val1 << ", Big Endian Hex Representation: " << hexString << std::endl;
    hexString = HexString(val1);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val1 << ", Big Endian No Space Hex Representation: " << hexString << std::endl;


    // uint16_t
    uint16_t val2 = 12345;
    expected = "39 30";
    hexString = HexString(val2, true, false, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val2 << ", Hex Representation: " << hexString << std::endl;

    hexString = HexString(val2, false, false, false);
    expected = "3930";
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val2 << ", No Space Hex Representation: " << hexString << std::endl;

    expected = "30 39";
    hexString = HexString(val2);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val2 << ", Big Endian Hex Representation: " << hexString << std::endl;

    hexString = HexString(val2, false);
    expected = "3039";
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val2 << ", Big Endian No Space Hex Representation: " << hexString << std::endl;

    // uint32_t
    uint32_t val3 = 1234567890;
    expected = "D2 02 96 49";
    hexString = HexString(val3, true, false, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val3 << ", Hex Representation: " << hexString << std::endl;

    hexString = HexString(val3, false, false, false);
    expected = "D2029649";
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val3 << ", No Space Hex Representation: " << hexString << std::endl;

    expected = "49 96 02 D2";
    hexString = HexString(val3);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val3 << ", Big Endian Hex Representation: " << hexString << std::endl;

    hexString = HexString(val3, false);
    expected = "499602D2";
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val3 << ", Big Endian No Space Hex Representation: " << hexString << std::endl;

    // uint64_t
    uint64_t val4 = 1234567890123456789;
    expected = "15 81 E9 7D F4 10 22 11";
    hexString = HexString(val4, true, false, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val4 << ", Hex Representation: " << hexString << std::endl;

    hexString = HexString(val4, false, false, false);
    expected = "1581E97DF4102211";
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val4 << ", No Space Hex Representation: " << hexString << std::endl;

    expected = "11 22 10 F4 7D E9 81 15";
    hexString = HexString(val4);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << val4 << ", Big Endian Hex Representation: " << hexString << std::endl;

    hexString = HexString(val4, false);
    expected = "112210F47DE98115";
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "Test Val: " << val4 << ", Big Endian No Space Hex Representation: " << hexString << std::endl;

    // string
    expected = "51 75 61 64 20 41";
    hexString = HexString("Quad A");
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << "Quad A" << ", Hex Representation: " << hexString << std::endl;
    
    expected = "517561642041";
    hexString = HexString("Quad A", false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << "Quad A" << ", No Space Hex Representation: " << hexString << std::endl;
    
    // char array
    char charStr[] = "Quad A";
    expected = "51 75 61 64 20 41";
    hexString = HexString(charStr);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << charStr << ", Hex Representation: " << hexString << std::endl;

    expected = "517561642041";
    hexString = HexString(charStr, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << charStr << ", No Space Hex Representation: " << hexString << std::endl;

    // std::string
    std::string str1 = "Quad ABC";
    expected = "51 75 61 64 20 41 42 43";
    hexString = HexString(str1);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << str1 << ", Hex Representation: " << hexString << std::endl;

    expected = "5175616420414243";
    hexString = HexString(str1, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << str1 << ", No Space Hex Representation: " << hexString << std::endl;

    std::string str2 = "Q0,P35.1234567 -120.789101112 103.789,R0";
    expected = "51 30 2C 50 33 35 2E 31 32 33 34 35 36 37 20 2D 31 32 30 2E 37 38 39 31 30 31 31 31 32 20 31 30 33 2E 37 38 39 2C 52 30";
    hexString = HexString(str2);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << str2 << ", Big Endian Hex Representation: " << hexString << std::endl;

    expected = "51302C5033352E31323334353637202D3132302E373839313031313132203130332E3738392C5230";
    hexString = HexString(str2, false);
    assert(!strcmp(hexString.c_str(), expected.c_str()));
//    std::cout << "\nTest Val: " << str2 << ", Big Endian No Space Hex Representation: " << hexString << std::endl;
//    std::cout << "All tests for HexString function passed\n" << std::endl;
    return 0;
}

int TestByteSum() {
    // Test cases for ByteSum function
    uint8_t expected;
    int sum;
    
    // string
    expected = 236;
    sum = ByteSum("Quad A");
    assert(expected == sum);
//    std::cout << "Test Val: " << "Quad A" << ", Byte Sum: " << sum << std::endl;
    
    expected = 19;
    sum = ByteSum("Q0,P35.1234567 -120.789101112 103.789,R0");
    assert(expected == sum);
//    std::cout << "Test Val: " << "Q0,P35.1234567 -120.789101112 103.789,R0"<< ", Byte Sum: " << sum << std::endl;

    // char array
    char charArr[] = "Quad A";
    expected = 236;
    sum = 0;
    sum = ByteSum(charArr);
    assert(expected == sum);
//    std::cout << "Test Val: " << charArr << ", Byte Sum: " << sum << std::endl;

    char charArr2[] = "Q0,P35.1234567 -120.789101112 103.789,R0";
    expected = 19;
    sum = ByteSum(charArr2);
    assert(expected == sum);
//    std::cout << "Test Val: " << charArr2 << ", Byte Sum: " << sum << std::endl;

    // std::string
    std::string str = charArr;
    expected = 236;
    sum = ByteSum(str);
    assert(expected == sum);
//    std::cout << "Test Val: " << str << ", Byte Sum: " << sum << std::endl;

    expected = 236;
    sum = ByteSum(str.c_str());
    assert(expected == sum);
//    std::cout << "Test Val: " << str.c_str() << ", Byte Sum: " << sum << std::endl;

    std::string str2 = charArr2;
    expected = 19;
    sum = ByteSum(str2);
    assert(expected == sum);
//    std::cout << "Test Val: " << str2 << ", Byte Sum: " << sum << std::endl;

    // uint8_t
    uint8_t uint8 = 123;
    expected = 123;
    sum = ByteSum(uint8);
    assert(expected == sum);
//    std::cout << "Test Val: " << uint8 << ", Byte Sum: " << sum << std::endl;

    // uint16_t
    uint16_t uint16 = 12345;
    expected = 105;
    sum = ByteSum(uint16);
    assert(expected == sum);
//    std::cout << "Test Val: " << uint16 << ", Byte Sum: " << sum << std::endl;

    // uint32_t
    uint32_t uint32 = 1234567890;
    expected = 179;
    sum = ByteSum(uint32);
    assert(expected == sum);
//    std::cout << "Test Val: " << uint32 << ", Byte Sum: " << sum << std::endl;

    // uint64_t
    uint64_t uint64 = 1234567890123456789;
    expected = 51;
    sum = ByteSum(uint64);
    assert(expected == sum);
//    std::cout << "Test Val: " << uint64 << ", Byte Sum: " << sum << std::endl;
//    std::cout << "All tests for ByteSum function passed" << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (TestHexString()) cout << "HexString test failed" << std::endl;
    if (TestByteSum()) cout << "ByteSum test failed" << std::endl;

    // Warning API level 1 (no escapes)
    XBEE::SerialXbee test;
    test.AsyncReadFrame();
    int i = 0;

    while(true) {
    std::cout << std::dec << i++ << " seconds have passed" << std::endl;
    sleep(1);
}
}
