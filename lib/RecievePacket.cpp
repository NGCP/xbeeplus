#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include "../include/RecievePacket.hpp"
#include "../include/Xbee.hpp"

namespace XBEE {
	RecievePacket::RecievePacket() {
		frame_type = 0x90;
		source_mac_64 = static_cast<uint64_t>(BroadcastType_64::UNKNOWN);
		source_mac_16 = static_cast<uint16_t>(BroadcastType_16::DEFAULT);
		RecievePacket::SetLength();
		RecievePacket::SetChecksum();
	}

	RecievePacket::RecievePacket(uint64_t source_64) {
		frame_type = 0x90;
		source_mac_64 = source_64;
		source_mac_16 = static_cast<uint64_t>(BroadcastType_16::DEFAULT);
		RecievePacket::SetLength();
		RecievePacket::SetChecksum();
	}

	RecievePacket::RecievePacket(uint64_t source_64, uint16_t source_16) {
		frame_type = 0x90;
		source_mac_64 = source_64;
		source_mac_16 = source_16;
		RecievePacket::SetLength();
		RecievePacket::SetChecksum();
	}

	void RecievePacket::SetLength() {
		length = sizeof(frame_type)
							+ sizeof(frame_id)
							+ sizeof(source_mac_64)
							+ sizeof(source_mac_16)
							+ sizeof(options);
		for(auto it = data.begin(); it != data.end(); ++it) {
			length += (*it != 0x00) ? sizeof(*it) : 0x00;
		}
	}

	void RecievePacket::SetChecksum() {
		uint8_t byte_sum = frame_type + frame_id;
		byte_sum += ((0xFF00000000000000 & source_mac_64) >> 56);
		byte_sum += ((0x00FF000000000000 & source_mac_64) >> 48);
		byte_sum += ((0x0000FF0000000000 & source_mac_64) >> 40);
		byte_sum += ((0x000000FF00000000 & source_mac_64) >> 32);
		byte_sum += ((0x00000000FF000000 & source_mac_64) >> 24);
		byte_sum += ((0x0000000000FF0000 & source_mac_64) >> 16);
		byte_sum += ((0x000000000000FF00 & source_mac_64) >> 8);
		byte_sum += (0x00000000000000FF & source_mac_64);
		byte_sum += ((0xFF00 & source_mac_16) >> 8);
		byte_sum += (0x00FF & source_mac_16);
		byte_sum += options;
		for(auto itr = data.begin(); itr != data.end(); ++itr) {
			byte_sum += *itr;
		}
		checksum = kMaxChecksum - byte_sum;
	}

	
}