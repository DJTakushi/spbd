#include "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"
#include "boost/algorithm/string.hpp"


#include <csignal>
#include <iostream>
#include <thread>

#include <mosquitto.h>
#include "nlohmann/json.hpp"

#include "connection_factory.h"
#include "environment_helpers.h"

std::shared_ptr<boost::asio::serial_port> serial_port_;

void exit_application(int signum) {
  std::cout  << "exiting sub application..."<<std::endl;
  exit(0);
}

void sig_int_handler(int signum) {
  // TODO : try relocating serial_port_->close() to exit_application()
  if(serial_port_!= NULL){
    serial_port_->close();
  }
  std::cout << "ctrl+c pressed, exiting..."<<std::endl;
  exit_application(1);
}

nlohmann::ordered_json gen_metrics_from_serial(std::string str){
  nlohmann::ordered_json j;
  std::vector<std::string> parts;
  boost::split(parts,str,boost::is_any_of(","));

  size_t counter = 0;
  for(auto part : parts) {
    std::string metric_name = "metric"+std::to_string(counter);
    boost::trim(part);
    j[metric_name] = std::stod(part);
    counter++;
  }

  return j;
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sig_int_handler); // registar for ctrl-c
  signal(SIGTERM, exit_application); // terminate from Docker STOPSIGNAL

  boost::asio::io_service m_ioService;
  serial_port_ = std::make_shared<boost::asio::serial_port>(m_ioService);

  std::string serial_port_name = get_serial_port_name();
  serial_port_->open(serial_port_name);
 
  // parameters for reading from proxybox
  serial_port_->set_option(spb::baud_rate(get_serial_bauderate()));
  serial_port_->set_option(spb::character_size(get_serial_char_size()));
  serial_port_->set_option(spb::parity(get_serial_parity()));
  serial_port_->set_option(spb::stop_bits(get_serial_stop_bits()));
  serial_port_->set_option(spb::flow_control(get_serial_flow_control()));

  std::cout << "opened serial port " << serial_port_name << std::endl;

  #ifdef AZURE_ROUTES
    connection_type type = kAzureIot;
  #else
    connection_type type = kMqtt;
  #endif
  std::shared_ptr<connection_i> connection = connection_factory::create(type);
  connection->initialize();
  connection->start_loop();

  while(true){
    std::string str = "";
    char c = '0';

    while (c != '\n') {
      serial_port_->read_some(boost::asio::buffer(&c, 1));
      str += c;
    }

    nlohmann::ordered_json j = gen_metrics_from_serial(str);
    std::cout << j.dump() << std::endl;

    connection->publish("data_serial_output",j.dump());
  }

  std::cout << "...exiting main.  goodbye." << std::endl;
}