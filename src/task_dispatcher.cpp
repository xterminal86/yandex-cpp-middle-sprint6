#include "task_dispatcher.hpp"
#include "logger.hpp"

namespace dispatcher {

TaskDispatcher::TaskDispatcher(size_t threads)
{
  _priorityQueue = std::make_shared<PriorityQueue>(
    std::map<TaskPriority, QueueOptions>
    {
        { TaskPriority::High,   { true,  1000         } }
      , { TaskPriority::Normal, { false, std::nullopt } }
    }
  );

  _threadPool = std::make_unique<ThreadPool>(_priorityQueue, threads);
}

// =============================================================================

TaskDispatcher::~TaskDispatcher()
{
  _threadPool->stop();
  //Logger::Get().Log("~TaskDispatcher()");
}

// =============================================================================

bool TaskDispatcher::schedule(TaskPriority priority, Action task)
{
  switch (priority)
  {
    case TaskPriority::Normal:
    case TaskPriority::High:
    {
      _priorityQueue->push(priority, task);
    }
    break;

    default:
    {
      Logger::Get().Log(
        std::format("Unexpected task priority {}", (int)priority)
      );
      return false;
    }
    break;
  }

  return true;
}

} // namespace dispatcher