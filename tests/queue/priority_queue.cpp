#include <gtest/gtest.h>

#include "queue/priority_queue.hpp"
#include "logger.hpp"

using namespace dispatcher;
using namespace dispatcher::queue;

TEST(PriorityQuueue, CtorTest)
{
  {
    std::map<TaskPriority, QueueOptions> cfg =
    {
      std::map<TaskPriority, QueueOptions>
      {
          { TaskPriority::High,   { true,  std::nullopt } }
        , { TaskPriority::Normal, { false, std::nullopt } }
      }
    };

    EXPECT_ANY_THROW(PriorityQueue q(cfg));
  }
  // ---------------------------------------------------------------------------
  {
    std::map<TaskPriority, QueueOptions> cfg =
    {
      std::map<TaskPriority, QueueOptions>
      {
          { (TaskPriority)2,      { true,  1000 } }
        , { TaskPriority::Normal, { false, std::nullopt } }
      }
    };

    EXPECT_ANY_THROW(PriorityQueue q(cfg));
  }
}

// =============================================================================

TEST(PriorityQuueue, Test)
{
  std::map<TaskPriority, QueueOptions> cfg =
  {
    std::map<TaskPriority, QueueOptions>
    {
        { TaskPriority::High,   { true,  1000         } }
      , { TaskPriority::Normal, { false, std::nullopt } }
    }
  };

  PriorityQueue q(cfg);

  q.push(TaskPriority::High, []() { Logger::Get().Log("H 1"); });
  q.push(TaskPriority::High, []() { Logger::Get().Log("H 2"); });
  q.push(TaskPriority::High, []() { Logger::Get().Log("H 3"); });

  std::optional<Action> action = q.pop();
  if (action)
  {
    std::invoke(action.value());
  }
}
