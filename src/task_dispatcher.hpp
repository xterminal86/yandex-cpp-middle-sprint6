#pragma once

#include <memory>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

using namespace dispatcher::queue;
using namespace dispatcher::thread_pool;

namespace dispatcher {

class TaskDispatcher
{
  public:
    TaskDispatcher(size_t thread_count);
    ~TaskDispatcher();

    bool schedule(TaskPriority priority, Action task);

  private:
    std::shared_ptr<PriorityQueue> _priorityQueue;
    std::unique_ptr<ThreadPool> _threadPool;
};

}  // namespace dispatcher