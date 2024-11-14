#include <iostream>
#include "device_client.h"

device_client::device_client(std::string group_id,
                              std::string node_id,
                              nlohmann::json& config)
                              : group_id_(group_id),
                                edge_node_id_(node_id),
                                config_(config){
  device_id_ = config["name"];
  update(config);
  set_topics();
}
void device_client::set_topics(){
  topic_ddata_ = "spBv1.0/";
  topic_ddata_ += group_id_ + "/DDATA/";
  topic_ddata_ += edge_node_id_+"/"+device_id_;
}

void device_client::dbirth_send(struct mosquitto* m){
  // TODO :
}
void device_client::ddeath_send(struct mosquitto* m){
  // TODO :
}
void device_client::ddata_send(struct mosquitto* m){
  org_eclipse_tahu_protobuf_Payload ddata_payload;
  get_next_payload(&ddata_payload);

  attribute_host_.ddata_gen(ddata_payload);
  if(ddata_payload.metrics_count > 0){
    size_t buffer_length = 1024;
    uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
    size_t message_length = encode_payload(binary_buffer,
                                            buffer_length,
                                            &ddata_payload);

    mosquitto_publish(m,
                      NULL,
                      topic_ddata_.c_str(),
                      message_length,
                      binary_buffer,
                      0,
                      false);
    std::cout << "ddata_send sent payload to topic "<< topic_ddata_ <<  std::endl;

    free(binary_buffer);
  }
  else{
    std::cout << "no payloadto send" <<  std::endl;
  }
  free_payload(&ddata_payload);
}
void device_client::dcmd_pass(std::string command){
  // TODO :
}
void device_client::update(nlohmann::json& j){
  if(j.contains("attributes")){
    attribute_host_.update_attributes_from_array(j["attributes"]);
  }
  else{
    std::cout << "attributes section not found in msg :"<< j.dump()<<std::endl;
  }
}