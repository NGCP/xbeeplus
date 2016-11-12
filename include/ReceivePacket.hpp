#pragma once

#ifndef RECIEVE_PACKET_HPP
#define RECIEVE_PACKET_HPP

#include "Frame.hpp"

#include <string>
#include <array>

namespace XBEE {
  // Allow API mode 2 with escape characters

  class ReceivePacket : public Frame {
  public:
    ReceivePacket();
    ReceivePacket(uint64_t source_64);
    ReceivePacket(uint64_t source_64, uint16_t source_16);

    std::string GetData();

  protected:
    // Used to enable xbee firmware check message on successful send (for values above 0) -- Not implemented
    uint8_t frame_id = 0x00;
    uint64_t source_mac_64;
    uint16_t source_mac_16;
    // Used to set packet options (not yet implemented)
    uint8_t options = 0x00;
    // Possible bug: Cannot receive the null character
    std::array<uint8_t, 65521> data{};

  private:
    void SetLength();
    void SetChecksum();
    void SetChecksum_old();
    void SetData();
    std::string HexString(bool equal_space = false) const;
  };
}
#endif