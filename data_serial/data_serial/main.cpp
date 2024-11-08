#include <boost/asio.hpp>
#include "boost/asio/serial_port.hpp"

#include <csignal>
#include <iostream>
#include <thread>

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

int main(int argc, char* argv[]) {
  signal(SIGINT, sig_int_handler); // registar for ctrl-c
  signal(SIGTERM, exit_application); // terminate from Docker STOPSIGNAL

  std::string serial_port_name = "/dev/pts/12";

  for(size_t i = 0; i <= (argc-1); i++) {
    std::string str(argv[i]);
    if(str.find(serial_tag) != std::string::npos){
      serial_port_name = str.erase(0,serial_tag.length());
      std::cout << "serial_port_name : " << serial_port_name << std::endl;
    }
  }

  serial_port.open(serial_port_name);

  while(true){
    std::string str = "";
    char c = '0';

    while (c != '\n') {
      serial_port.read_some(boost::asio::buffer(&c, 1));
      str += c;
    }

    std::cout << "str : " << str;
  }

  std::cout << "...exiting main.  goodbye." << std::endl;
}