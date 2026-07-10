#pragma once

#include <memory>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

using namespace dispatcher::queue;

namespace dispatcher {

class TaskDispatcher
{
  public:
    TaskDispatcher(size_t thread_count);

    void schedule(TaskPriority priority, Action task);
    ~TaskDispatcher();

  private:
    std::unique_ptr<PriorityQueue> _priorityQueue;
};

}  // namespace dispatcher