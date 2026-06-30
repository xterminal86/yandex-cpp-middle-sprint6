#include <gtest/gtest.h>
#include <thread>
#include <print>

#include "queue/unbounded_queue.hpp"

using namespace dispatcher::queue;

constexpr size_t MaxIterations = 10;

#define CREATE_THREAD(name)                                         \
  std::jthread pusher##name(                                        \
    [&q]()                                                          \
    {                                                               \
      for (size_t j = 0; j < MaxIterations; j++)                    \
      {                                                             \
        q.push(                                                     \
          [j]()                                                     \
          {                                                         \
            std::println("pusher{}, iteration {}", #name, (j + 1)); \
          }                                                         \
        );                                                          \
      }                                                             \
    }                                                               \
  );

TEST(UnboundedQueue, Test)
{
  UnboundedQueue q;

  //
  // Joinable threads will get join at the end if }, so we basically have to
  // paste shit manually in order to see that the order in which threads are
  // created is indeed non-deterministic.
  //
  CREATE_THREAD(1);
  CREATE_THREAD(2);
  CREATE_THREAD(3);
  CREATE_THREAD(4);

  std::jthread reader(
    [&q]()
    {
      for (size_t i = 0; i < 4; i++)
      {
        for (size_t j = 0; j < MaxIterations; j++)
        {
          std::optional<UnboundedQueue::Action> res = q.try_pop();
          if (res)
          {
            (*res)();
          }
          else
          {
            std::println("Queue is empty!");
          }
        }
      }
    }
  );
}