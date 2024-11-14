#include <sys/types.h>
#include "tahu.h"
#include "attribute.h"
attribute::attribute(uint64_t datatype) : datatype_(datatype) {
  switch (datatype_){
    case METRIC_DATA_TYPE_INT64:
      value_ = (void*)(new uint64_t);
      break;
    case METRIC_DATA_TYPE_DOUBLE:
      value_ = (void*)(new double);
      break;
    case METRIC_DATA_TYPE_STRING:
      value_ = (void*)(new std::string);
      break;


  }}
void attribute::set_value_with_timetamp(nlohmann::json& j_val,
                                              steady_tp time){
  void* data = NULL;
  switch (datatype_){
    case METRIC_DATA_TYPE_INT64:
      if (j_val.is_number_integer()){
        *((uint64_t*)(value_)) = j_val;
        time_recv_ = time;
      }
      break;
    case METRIC_DATA_TYPE_DOUBLE:
      if (j_val.is_number_float()){
        *((double*)(value_)) = j_val;
        time_recv_ = time;
      }
      break;
    case METRIC_DATA_TYPE_STRING:
      if (j_val.is_string()){
        *((std::string*)(value_)) = j_val;
        time_recv_ = time;
      }
      break;
  }
}

const void* attribute::get_value(){
  return value_;
}
uint64_t attribute::get_datatype(){
  return datatype_;
}
bool attribute::is_recently_published(){
  return time_sent_ < time_recv_;
}
uint64_t attribute::reported_epoch_get(){
  return reported_epoch_;
}

void attribute::reported_epoch_set(uint64_t epoch){
  reported_epoch_ = epoch;
}
