#include "boost/asio/serial_port.hpp"
using spb  = boost::asio::serial_port_base;

std::string get_serial_port_name();
uint32_t get_serial_bauderate();
uint8_t get_serial_char_size();
spb::parity::type get_serial_parity();
spb::stop_bits::type get_serial_stop_bits();
spb::flow_control::type get_serial_flow_control();