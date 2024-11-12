#include <iostream>
#include "connection_azure_routes.h"
#include "azureiot/iothub.h"
#include "azure_c_shared_utility/threadapi.h"


IOTHUBMESSAGE_DISPOSITION_RESULT callback_1(IOTHUB_MESSAGE_HANDLE message,
                                            void* userContextCallback){
  connection_azure_routes* connection;
  connection = (connection_azure_routes*)(userContextCallback);

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
      connection->add_message_to_queue(str);
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

  if (IoTHub_Init() == 0) {
    handle = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol);
    if (handle != NULL){
      IOTHUB_CLIENT_RESULT cresult;
      cresult = IoTHubModuleClient_LL_SetInputMessageCallback(
                                                        handle,
                                                        "data_serial_input",
                                                        callback_1,
                                                        this);
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


struct EVENT_INSTANCE {
  IOTHUB_MESSAGE_HANDLE messageHandle;
  size_t messageTrackingId; // For tracking the instances in the user callback.
};

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result,
                                    void* userContextCallback) {
  EVENT_INSTANCE* eventInstance = (EVENT_INSTANCE*)userContextCallback;
  /* Some device specific action code goes here... */
  size_t id = eventInstance->messageTrackingId;
  IoTHubMessage_Destroy(eventInstance->messageHandle);
  delete eventInstance;
  // std::cout << "deleted eventInstance with id "<< id <<std::endl;
}

void connection_azure_routes::publish(std::string topic, std::string msg){
  static size_t iterator = 0;
  EVENT_INSTANCE* instance = new EVENT_INSTANCE();
  instance->messageHandle = IoTHubMessage_CreateFromString(msg.c_str());

  if (instance->messageHandle != NULL) {
    IoTHubMessage_SetMessageId(instance->messageHandle, "MSG_ID");
    IoTHubMessage_SetCorrelationId(instance->messageHandle, "CORE_ID");

    instance->messageTrackingId = iterator;
    iterator++;

    IOTHUB_CLIENT_RESULT result;
    result = IoTHubModuleClient_LL_SendEventToOutputAsync(handle,
                                                    instance->messageHandle,
                                                    topic.c_str(),
                                                    SendConfirmationCallback,
                                                    instance);
    if (result != IOTHUB_CLIENT_OK) {
      std::cerr << "ERROR: IoTHubModuleClient_LL_SendEventAsync......FAILED!";
      std::cerr << std::endl;
    }
  }
  else {
    std::cerr<<"ERROR: iotHubMessageHandle is NULL!"<<std::endl;
  }
}
