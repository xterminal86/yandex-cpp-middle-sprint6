#include "queue/priority_queue.hpp"
#include "logger.hpp"

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

  //Logger::Get().Log("  PriorityQueue::Pop()");

  std::optional<Action> res;

  res = _queueMap[TaskPriority::High]->try_pop();

  if (not res)
  {
    //Logger::Get().Log("  nothing in High, checking Normal...");
    res = _queueMap[TaskPriority::Normal]->try_pop();
    /*
    if (not res)
    {
      Logger::Get().Log("  still nothing");
    }
    */
  }

  return res;
}

// =============================================================================

void PriorityQueue::push(TaskPriority priority, Action task)
{
  //Logger::Get().Log("  PriorityQueue::Push()");
  _queueMap[priority]->push(task);
}

// =============================================================================

void PriorityQueue::shutdown()
{
  //Logger::Get().Log("  PriorityQueue::Shutdown()");

  _shutdown = true;

  for (auto& kvp : _queueMap)
  {
    //Logger::Get().Log(std::format("  {} - stop", (int)kvp.first));
    kvp.second->stop();
    kvp.second->ForceNotify();
  }
}

} // namespace dispatcher::queue