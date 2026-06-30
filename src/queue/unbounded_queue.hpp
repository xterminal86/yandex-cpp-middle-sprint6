#pragma once
#include "queue/queue.hpp"

#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue
{
  public:
    using Action = std::function<void()>;

    UnboundedQueue() = default;

    void push(Action task) override;

    std::optional<Action> try_pop() override;

  private:
    std::queue<Action> _queue;
    std::mutex _mutex;
};

}  // namespace dispatcher::queue