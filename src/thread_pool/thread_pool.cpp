#include "thread_pool/thread_pool.hpp"
#include "logger.hpp"

namespace dispatcher::thread_pool
{

ThreadPool::ThreadPool(
  std::shared_ptr<PriorityQueue> queueRef, size_t threadsNum
) : _queueRef(queueRef), _threadsNum(threadsNum)
{
  _workers.reserve(threadsNum);

  start();
}

// =============================================================================

ThreadPool::~ThreadPool()
{
  while(true)
  {
    std::optional<Action> action = _queueRef->pop();
    if (not action.has_value())
    {
      Logger::Get().Log("No tasks left, fucking off.");
      break;
    }
    else
    {
      std::invoke(action.value());
    }
  }

  //Logger::Get().Log("~ThreadPool()");
}

// =============================================================================

void ThreadPool::start()
{
  for (size_t i = 0; i < _threadsNum; i++)
  {
    _workers.push_back(
      std::jthread(
        [this]()
        {
          while (not _shouldStop)
          {
            std::optional<Action> action = _queueRef->pop();
            if (action)
            {
              std::invoke(action.value());
            }
          }
        }
      )
    );
  }
}

// =============================================================================

void ThreadPool::stop()
{
  _shouldStop = true;
  _queueRef->shutdown();
}

} // namespace dispatcher::thread_pool
