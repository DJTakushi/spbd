#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

#include <csignal>
#include <iostream>

LibSerial::SerialPort serial_port;
// LibSerial::SerialStream my_serial_stream( "/dev/ttyUSB0" ) ;


void exit_application(int signum) {
  std::cout  << "exiting sub application..."<<std::endl;
  exit(0);
}

void sig_int_handler(int signum) {
  std::cout << std::endl;
  serial_port.Close();
  // my_serial_stream.Close();

  std::cout << "ctrl+c pressed, exiting..."<<std::endl;
  exit_application(1);
}

int main(int argc, char* argv[]) {
  std::cout << "hello world!" << std::endl;

  signal(SIGINT, sig_int_handler); // registar for ctrl-c
  signal(SIGTERM, exit_application); // terminate from Docker STOPSIGNAL

  serial_port.Open("/dev/pts/15");
  // my_serial_stream.Open("/dev/pts/15");

  while(true){
    std::string str;
    try{
      serial_port.ReadLine(str,'\n',10);
      std::cout << "str : " << str;;
      // char next_char;
      // my_serial_stream.read(next_char);
      // my_serial_stream >> next_char;
      // std::cout << "str : " <<  next_char << str;;
    }
    catch(LibSerial::ReadTimeout){ }

  }

  std::cout << "...exiting main.  goodbye." << std::endl;
}