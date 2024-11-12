#include "message_queue.h"

message_queue::message_queue(size_t max_messages) : max_messages_(max_messages){
}

std::string message_queue::get_popped_message(){
  std::lock_guard<std::mutex> lock(message_mutex);
  std::string out = "";
  if(messages_.size()>0){
    out = messages_.front();
    messages_.pop();
  }
  return out;
}

void message_queue::add_message_to_queue(std::string msg){
  while(messages_.size() > max_messages_){
    messages_.pop();
  }
  std::lock_guard<std::mutex> lock(message_mutex);
  messages_.push(msg);
}