#pragma once

#ifndef SERIALXBEE_HPP
#define SERIALXBEE_HPP

#include <functional>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Xbee.hpp"
#include "Frame.hpp"

#ifdef _WIN32
	// Figure out path later
	static const std::string kDefaultPath = "COM0";
#elif __linux__
	static const std::string kDefaultPath = "/dev/ttyUSB0";
#endif

// TODO: Frank needs to add a callback function to his GCS interface
namespace XBEE {
	class SerialXbee : boost::noncopyable {
	private:
		boost::asio::streambuf buffer;
		boost::asio::io_service m_io;
		boost::asio::serial_port m_port;
		boost::thread runner;

		// Parses the incoming data into appropriate Frames
		void ParseFrame(const boost::system::error_code &error, size_t num_bytes);
		// Handles Errors and executes callback for AsyncWriteFrame
		void FrameWritten(const boost::system::error_code &error, size_t num_bytes, Frame *a_frame);
		// Used as the default call back function for reads/writes, if none are specified
		void PrintFrame(Frame *a_frame);

	public:
		std::function<void(Frame *)> ReadHandler;
		std::function<void(Frame *)> WriteHandler;
		SerialXbee();
		// TODO: Add a blocking (synchronous) read function
		void AsyncReadFrame();
		// TODO: Add a blocking (synchronous) write function
		void AsyncWriteFrame(Frame *a_frame);
		// TODO: Add support for port options, data bit size, parity etc...
		void Connect(std::string device_path, uint32_t baud_rate = 57600);
	};
}
#endif