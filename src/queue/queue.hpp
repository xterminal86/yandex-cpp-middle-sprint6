#pragma once
#include "types.hpp"

#include <functional>
#include <optional>
#include <atomic>

namespace dispatcher::queue {

struct QueueOptions
{
  bool bounded;
  std::optional<int> capacity;
};

class IQueue
{
  public:
    virtual ~IQueue() = default;
    virtual void push(const Action& task) = 0;
    virtual std::optional<Action> try_pop() = 0;

    void stop()
    {
      _enabled = false;
    }

    virtual void ForceNotify()
    {
    }

  protected:
    std::atomic<bool> _enabled = true;
};

}  // namespace dispatcher::queue
