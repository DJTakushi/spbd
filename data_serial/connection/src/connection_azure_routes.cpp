#include "connection_azure_routes.h"

bool connection_azure_routes::initialize(){
  return true;
}

bool connection_azure_routes::register_message_callback(
    std::string subscription,
    message_callback callback){
  return true;
}

void connection_azure_routes::start_loop(){

}

void connection_azure_routes::stop_loop(){

}
