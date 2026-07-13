#pragma once

#include "priority_queue.hpp"

using namespace dispatcher::queue;

namespace dispatcher::thread_pool {

class ThreadPool
{
  public:
    ThreadPool(std::shared_ptr<PriorityQueue> queueRef, size_t threadsNum);
    ~ThreadPool();

  private:
    void Start();

    std::shared_ptr<PriorityQueue> _queueRef;
    size_t _threadsNum;
};

} // namespace dispatcher::thread_pool
