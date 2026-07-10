#include "task_dispatcher.hpp"

namespace dispatcher {

TaskDispatcher::TaskDispatcher(size_t threads)
{
  _priorityQueue = std::make_unique<PriorityQueue>(
    std::map<TaskPriority, QueueOptions>
    {
        { TaskPriority::High,   { true,  1000         } }
      , { TaskPriority::Normal, { false, std::nullopt } }
    }
  );
}

} // namespace dispatcher