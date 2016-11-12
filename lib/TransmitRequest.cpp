#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include "../include/TransmitRequest.hpp"
#include "../include/Xbee.hpp"
#include "../include/Utility.hpp"
#include "../lib/Utility.cpp"

namespace XBEE {

  TransmitRequest::TransmitRequest() {
    frame_type = 0x10;
    target_mac_64 = static_cast<uint64_t> (BroadcastType_64::RESERVE_COORDINATOR);
    target_mac_16 = static_cast<uint16_t> (BroadcastType_16::DEFAULT);
    TransmitRequest::SetLength();
    TransmitRequest::SetChecksum();
  }

  TransmitRequest::TransmitRequest(uint64_t target_64) {
    frame_type = 0x10;
    target_mac_64 = target_64;
    target_mac_16 = static_cast<uint64_t> (BroadcastType_16::DEFAULT);
    TransmitRequest::SetLength();
    TransmitRequest::SetChecksum();
  }

  TransmitRequest::TransmitRequest(uint64_t target_64, uint16_t target_16) {
    frame_type = 0x10;
    target_mac_64 = target_64;
    target_mac_16 = target_16;
    TransmitRequest::SetLength();
    TransmitRequest::SetChecksum();
  }

  std::string TransmitRequest::ToHexString(Hex_string_format data_format) const {

    std::stringstream tmp;
    bool is_spaced;
    bool even_spacing = data_format != DATA_SPACE ? true : false;

    switch (even_spacing) {
      case true: // even spacisng
        is_spaced = data_format == BYTE_SPACE ? true : false;
        tmp << HexString(start, is_spaced, is_spaced);
        tmp << HexString(length, is_spaced, is_spaced);
        tmp << HexString(frame_type, is_spaced, is_spaced);
        tmp << HexString(frame_id, is_spaced, is_spaced);
        tmp << HexString(target_mac_64, is_spaced, is_spaced);
        tmp << HexString(target_mac_16, is_spaced, is_spaced);
        tmp << HexString(broadcast_radius, is_spaced, is_spaced);
        tmp << HexString(options, is_spaced, is_spaced);

        for (auto itr = data.begin(); itr != data.end(); ++itr) if (*itr != 0x00) tmp << HexString(*itr, is_spaced, is_spaced);
        tmp << HexString(checksum, is_spaced);
        break;

      case false: // not even spacing
        tmp << HexString(start, false, true);
        tmp << HexString(length, false, true);
        tmp << HexString(frame_type, false, true);
        tmp << HexString(frame_id, false, true);
        tmp << HexString(target_mac_64, false, true);
        tmp << HexString(target_mac_16, false, true);
        tmp << HexString(broadcast_radius, false, true);

        // TODO: Does the bug still exist?
        tmp << HexString(options, false, true);

        // TODO: Should there be a space between each data member?
        for (auto itr = data.begin(); itr != data.end(); ++itr) if (*itr != 0x00) tmp << HexString(*itr, false, false);
        tmp << ' ' << HexString(checksum, false);
        break;

      default:
        // TODO: Will this do what I think it does?
        static_assert(true, "Invalid data_format parameter to TransmitRequest HexString method");
        break;
    }
        return tmp.str();
  }

  void TransmitRequest::SetData(const std::string &message) {
    auto data_itr = data.begin();
    for (auto letter = message.begin(); letter != message.end(); ++data_itr, ++letter) {
      *data_itr = *letter;
    }
    TransmitRequest::SetLength();
    TransmitRequest::SetChecksum();
  }

  void TransmitRequest::SetLength() {
    length = sizeof (frame_type)
            + sizeof (frame_id)
            + sizeof (target_mac_64)
            + sizeof (target_mac_16)
            + sizeof (broadcast_radius)
            + sizeof (options);
    for (auto it = data.begin(); it != data.end(); ++it) {
      length += (*it != 0x00) ? sizeof (*it) : 0x00;
    }
  }

  void TransmitRequest::SetChecksum() {

    uint8_t byte_sum = ByteSum(frame_type);
    byte_sum += ByteSum(frame_id);
    byte_sum += ByteSum(target_mac_64);
    byte_sum += ByteSum(target_mac_16);
    byte_sum += broadcast_radius;
    byte_sum += options;

    for (auto itr = data.begin(); itr != data.end(); ++itr) {
      byte_sum += *itr;
    }

    checksum = kMaxChecksum - byte_sum;
  }

  // Add delimiter support
  std::istream& operator>>(std::istream &strm, TransmitRequest &tr) {
    std::string temp;
    std::getline(strm, temp);
    tr.SetData(temp);
    return strm;
  }
}