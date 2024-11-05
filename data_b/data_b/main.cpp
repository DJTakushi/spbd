// based on azure-iot-sdk-c/iothub_client/samples/iothub_client_sample_module_sender/iothub_client_sample_module_sender.c

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define USE_EDGE_MODULES
#include "azureiot/iothub_module_client_ll.h"
#include "azureiot/iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "azureiot/iothub_client_options.h"
#include "azureiot/iothubtransportmqtt.h"
#include "azureiot/iothub.h"

#include "nlohmann/json.hpp"

#define MESSAGE_COUNT 500
#define DOWORK_LOOP_NUM 60


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

int main(void) {
  std::time_t now = std::time(nullptr);
  std::cout << "starting data_b main at " << std::asctime(std::localtime(&now));
  std::cout <<"..."  << std::endl;

  srand((unsigned int)time(NULL));
  double avgWindSpeed = 10.0;
  double minTemperature = 20.0;
  double minHumidity = 60.0;

  if (IoTHub_Init() != 0) {
    (void)printf("Failed to initialize the platform.\r\n");
    return 1;
  }
  IOTHUB_MODULE_CLIENT_LL_HANDLE handle;
  /* Note: You must use MQTT_Protocol as the argument below.
  Using other protocols will result in undefined behavior.*/
  handle = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol);
  if (handle == NULL) {
    (void)printf("ERROR: handle is NULL!\r\n");
    return 1;
  }

  std::cout << "starting loop..."<<std::endl;
  size_t iterator = 0;
  double engine_speed=0.0;
  do {
    double temperature = minTemperature + (rand() % 10);
    double humidity = minHumidity +  (rand() % 20);
    engine_speed+=0.01;
    nlohmann::json j;
    j["deviceId"] = "myFirstDevice";
    j["windSpeed"] = avgWindSpeed + (rand() % 4 + 2);
    j["temperature"] = temperature;
    j["humidity"] = humidity;
    j["engine_speed"] = engine_speed;

    EVENT_INSTANCE* instance = new EVENT_INSTANCE();
    instance->messageHandle = IoTHubMessage_CreateFromString(j.dump().c_str());

    if (instance->messageHandle == NULL) {
      (void)printf("ERROR: iotHubMessageHandle is NULL!\r\n");
    }
    else {
      (void)IoTHubMessage_SetMessageId(instance->messageHandle, "MSG_ID");
      (void)IoTHubMessage_SetCorrelationId(instance->messageHandle, "CORE_ID");

      instance->messageTrackingId = iterator;
      MAP_HANDLE propMap = IoTHubMessage_Properties(instance->messageHandle);
      std::string propText = temperature > 28 ? "true" : "false";
      Map_AddOrUpdate(propMap, "temperatureAlert", propText.c_str());

      IOTHUB_CLIENT_RESULT result;
      result = IoTHubModuleClient_LL_SendEventToOutputAsync(handle,
                                                      instance->messageHandle,
                                                      "temperatureOutput",
                                                      SendConfirmationCallback,
                                                      instance);
      if (result != IOTHUB_CLIENT_OK) {
        std::cerr << "ERROR: IoTHubModuleClient_LL_SendEventAsync......FAILED!";
        std::cerr << std::endl;
      }
    }

    IoTHubModuleClient_LL_DoWork(handle);
    ThreadAPI_Sleep(1000);
    iterator++;
  } while (1);

  // Loop while we wait for instances to drain off.
  size_t index = 0;
  for (index = 0; index < DOWORK_LOOP_NUM; index++) {
    IoTHubModuleClient_LL_DoWork(handle);
    ThreadAPI_Sleep(100);
  }

  IoTHubModuleClient_LL_Destroy(handle);

  (void)printf("Finished executing\n");
  IoTHub_Deinit();
  return 0;
}
