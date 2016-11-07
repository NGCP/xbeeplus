#include "../include/TransmitRequest.hpp"
#include "../include/Xbee.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/thread.hpp>

using namespace XBEE;
using namespace boost;
using namespace std;

asio::streambuf buffer;
asio::io_service a_serv;
asio::serial_port port(a_serv);

bool thread_is_running = true;
int i = 0;

void read_complete(const system::error_code &error, size_t bytes_transferred) {
	cout << "ENTERED CALLBACK!" << endl;
	if(error || bytes_transferred <= 0){
		cout << "ERROR: NOTHING WAS READ!";
	}
	else {
		system::error_code two;
		asio::read(port, buffer, asio::transfer_exactly(3), two);
		istream is(&buffer);
		string data(bytes_transferred + 2, '\0');
		is.read(&data[0], bytes_transferred + 2);

		cout << "Recieved Data: ";
		for(char &val : data) {
			cout << setfill('0') << setw(2) << hex << uppercase << static_cast<int>(val) << " ";
		}
		cout << endl;
	}
	asio::async_read_until(port, buffer, 0x7E, boost::bind(&read_complete, _1, _2));
	//thread_is_running = false;
}

int main(int argc, char* argv[]) {
	// Warning API level 1 (no escapes)
	TransmitRequest t1(0x0013A20040F8064D);
	//t1.SetData("Hello QuadC!");
	cout << "Enter TransmitRequest Payload: ";
	cin >> t1;
	cout << t1 << endl;

	port.open("/dev/ttyUSB0");
	port.set_option(asio::serial_port_base::baud_rate(57600));

	if (!port.is_open()) {
		cout << "Shit isn't open yo!'" << endl;
		port.close();
		return 1;
	}
	else {
		boost::thread(boost::bind(&boost::asio::io_service::run, &a_serv));
		asio::async_read_until(port, buffer, 0x7E, boost::bind(&read_complete, _1, _2));
		while(thread_is_running) {
			cout << "This is test #: " << std::dec << i++ << endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		port.close();
	}
}