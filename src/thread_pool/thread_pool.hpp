#pragma once

#include "priority_queue.hpp"

using namespace dispatcher::queue;

namespace dispatcher::thread_pool {

class ThreadPool
{
  public:
    ThreadPool(std::shared_ptr<PriorityQueue> q, size_t threadsNum);
    ~ThreadPool();

  private:
    void Start();

    std::shared_ptr<PriorityQueue> _queue;
    size_t _threadsNum;
};

} // namespace dispatcher::thread_pool
