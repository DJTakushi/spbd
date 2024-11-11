#include "connection_mqtt.h"

bool connection_mqtt::initialize(){
  return true;
}

bool connection_mqtt::register_message_callback(std::string subscription,
                                                message_callback callback){
  return true;
}

void connection_mqtt::start_loop(){

}

void connection_mqtt::stop_loop(){

}
