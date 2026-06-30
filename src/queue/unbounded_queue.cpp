#include "queue/unbounded_queue.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

void UnboundedQueue::push(std::function<void()> task)
{
  std::lock_guard lock(_mutex);
  _queue.push(task);
}

// =============================================================================

std::optional<UnboundedQueue::Action> UnboundedQueue::try_pop()
{
  std::lock_guard lock(_mutex);
  if (_queue.empty())
  {
    return std::nullopt;
  }

  Action action = _queue.front();
  _queue.pop();

  return action;
}

} // namespace dispatcher::queue