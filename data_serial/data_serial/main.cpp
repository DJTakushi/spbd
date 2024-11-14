#include "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"
#include "boost/algorithm/string.hpp"

#include <chrono>
#include <csignal>
#include <ctime>
#include <iostream>
#include <thread>

#include <mosquitto.h>
#include "nlohmann/json.hpp"

#include "connection_factory.h"
#include "environment_helpers.h"

std::shared_ptr<boost::asio::serial_port> serial_port_; // global for handlers
typedef std::chrono::system_clock::time_point sys_tp;
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

nlohmann::ordered_json gen_metrics_from_serial(std::string str, sys_tp time){
  nlohmann::ordered_json j;
  j["name"]="data_serial";
  std::vector<std::string> parts;
  boost::split(parts,str,boost::is_any_of(","));

  size_t counter = 0;
  auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
  for(auto part : parts) {
    nlohmann::json attr;
    std::string metric_name = "metric"+std::to_string(counter);
    boost::trim(part);
    attr["name"] = metric_name;
    attr["datatype"] = 10;
    attr["value"] = std::stod(part);
    attr["timestamp"] = epoch;

    j["attributes"].emplace_back(attr);
    counter++;
  }

  return j;
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sig_int_handler); // registar for ctrl-c
  signal(SIGTERM, exit_application); // terminate from Docker STOPSIGNAL

  boost::asio::io_service m_ioService;
  serial_port_ = get_serial_port(m_ioService);

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
    sys_tp time_(std::chrono::system_clock::now());

    nlohmann::ordered_json j = gen_metrics_from_serial(str, time_);
    std::cout << j.dump() << std::endl;

    connection->publish("data_serial_output",j.dump());
  }

  std::cout << "...exiting main.  goodbye." << std::endl;
}