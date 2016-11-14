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
	}

	void SerialXbee::Connect(std::string device_path, uint32_t baud_rate) {
		m_port.open(device_path);
		m_port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
		runner = boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));
	}

	// TODO: Fix up this function so it actually parses the frame correctly
	void SerialXbee::ParseFrame(const boost::system::error_code &error, size_t num_bytes) {
		std::cout << "FOUND A FRAME TO PARSE!" << std::endl;
		using namespace boost::asio;

		//uint16_t frame_length;
		//uint8_t frame_type;
		std::string placeholder;
		Frame *cur_frame = NULL;

		if (error) {
			// throw an error, by repeating system error code
		}
		if (num_bytes != 2) {
			// throw an error, saying that there was other things read besides the frame delimiter
		}

		// Clears buffer (Should only contain 0x7E delimiter, which is added automatically when new Frame object is created)
		buffer.consume(num_bytes);
		// Synchronously read the next 2 bytes of Frame (Frame Length)
		read(m_port, buffer, transfer_exactly(2));
		std::istream temp(&buffer);
		//temp >> placeholder;
		std::cout << HexString(placeholder, true) << std::endl;
		read(m_port, buffer, transfer_exactly(3));
		//temp.rdbuf(&buffer);
		temp >> placeholder;
		std::cout << HexString(placeholder, true) << std::endl;
		/*temp.get((char*)&frame_length, 2);
		frame_type = temp.get();
		std::cout << "Frame Length: " << frame_length << std::endl;*/
		// Fill up buffer with the rest of packet data
		//read(m_port, buffer, transfer_exactly(frame_length - 1));
		// TODO: Find out if this is necessary?
		//temp.rdbuf(&buffer);

		std::cout << "READ FRAME HEADER" << std::endl;

		// Construct Frame object
		/*switch(FrameType(frame_type)) {
			case FrameType::RECEIVE_PACKET:
			{
				ReceivePacket frame;
				temp.get((char*)&(frame.source_mac_64), 8);
				temp.get((char*)&(frame.source_mac_16), 2);
				temp.get((char*)&(frame.options), 1);
				// Store into RecievePacket's data field
				for (auto itr = (frame.data).begin(); itr != (frame.data).end(); ++itr) {
					temp.get((char*)itr, 1);
				}
				frame.length = frame_length;
				temp.get((char*)&(frame.checksum), 1);
				cur_frame = &frame;
				break;
			}
			default:
				// Throw unable to parse frame error
				break;
		}*/
		// TODO: Add support for user callback function with Frame pointer type
		PrintFrame(cur_frame);
		AsyncReadFrame();
	}

	void SerialXbee::PrintFrame(Frame *a_frame) {
		std::cout << "ENTERED PRINT FRAME FUNCTION" << std::endl;
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