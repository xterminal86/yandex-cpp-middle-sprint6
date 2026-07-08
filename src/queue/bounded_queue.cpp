#include "queue/bounded_queue.hpp"

namespace dispatcher::queue {

BoundedQueue::BoundedQueue(size_t capacity)
  : _capacity(capacity) {}

// =============================================================================

void BoundedQueue::push(Action task)
{
  std::unique_lock lock(_mutex);

  //
  // Equivalent to:
  // while (!pred) wait(lock);
  //
  // So if condition is not satisfied, execution blocks (in our case if queue is
  // full). wait() can also unblock if thread was notified or spurious wakeup
  // occured. In latter case pred will be re-checked to make sure it was the
  // right "wake up call".
  //
  _blockIfFull.wait(
    lock,
    [this]()
    {
      return _queue.size() < _capacity;
    }
  );

  _queue.push(std::move(task));

  _blockIfEmpty.notify_one();
}

// =============================================================================

std::optional<Action> BoundedQueue::try_pop()
{
  std::unique_lock lock(_mutex);

  _blockIfEmpty.wait(
    lock,
    [this]()
    {
      return not _queue.empty();
    }
  );

  if (_queue.empty())
  {
    return std::nullopt;
  }

  Action result = std::move(_queue.front());

  _queue.pop();

  _blockIfFull.notify_one();

  return result;
}

} // namespace dispatcher::queue