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

void TaskDispatcher::schedule(TaskPriority priority, Action task)
{
  _priorityQueue->push(priority, task);
}

} // namespace dispatcher