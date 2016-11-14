#pragma once

#ifndef SERIALXBEE_HPP
#define SERIALXBEE_HPP

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

namespace XBEE {
	class SerialXbee : boost::noncopyable {
	private:
		boost::asio::streambuf buffer;
		boost::asio::io_service m_io;
		boost::asio::serial_port m_port;
		boost::thread runner;

		void ParseFrame(const boost::system::error_code &error, size_t num_bytes);

	public:
		SerialXbee();
		// TODO: Add a blocking (synchronous) read function
		void AsyncReadFrame();
		// TODO: Add support for port options, data bit size, parity etc...
		void Connect(std::string device_path, uint32_t baud_rate = 57600);
		// Used as the default call back function, if none are specified
		void PrintFrame(Frame *a_frame);
	};
}
#endif