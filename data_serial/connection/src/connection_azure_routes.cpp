#include <iostream>
#include "connection_azure_routes.h"
#include "azureiot/iothub.h"
#include "azure_c_shared_utility/threadapi.h"


class callback_1_context{
 public:
  std::shared_ptr<message_queue> message_queue_;
  callback_1_context(std::shared_ptr<message_queue> queue){
    message_queue_ = queue;
  };
};

IOTHUBMESSAGE_DISPOSITION_RESULT callback_1(IOTHUB_MESSAGE_HANDLE message,
                                            void* userContextCallback){
  callback_1_context* context = (callback_1_context*)(userContextCallback);

  IOTHUBMESSAGE_CONTENT_TYPE contentType = IoTHubMessage_GetContentType(message);
  if (contentType != IOTHUBMESSAGE_BYTEARRAY) {
    /*  The transport will only create messages of type IOTHUBMESSAGE_BYTEARRAY,
        never IOTHUBMESSAGE_STRING.*/
    std::cout << "Warning: Unknown content type "<<int(contentType)<< \
        "[%d] received for message.  Cannot display"<<std::endl;
  }
  else {
    /*  IoTHubMessage_GetByteArray retrieves a shallow copy of the data.
        Caller must NOT free messageBody.*/
    IOTHUB_MESSAGE_RESULT messageResult;
    unsigned const char* messageBody;
    size_t messageBodyLength;

    messageResult = IoTHubMessage_GetByteArray(message, &messageBody,
                                                &messageBodyLength);
    if (messageResult == IOTHUB_MESSAGE_OK) {
      std::string str((const char*)(messageBody),messageBodyLength);
      context->message_queue_->add_message_to_queue(str);
    }
    else {
      std::cout <<" WARNING: messageBody = NULL"<<std::endl;
    }
  }

  return IOTHUBMESSAGE_ACCEPTED;
};

connection_azure_routes::connection_azure_routes(size_t max_messages)
    : connection_base(max_messages){}

bool connection_azure_routes::initialize(){
  bool good = true;

  if (IoTHub_Init() != 0) {
    handle = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol);
    if (handle == NULL){

      callback_1_context* context = new callback_1_context(received_queue_);

      IOTHUB_CLIENT_RESULT cresult;
      cresult = IoTHubModuleClient_LL_SetInputMessageCallback(
                                                        handle,
                                                        "data_serial_input",
                                                        callback_1,
                                                        context);
      if (cresult == IOTHUB_CLIENT_OK) {
        good = true;
      }
      else {
        std::cerr << "ERROR: setting callback FAILED!"<<std::endl;
      }
    }
    else {
      std::cerr << "ERROR: handle is NULL!" << std::endl;
      good = false;
    }
  }
  else {
    std::cerr << "Failed to initialize the platform." << std::endl;
    good = false;
  }
  return good;
}

void connection_azure_routes::start_loop(){
  active_=true;
  do_work_thread_ = std::thread([this](){
    while(this->active_){
      IoTHubModuleClient_LL_DoWork(handle);
      ThreadAPI_Sleep(10);
    }
  });
}

void connection_azure_routes::publish(std::string output_name, std::string msg){
  
}
