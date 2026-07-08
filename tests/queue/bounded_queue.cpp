#include <gtest/gtest.h>
#include <thread>
#include <print>

#include "queue/bounded_queue.hpp"

using namespace dispatcher;
using namespace dispatcher::queue;

TEST(BoundedQueue, Test10Producers1Conusmer)
{
  BoundedQueue q(5);

  auto _create_thread = [&q](const size_t index)
  {
    return std::jthread(
      [&q, index]
      {
        q.push(
          [&q, index]()
          {
            std::println("Task {}", index);
          }
        );
      }
    );
  };

  std::vector<std::jthread> threads;

  threads.push_back(_create_thread(1));
  threads.push_back(_create_thread(2));
  threads.push_back(_create_thread(3));
  threads.push_back(_create_thread(4));
  threads.push_back(_create_thread(5));

  for (auto& t : threads)
  {
    t.join();
  }
}

