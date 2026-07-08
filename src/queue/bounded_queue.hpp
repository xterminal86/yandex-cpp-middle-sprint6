#pragma once
#include "queue/queue.hpp"
#include "types.hpp"

#include <queue>
#include <mutex>
#include <condition_variable>

namespace dispatcher::queue {

class BoundedQueue : public IQueue
{
  public:
    explicit BoundedQueue(size_t capacity);

    void push(Action task) override;

    std::optional<Action> try_pop() override;

  private:
    std::queue<Action> _queue;
    std::mutex _mutex;
    std::condition_variable _blockIfFull;
    std::condition_variable _blockIfEmpty;

    size_t _capacity;
};

}  // namespace dispatcher::queue