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

  std::string TransmitRequest::ToHexString(HexFormat spacing) const {

    std::stringstream tmp;
    /*
      Example of good minimal switch case implementation:
      This is called case fall-through, which is desirable for minimal code
      Rule of thumb, if your using a switch-case, you shouldn't have if statements
      bool even_space = false;
      bool data_space = false;
      switch (spacing) {
        case HexFormat::BYTE_SPACING:
          even_space = true;
        case HexFormat::DATA_SPACING:
          data_space = true;
        case HexFormat::NO_SPACING:
          tmp << HexString(start, even_space, data_space);
          tmp << HexString(length, even_space, data_space);
          tmp << HexString(frame_type, even_space, data_space);
          tmp << HexString(frame_id, even_space, data_space);
          tmp << HexString(target_mac_64, even_space, data_space);
          tmp << HexString(target_mac_16, even_space, data_space);
          tmp << HexString(broadcast_radius, even_space, data_space);
          tmp << HexString(options, even_space, data_space);
          for (auto itr = data.begin(); itr != data.end(); ++itr)
            if (*itr != 0x00)
              tmp << HexString(*itr, even_space, data_space);
          tmp << HexString(checksum, even_space, data_space);
      }
      TODO: Bonus points, for implementing the above while also getting rid of your HexString's third argument
            The third argument is extraneous and unnecessary. Above design can be implemented without it as well.
            This will remove code complexity from your HexString function as well.
            Less Code == Better in C++
    */

    // TODO: Re-factor this messy stuff (Looking at you Frank)
    switch (spacing) {
      case HexFormat::NO_SPACING:
        tmp << HexString(start, false, false);
        tmp << HexString(length, false, false);
        tmp << HexString(frame_type, false, false);
        tmp << HexString(frame_id, false, false);
        tmp << HexString(target_mac_64, false, false);
        tmp << HexString(target_mac_16, false, false);
        tmp << HexString(broadcast_radius, false, false);
        tmp << HexString(options, false, false);
        for (auto itr = data.begin(); itr != data.end(); ++itr)
          if (*itr != 0x00)
            tmp << HexString(*itr, false, false);
        tmp << HexString(checksum, false, false);
        break;
      case HexFormat::DATA_SPACING:
        tmp << HexString(start, false, true);
        tmp << HexString(length, false, true);
        tmp << HexString(frame_type, false, true);
        tmp << HexString(frame_id, false, true);
        tmp << HexString(target_mac_64, false, true);
        tmp << HexString(target_mac_16, false, true);
        tmp << HexString(broadcast_radius, false, true);
        tmp << HexString(options, false, true);
        for (auto itr = data.begin(); itr != data.end(); ++itr)
          if (*itr != 0x00)
            tmp << HexString(*itr, false, true);
        tmp << HexString(checksum, false, true);
        break;
      case HexFormat::BYTE_SPACING:
        tmp << HexString(start, true, true);
        tmp << HexString(length, true, true);
        tmp << HexString(frame_type, true, true);
        tmp << HexString(frame_id, true, true);
        tmp << HexString(target_mac_64, true, true);
        tmp << HexString(target_mac_16, true, true);
        tmp << HexString(broadcast_radius, true, true);
        tmp << HexString(options, true, true);
        for (auto itr = data.begin(); itr != data.end(); ++itr)
          if (*itr != 0x00)
            tmp << HexString(*itr, true, true);
        tmp << HexString(checksum, true, true);
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