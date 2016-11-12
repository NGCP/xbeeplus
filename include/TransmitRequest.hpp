#pragma once

#ifndef TRANSMIT_REQUEST_HPP
#define TRANSMIT_REQUEST_HPP

#include "Frame.hpp"

#include <array>
#include <string>

namespace XBEE {
  
  // enum specifying the possible data formats for calls to ToHexString
  enum Hex_string_format {
    NO_SPACE,
    DATA_SPACE,
    BYTE_SPACE,
    
    NUM_FORMATS
  };
  // Allow API mode 2 (Escape characters 0x7E, 0x7D, 0x13, 0x11)

  class TransmitRequest : public Frame {
  public:
    TransmitRequest();
    TransmitRequest(uint64_t target_64);
    TransmitRequest(uint64_t target_64, uint16_t target_16);

    // Currently this rewrites the entire data buffer with string value
    // Fix this so it appends the data buffer
    void SetData(const std::string &message);

  protected:
    // Used to enable xbee firmware check message on successful send (for values above 0) -- Not implemented
    uint8_t frame_id = 0x00;
    uint64_t target_mac_64;
    uint16_t target_mac_16;
    // Used to set how many broadcast hops are allowed -- Not implemented
    uint8_t broadcast_radius = 0x00;
    // Used to set extra options like AES encryption -- Not implemented
    uint8_t options = 0x00;
    // Possible bug: Cannot send/transmit the null character
    std::array<uint8_t, 65521> data{};

  private:
    void SetLength();
    void SetChecksum_old();
    void SetChecksum();
    std::string ToHexString(Hex_string_format equal_space = NO_SPACE) const;

    friend std::ostream& operator<<(std::ostream &strm, const TransmitRequest &tr) {
      return strm << tr.ToHexString();
    }
    friend std::istream& operator>>(std::istream &strm, TransmitRequest &tr);
    // Operator overloads for +, should conacatenate the data variable with other operand data type
  };
}
#endif