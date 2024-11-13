#include <iostream>
#include "environment_helpers.h"

std::string get_serial_port_name(){
  std::string port_name = "/dev/ttyUSB0";
  char* port_name_env = std::getenv("SERIAL_PORT_NAME");
  if (port_name_env != NULL){
    port_name = std::stoi(port_name_env);
  }
  return port_name;
}
uint32_t get_serial_bauderate(){
  uint32_t bauderate = 115200;
  char* baude_env = std::getenv("SERIAL_BAUDERATE");
  if (baude_env != NULL){
    bauderate = std::stoi(baude_env);
  }
  return bauderate;
};
uint8_t get_serial_char_size(){
  uint8_t char_size = 8;
  char* char_size_env = std::getenv("SERIAL_CHAR_SIZE");
  if (char_size_env != NULL){
    char_size = std::stoi(char_size_env);
  }
  return char_size;
}
spb::parity::type get_serial_parity(){
  spb::parity::type parity = spb::parity::type::none;
  char* parity_env = std::getenv("SERIAL_PARITY");
  if (parity_env != NULL){
    parity = spb::parity::type(std::stoi(parity_env));
  }
  return parity;
}
spb::stop_bits::type get_serial_stop_bits(){
  spb::stop_bits::type stop_bits = spb::stop_bits::type::one;
  char* stop_bits_env = std::getenv("SERIAL_STOP_BITS");
  if (stop_bits_env != NULL){
    stop_bits = spb::stop_bits::type(std::stoi(stop_bits_env));
  }
  return stop_bits;
}
spb::flow_control::type get_serial_flow_control() {
  spb::flow_control::type  flow_control = spb::flow_control::type::none;
  char* flow_control_env = std::getenv("SERIAL_FLOW_CONTROL");
  if (flow_control_env != NULL){
    flow_control = spb::flow_control::type(std::stoi(flow_control_env));
  }
  return flow_control;
}

std::shared_ptr<boost_serial_port> get_serial_port(boost_service& service){
  std::shared_ptr<boost_serial_port> serial_port_;
  serial_port_ = std::make_shared<boost::asio::serial_port>(service);

  std::string serial_port_name = get_serial_port_name();
  serial_port_->open(serial_port_name);
  std::cout << "opened serial port " << serial_port_name << std::endl;

  // parameters for reading from proxybox
  uint32_t baud_rate_ = get_serial_bauderate();
  uint8_t serial_char_size_ = get_serial_char_size();
  spb::parity::type serial_parity_ = get_serial_parity();
  spb::stop_bits::type serial_stop_bits_ = get_serial_stop_bits();
  spb::flow_control::type serial_flow_control_ = get_serial_flow_control();

  serial_port_->set_option(spb::baud_rate(baud_rate_));
  std::cout << "set baud_rate : " << baud_rate_ << std::endl;

  serial_port_->set_option(spb::character_size(serial_char_size_));
  std::cout << "set char_size : " << int(serial_char_size_) << std::endl;

  serial_port_->set_option(spb::parity(serial_parity_));
  std::cout << "set parity : " << int(serial_parity_) << std::endl;

  serial_port_->set_option(spb::stop_bits(serial_stop_bits_));
  std::cout << "set stop bits : " << int(serial_stop_bits_) << std::endl;

  serial_port_->set_option(spb::flow_control(serial_flow_control_));
  std::cout << "set flow control : " << int(serial_flow_control_) << std::endl;

  return serial_port_;
}
