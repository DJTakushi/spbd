#include "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"
#include "boost/algorithm/string.hpp"


#include <csignal>
#include <iostream>
#include <thread>

#include "nlohmann/json.hpp"

#define DEFAULT_SERIAL_PORT_NAME "/dev/pts/12"

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
  std::cout << "opened serial port " << serial_port_name << std::endl;

  while(true){
    std::string str = "";
    char c = '0';

    while (c != '\n') {
      serial_port.read_some(boost::asio::buffer(&c, 1));
      str += c;
    }

    nlohmann::ordered_json j = gen_metrics_from_serial(str);
    std::cout << j.dump() << std::endl;
  }

  std::cout << "...exiting main.  goodbye." << std::endl;
}