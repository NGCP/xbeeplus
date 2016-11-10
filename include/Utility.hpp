/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Utility.hpp
 * Author: zhangh94
 *
 * Created on November 9, 2016, 4:54 PM
 */

#ifndef UTILITY_HPP
#define UTILITY_HPP
namespace XBEE {
    // HexString functions and overloads
    std::string HexString(char value[], bool is_spaced = true);
    std::string HexString(const char value[], bool is_spaced = true);
    std::string HexString(const std::string &value, bool is_spaced = true);
    template <typename T> static std::string HexString(const T &value, bool is_spaced = true, bool little_endian = true);
    
    // ByteSum functions and overloads
    uint8_t ByteSum(char *item);
    uint8_t ByteSum(const char *item);
    uint8_t ByteSum(const std::string& item);
    template<typename T> uint8_t ByteSum(T &item);
}
#endif /* UTILITY_HPP */

