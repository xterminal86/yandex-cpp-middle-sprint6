#include <gtest/gtest.h>
#include <thread>
#include <atomic>

#include "queue/unbounded_queue.hpp"
#include "logger.hpp"

using namespace dispatcher;
using namespace dispatcher::queue;

TEST(UnboundedQueue, Test100Producers1Conusmer)
{
  UnboundedQueue q;

  auto _create_thread = [&q](const size_t index)
  {
    return std::jthread(
      [&q, index]
      {
        q.push(
          [&q, index]()
          {
            Logger::Get().Log(std::format("Task {}", index));
          }
        );
      }
    );
  };

  size_t maxThreads = 100;

  std::vector<std::jthread> threads;

  for (size_t i = 0; i < maxThreads; i++)
  {
    threads.push_back(_create_thread(i + 1));
  }

  for (auto& t : threads)
  {
    t.join();
  }

  //
  // This will signal that producers are finished.
  //
  q.push(Action());

  std::jthread reader(
    [&q]()
    {
      //
      // Even though q.empty() is atomic from the client's perspective, we
      // still can't rely on it because we don't know whether empty() means that
      // consumers are just froze, or haven't started yet or indeed finished.
      //
      while (true)
      {
        std::optional<Action> res = q.try_pop();
        if (res)
        {
          if (IsActionEmpty(*res))
          {
            Logger::Get().Log("Sentinel value hit - exiting");
            break;
          }
          else
          {
            (*res)();
          }
        }
      }
    }
  );

  //
  // Without join q.empty() might still not be empty (see above).
  //
  reader.join();

  ASSERT_EQ(0, q.size());
}

// =============================================================================

TEST(UnboundedQueue, Test10Producers3Conusmer)
{
  UnboundedQueue q;

  auto _create_thread = [&q](const size_t index)
  {
    return std::jthread(
      [&q, index]
      {
        q.push(
          [&q, index]()
          {
            for (size_t i = 0; i < 10; i++)
            {
              Logger::Get().Log(
                std::format("Task {:02d}, some data {}", index, i)
              );
              std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
          }
        );
      }
    );
  };

  size_t producerThreads = 10;

  std::vector<std::jthread> producers;

  for (size_t i = 0; i < producerThreads; i++)
  {
    producers.push_back(_create_thread(i + 1));
  }

  for (auto& t : producers)
  {
    t.join();
  }

  size_t consumerThreads = 3;

  std::vector<std::jthread> consumers;

  std::atomic<uint64_t> emptyCounter = 0;

  for (size_t i = 0; i < consumerThreads; i++)
  {
    std::jthread reader(
      [&q, &emptyCounter]()
      {
        while (true)
        {
          //
          // Assume we're done (+/- 3 consumers waiting for 1 second).
          //
          if (emptyCounter >= 30)
          {
            break;
          }

          std::optional<Action> res = q.try_pop();
          if (res)
          {
            (*res)();
          }
          else
          {
            Logger::Get().Log("empty");
            emptyCounter++;
            std::this_thread::sleep_for(std::chrono::seconds(1));
          }
        }
      }
    );

    consumers.push_back(std::move(reader));
  }

  for (auto& t : consumers)
  {
    t.join();
  }

  EXPECT_EQ(0, q.size());
}