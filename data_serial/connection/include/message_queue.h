
#include <memory>
#include <mutex>
#include <queue>
#include <string>

class message_queue {
  std::queue<std::string> messages_;
  std::mutex message_mutex;
  size_t max_messages_;
 public:
  message_queue(size_t max_messages);
  std::string get_popped_message();
  void add_message_to_queue(std::string msg);
};