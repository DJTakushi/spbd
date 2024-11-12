#include "connection_mqtt.h"
connection_mqtt::connection_mqtt(size_t max_messages)
    : connection_base(max_messages) {}

bool connection_mqtt::initialize(){
  return true;
}

void connection_mqtt::start_loop(){

}

void connection_mqtt::publish(std::string output_name, std::string msg){
  
}