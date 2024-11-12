#include "connection_base.h"

connection_base::connection_base(size_t max_messages){
  received_queue_ = std::make_shared<message_queue>(max_messages);
}

void connection_base::stop_loop(){
  active_ = false;
  if(do_work_thread_.joinable()){
    do_work_thread_.join();
  }
}
