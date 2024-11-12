#include "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"
#include "boost/algorithm/string.hpp"


#include <csignal>
#include <iostream>
#include <thread>

#include <mosquitto.h>
#include "nlohmann/json.hpp"

#include "connection_factory.h"

#define DEFAULT_SERIAL_PORT_NAME "/dev/ttyUSB0"

std::string serial_tag = "--serial=";
boost::asio::io_service m_ioService;
boost::asio::serial_port serial_port(m_ioService);

void exit_application(int signum) {
  std::cout  << "exiting sub application..."<<std::endl;
  exit(0);
}

void sig_int_handler(int signum) {
  std::cout << std::endl;
  serial_port.close();

  std::cout << "ctrl+c pressed, exiting..."<<std::endl;
  exit_application(1);
}

std::string get_serial_port_name(int argc, char* argv[]){
  std::string out = DEFAULT_SERIAL_PORT_NAME;
  for(size_t i = 0; i <= (argc-1); i++) {
    std::string str(argv[i]);
    if(str.find(serial_tag) != std::string::npos){
      out = str.erase(0,serial_tag.length());
    }
  }
  return out;
}

nlohmann::ordered_json gen_metrics_from_serial(std::string str){
  nlohmann::ordered_json j;
  std::vector<std::string> parts;
  boost::split(parts,str,boost::is_any_of(","));

  size_t counter = 0;
  for(auto part : parts) {
    std::string metric_name = "metric"+std::to_string(counter);
    boost::trim(part);
    j[metric_name] = part;
    counter++;
  }

  return j;
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sig_int_handler); // registar for ctrl-c
  signal(SIGTERM, exit_application); // terminate from Docker STOPSIGNAL

  std::string serial_port_name = get_serial_port_name(argc,argv);
  serial_port.open(serial_port_name);

  // parameters for reading from proxybox
  serial_port.set_option(boost::asio::serial_port_base::baud_rate(115200));
  serial_port.set_option(boost::asio::serial_port_base::character_size(8));
  serial_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  serial_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  serial_port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

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
      serial_port.read_some(boost::asio::buffer(&c, 1));
      str += c;
    }

    nlohmann::ordered_json j = gen_metrics_from_serial(str);
    std::cout << j.dump() << std::endl;

    connection->publish("data_serial_output",j.dump());
  }

  std::cout << "...exiting main.  goodbye." << std::endl;
}