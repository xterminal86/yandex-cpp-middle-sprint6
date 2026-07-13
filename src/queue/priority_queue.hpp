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
#include <map>
#include <format>
#include <print>
#include <condition_variable>
#include <thread>

namespace dispatcher::queue {

class PriorityQueue
{
  public:
    explicit PriorityQueue(const std::map<TaskPriority, QueueOptions>& config);
    ~PriorityQueue() = default;

    void push(TaskPriority priority, const Action& task);

    // block on pop until shutdown is called
    // after that return std::nullopt on empty queue
    std::optional<Action> pop();

    void shutdown();

  private:

    std::map<TaskPriority, QueueOptions> _config;

    using QueueMap = std::map<TaskPriority, std::unique_ptr<IQueue>>;
    QueueMap _queueMap;

    std::mutex _mutex;
    std::condition_variable _popBlocker;

    std::atomic<bool> _shutdown = false;
};

}  // namespace dispatcher::queue
