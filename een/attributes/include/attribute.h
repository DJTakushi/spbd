#pragma once
#include  <chrono>
#include <stdint.h>
#include <nlohmann/json.hpp>

typedef std::chrono::steady_clock::time_point steady_tp;

class attribute{
  const void* value_;
  uint64_t datatype_;
  std::chrono::steady_clock::time_point time_recv_{};
  std::chrono::steady_clock::time_point time_sent_{};
  uint64_t reported_epoch_{0};
 public:
  attribute(uint64_t datatype);
  void set_value_with_timetamp(nlohmann::json& j_val, steady_tp time);
  const void* get_value();
  uint64_t get_datatype();
  bool is_recently_published();
  uint64_t reported_epoch_get();
  void reported_epoch_set(uint64_t epoch);
};