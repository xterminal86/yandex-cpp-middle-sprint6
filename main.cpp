#include <chrono>
#include <climits>
#include <iostream>
#include <print>
#include <thread>
#include <format>

#include "logger.hpp"
#include "task_dispatcher.hpp"

using namespace dispatcher;

int main() {
  TaskDispatcher td(std::thread::hardware_concurrency());
  std::vector<std::jthread> threads;

  constexpr size_t ThreadsCount = 5;
  constexpr size_t ScheduledCount = 10;

  for (int i = 0; i < ThreadsCount; ++i)
  {
    threads.emplace_back(
      [&, i]()
      {
        for (int j = 0; j < ScheduledCount; j++)
        {
          td.schedule(
            TaskPriority::Normal,
            [=]()
            {
              Logger::Get().Log(
                std::format(
                  "T {} - Normal no {}",
                  std::this_thread::get_id(),
                  (10 * i + j)
                )
              );
              //Logger::Get().Log("Normal priority message №" + std::to_string(10 * i + j));
            }
          );

          td.schedule(
            TaskPriority::High,
            [=]()
            {
              Logger::Get().Log(
                std::format(
                  "T {} - HIGH no {}",
                  std::this_thread::get_id(),
                  (10 * i + j)
                )
              );
              //Logger::Get().Log("High priority message №" + std::to_string(10 * i + j));
            }
          );
        }
      }
    );
  }
}
