#include <iostream>
#include <memory>

#include "../include/SerialXbee.hpp"
#include "../include/ReceivePacket.hpp"
#include "../include/TransmitRequest.hpp"
#include "../include/Utility.hpp"
#include "../lib/Utility.cpp"

namespace XBEE {
	// TODO: Figure out why the below won't work for some dumb reason
	SerialXbee::SerialXbee() : m_io(), m_port(m_io) {
		Connect(kDefaultPath);
		ReadHandler = std::bind(&SerialXbee::PrintFrame, this, std::placeholders::_1);
	}

	void SerialXbee::Connect(std::string device_path, uint32_t baud_rate) {
		m_port.open(device_path);
		m_port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
		runner = boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));
	}

	// TODO: Clean up and optimize this messy code when time permits
	// TODO: [CRITICAL BUG] 1st packet does not get successfully parsed!!!
	// TODO: Implement checksum checking
	// TODO: Fix std::cout buffer flushing
	void SerialXbee::ParseFrame(const boost::system::error_code &error, size_t num_bytes) {
		// std::cout << "FOUND A FRAME TO PARSE!" << std::endl;
		using namespace boost::asio;

		char holder[8];

		uint16_t frame_length;
		uint8_t frame_type;
		Frame *cur_frame = NULL;

		if (error) {
			std::cout << "[ERROR] FOUND" << std::endl;
			// throw an error, by repeating system error code
		}
		if (num_bytes != 2) {
			std::cout << "[ERROR] NOT ENOUGH BYTES" << std::endl;
			// throw an error, saying that there was other things read besides the frame delimiter
		}
		// Clears buffer (Should only contain 0x7E delimiter, which is added automatically when new Frame object is created)
		buffer.consume(num_bytes);
		// Synchronously read the next 2 bytes of Frame (Frame Length)
		read(m_port, buffer, transfer_exactly(2));
		std::istream temp(&buffer);
		temp.get(holder[0]);
		temp.get(holder[1]);
		frame_length = (holder[0] << 8) + holder[1];
		read(m_port, buffer, transfer_exactly(1));
		temp.get(holder[0]);
		frame_type = holder[0];
		// std::cout << std::hex << std::uppercase << std::setfill('0') <<  std::setw(4) << static_cast<int>(frame_length) << std::endl;
		// std::cout << std::hex << std::uppercase << std::setfill('0') <<  std::setw(2) << static_cast<int>(frame_type) << std::endl;

		// Construct Frame object
		switch(FrameType(frame_type)) {
			case FrameType::RECEIVE_PACKET:
			{
				ReceivePacket frame;
				// Read rest of frame data
				read(m_port, buffer, transfer_exactly(frame_length - 1));
				// Mac 64
				for (int i = 0; i < 8; i++) {
					temp.get(holder[i]);
					//std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << static_cast<uint64_t>(holder[i]) << std::endl;
				}
				frame.source_mac_64 = (static_cast<uint64_t>(holder[0] & 0xFF) << 56);
				frame.source_mac_64 += (static_cast<uint64_t>(holder[1] & 0xFF) << 48);
				frame.source_mac_64 += (static_cast<uint64_t>(holder[2] & 0xFF) << 40);
				frame.source_mac_64 += (static_cast<uint64_t>(holder[3] & 0xFF) << 32);
				frame.source_mac_64 += (static_cast<uint64_t>(holder[4] & 0xFF) << 24);
				frame.source_mac_64 += (static_cast<uint64_t>(holder[5] & 0xFF) << 16);
				frame.source_mac_64 += (static_cast<uint64_t>(holder[6] & 0xFF) << 8);
				frame.source_mac_64 += static_cast<uint64_t>(holder[7]) & 0xFF;
				//std::cout << std::hex << std::uppercase << std::setfill('0') <<  std::setw(8) << frame.source_mac_64 << std::endl;
				// Mac 16
				temp.get(holder[0]);
				temp.get(holder[1]);
				frame.source_mac_16 = (static_cast<uint64_t>(holder[0] & 0xFF) << 8);
				frame.source_mac_16 += static_cast<uint64_t>(holder[1]) & 0xFF;
				//std::cout << std::hex << std::uppercase << std::setfill('0') <<  std::setw(4) << frame.source_mac_16 << std::endl;
				// Recieve Options
				temp.get(holder[0]);
				frame.options = holder[0];
				// Store into RecievePacket's data field
				uint8_t piece;
				for (int i = 0; i < frame_length - 12; i++) {
					temp.get(holder[0]);
					piece = holder[0];
					frame.data.at(i) = piece;
				}
				frame.length = frame_length;
				temp.get(holder[0]);
				piece = holder[0];
				std::cout << "Checksum is: " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(piece) << std::endl;
				frame.checksum = piece;
				cur_frame = &frame;
				break;
			}
			default:
				// Throw unable to parse frame error
				break;
		}
		// TODO: Add support for user callback function with Frame pointer type
		ReadHandler(cur_frame);
		AsyncReadFrame();
	}

	void SerialXbee::PrintFrame(Frame *a_frame) {
		std::cout << a_frame->ToHexString(HexFormat::BYTE_SPACING) << std::endl;
	}

	void SerialXbee::AsyncReadFrame() {
		boost::asio::async_read_until(m_port, buffer, 0x7E, boost::bind(&SerialXbee::ParseFrame, this, _1, _2));
	}

	/*void SerialXbee::PrintFrame(const boost::system::error_code &e, std::size_t size) {
		std::cout << "PrintFrame function was entered!" << std::endl;
		using namespace boost::asio;
		// Throw exception later
		if (e || size <= 0) {
			std::cout << "Nothing was read" << std::endl;
		}
		// Get rid of else after exception is added above
		else {
			// Figure out a better way to handle all the read calls error codes...
			// Possible bug: I don't check the second error code
			read(io_port, buffer, transfer_exactly(3));
			std::istream input_temp(&buffer);
			// Throw away delimiter
			input_temp.get();
			// Read in Frame Length
			uint16_t frame_length;
			input_temp.get((char*)&frame_length, 2);
			// Read in Frame Type
			uint8_t frame_type = input_temp.get();
			// Gather the rest of the data based on frame type
			// Add this to a function for modularity later
			switch(frame_type) {
				case (uint8_t) FrameType::RECIEVE_PACKET:
				{
					read(io_port, buffer, transfer_exactly(frame_length - 1));
					// This maybe works?
					input_temp.rdbuf(&buffer);
					// Get 64 bit mac address
					uint64_t mac_64;
					input_temp.get((char*)&mac_64, 8);
					// Get 16 bit mac address
					uint16_t mac_16;
					input_temp.get((char*)&mac_16, 2);
					// Get Recieve options
					uint8_t r_opt = input_temp.get();
					// Get Data as string;
					std::string r_data(frame_length - 12, '\0');
					input_temp.read(&r_data[0], frame_length - 12);
					std::cout << "Packet Payload is: " << r_data;
					//RecievePacket recieve(mac_64, mac_16, r_opt, r_data);
					break;
				}
				default:
					// Throw some error
					break;
			}
			// User callback with Frame type?
		}
	}*/
}