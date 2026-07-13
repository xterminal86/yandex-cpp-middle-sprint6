#pragma once

#include "priority_queue.hpp"

using namespace dispatcher::queue;

namespace dispatcher::thread_pool {

class ThreadPool
{
  public:
    ThreadPool(std::shared_ptr<PriorityQueue> queueRef, size_t threadsNum);
    ~ThreadPool();

    void stop();

  private:
    void start();

    std::shared_ptr<PriorityQueue> _queueRef;
    size_t _threadsNum;

    std::vector<std::jthread> _workers;

    std::atomic<bool> _shouldStop = false;
};

} // namespace dispatcher::thread_pool
