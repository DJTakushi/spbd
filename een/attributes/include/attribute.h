#pragma once
#include  <chrono>
#include <stdint.h>

typedef std::chrono::steady_clock::time_point steady_tp;

class attribute{
  void* value_;
  uint64_t datatype_;
  std::chrono::steady_clock::time_point time_recv_;
  std::chrono::steady_clock::time_point time_sent_;
 public:
  attribute(uint64_t datatype);
  void set_value(void* data);
  void* get_value();
  uint64_t get_datatype();
  bool must_publish();
};