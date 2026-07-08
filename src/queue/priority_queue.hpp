#pragma once
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"
#include "types.hpp"

#include <atomic>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace dispatcher::queue {

class PriorityQueue
{
    // здесь ваш код
  public:
    explicit PriorityQueue(/*const std::unordered_map<TaskPriority*/);
    ~PriorityQueue();

    void push(TaskPriority priority, Action task);

    // block on pop until shutdown is called
    // after that return std::nullopt on empty queue
    std::optional<Action> pop();

    void shutdown();
};

}  // namespace dispatcher::queue