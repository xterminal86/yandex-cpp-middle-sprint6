#include <gtest/gtest.h>

#include "task_dispatcher.hpp"
#include "types.hpp"

using namespace dispatcher;

TEST(TaskDispatcher, ScheduleTest)
{
  TaskDispatcher d(1);

  EXPECT_FALSE(d.schedule((TaskPriority)3, []() { int a = 42; }));
}

// здесь ваш код