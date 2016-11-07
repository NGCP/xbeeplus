#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include "../include/TransmitRequest.hpp"
#include "../include/Xbee.hpp"

namespace XBEE {
	TransmitRequest::TransmitRequest() {
		frame_type = 0x10;
		target_mac_64 = static_cast<uint64_t>(BroadcastType_64::RESERVE_COORDINATOR);
		target_mac_16 = static_cast<uint16_t>(BroadcastType_16::DEFAULT);
		TransmitRequest::SetLength();
		TransmitRequest::SetChecksum();
	}

	TransmitRequest::TransmitRequest(uint64_t target_64) {
		frame_type = 0x10;
		target_mac_64 = target_64;
		target_mac_16 = static_cast<uint64_t>(BroadcastType_16::DEFAULT);
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

	std::string TransmitRequest::HexString(bool equal_space) const {
		std::stringstream temp;
		// Equal Spacing is not yet implemented
		temp << std::hex << std::uppercase << std::setfill('0');
		temp << std::setw(2) << static_cast<int>(start);
		temp << " " << std::setw(4) << static_cast<int>(length);
		temp << " " << std::setw(2) << static_cast<int>(frame_type);
		temp << " " << std::setw(2) << static_cast<int>(frame_id);
		temp << " " << std::setw(16) << target_mac_64;
		temp << " " << std::setw(4) << target_mac_16;
		temp << " " << std::setw(2) << static_cast<int>(broadcast_radius);
		temp << " " << std::setw(2) << static_cast<int>(options);
		// Bug: Still prints out leading 00 hex characters
		for(auto itr = data.begin(); itr != data.end(); ++itr) {
			if(*itr != 0x00)
				temp << std::setw(2) << static_cast<int>(*itr);
		}
		temp << " " << std::setw(2) << static_cast<int>(checksum);
		return temp.str();
	}

	void TransmitRequest::SetData(const std::string &message) {
		auto data_itr = data.begin();
		for(auto letter = message.begin(); letter != message.end(); ++data_itr, ++letter) {
			*data_itr = *letter;
		}
		TransmitRequest::SetLength();
		TransmitRequest::SetChecksum();
	}

	void TransmitRequest::SetLength() {
		length = sizeof(frame_type)
							+ sizeof(frame_id)
							+ sizeof(target_mac_64)
							+ sizeof(target_mac_16)
							+ sizeof(broadcast_radius)
							+ sizeof(options);
		for(auto it = data.begin(); it != data.end(); ++it) {
			length += (*it != 0x00) ? sizeof(*it) : 0x00;
		}
	}

	void TransmitRequest::SetChecksum() {
		uint8_t byte_sum = frame_type + frame_id;
		byte_sum += ((0xFF00000000000000 & target_mac_64) >> 56);
		byte_sum += ((0x00FF000000000000 & target_mac_64) >> 48);
		byte_sum += ((0x0000FF0000000000 & target_mac_64) >> 40);
		byte_sum += ((0x000000FF00000000 & target_mac_64) >> 32);
		byte_sum += ((0x00000000FF000000 & target_mac_64) >> 24);
		byte_sum += ((0x0000000000FF0000 & target_mac_64) >> 16);
		byte_sum += ((0x000000000000FF00 & target_mac_64) >> 8);
		byte_sum += (0x00000000000000FF & target_mac_64);
		byte_sum += ((0xFF00 & target_mac_16) >> 8);
		byte_sum += (0x00FF & target_mac_16);
		byte_sum += broadcast_radius;
		byte_sum += options;
		for(auto itr = data.begin(); itr != data.end(); ++itr) {
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