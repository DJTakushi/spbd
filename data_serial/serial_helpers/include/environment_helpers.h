#include "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"

using spb  = boost::asio::serial_port_base;
using boost_serial_port = boost::asio::serial_port;
using boost_service = boost::asio::io_service;

std::string get_serial_port_name();
uint32_t get_serial_bauderate();
uint8_t get_serial_char_size();
spb::parity::type get_serial_parity();
spb::stop_bits::type get_serial_stop_bits();
spb::flow_control::type get_serial_flow_control();

std::shared_ptr<boost_serial_port> get_serial_port(boost_service& service);
