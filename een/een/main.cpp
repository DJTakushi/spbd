#include <iostream>
#include <thread>
#include "een.h"

void exit_application(int signum) {
  std::cout  << "exiting sub application..."<<std::endl;
  exit(0);
}
void sig_int_handler(int signum) {
  std::cout << std::endl;
  std::cout << "ctrl+c pressed, exiting..."<<std::endl;
  exit_application(1);
}

int main(int argc, char* argv[]) {
  std::shared_ptr<een> een_ = std::make_shared<een>("");

  std::cout << "starting service routine.."<<std::endl;
  while(een_->is_stable()){
    een_->service_mqtt();
    een_->service_iot_hub();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "exiting main.  goodbye."<<std::endl;
}