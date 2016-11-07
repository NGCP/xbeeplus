#pragma once

#ifndef SERIALDEVICE_HPP
#define SERIALDEVICE_HPP

#include "../include/Frame.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>

namespace XBEE {
	class SerialDevice {
	public:
		SerialInterface();
		~SerialInterface();
		SerialInterface(const std::string &port_name, int baud = 57600)

		Frame readFrame();
	};
}
#endif