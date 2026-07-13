#include "queue/priority_queue.hpp"

namespace dispatcher::queue {

PriorityQueue::PriorityQueue(const std::map<TaskPriority, QueueOptions>& config)
  : _config(config)
{
  for (auto& kvp : _config)
  {
    TaskPriority priority    = kvp.first;
    const QueueOptions& opts = kvp.second;

    bool priorityOk =
      (priority == TaskPriority::Normal or priority == TaskPriority::High);

    if (not priorityOk)
    {
      std::string err = std::format("Unexpected priority {}", (int)priority);
      throw std::runtime_error(err);
    }

    std::unique_ptr<IQueue> q;

    if (opts.bounded)
    {
      if (not opts.capacity)
      {
        throw std::logic_error("Requested bounded queue with no capacity!");
      }

      q = std::make_unique<BoundedQueue>(opts.capacity.value());
    }
    else
    {
      q = std::make_unique<UnboundedQueue>();
    }

    _queueMap[priority] = std::move(q);
  }
}

// =============================================================================

std::optional<Action> PriorityQueue::pop()
{
  std::unique_lock lock(_mutex);

  _popBlocker.wait(
    lock,
    [this]()
    {
      return (_shutdown.load() == true)
          or (not _queueMap[TaskPriority::High].empty()
           or not _queueMap[TaskPriority::Normal].empty());
    }
  );

  return std::nullopt;
}

// =============================================================================

void PriorityQueue::push(TaskPriority priority, Action task)
{
}

// =============================================================================

void PriorityQueue::shutdown()
{
  _shutdown = true;
}

} // namespace dispatcher::queue