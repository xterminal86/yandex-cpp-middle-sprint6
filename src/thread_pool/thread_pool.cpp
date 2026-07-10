#include "thread_pool/thread_pool.hpp"

namespace dispatcher::thread_pool
{

ThreadPool::ThreadPool(std::shared_ptr<PriorityQueue> q, size_t threadsNum)
  : _queue(q), _threadsNum(threadsNum)
{
  Start();
}

// =============================================================================

ThreadPool::~ThreadPool()
{
  std::println("~ThreadPool()");

  _queue->shutdown();

  while(true)
  {
    std::optional<Action> action = _queue->pop();
    if (not action.has_value())
    {
      std::println("No tasks left, fucking off.");
      break;
    }
    else
    {
      std::invoke(action.value());
    }
  }
}

// =============================================================================

void ThreadPool::Start()
{
}

} // namespace dispatcher::thread_pool
