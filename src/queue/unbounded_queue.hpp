#pragma once
#include "queue.hpp"
#include "types.hpp"

#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue
{
  public:
    UnboundedQueue() = default;

    void push(const Action& task) override;

    std::optional<Action> try_pop() override;

    size_t size();

    bool empty();

  private:
    std::queue<Action> _queue;
    std::mutex _mutex;
};

}  // namespace dispatcher::queue
