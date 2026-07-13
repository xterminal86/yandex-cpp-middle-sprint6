#include "queue/unbounded_queue.hpp"
#include "logger.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

void UnboundedQueue::push(std::function<void()> task)
{
  std::lock_guard lock(_mutex);

  //Logger::Get().Log("    UnboundedQueue::Push()");

  if (not _enabled)
  {
    return;
  }

  _queue.push(task);
}

// =============================================================================

std::optional<Action> UnboundedQueue::try_pop()
{
  std::lock_guard lock(_mutex);

  //Logger::Get().Log("    UnboundedQueue::TryPop()");

  if (_queue.empty())
  {
    return std::nullopt;
  }

  Action action = _queue.front();
  _queue.pop();

  return action;
}

// =============================================================================

size_t UnboundedQueue::size()
{
  std::lock_guard lock(_mutex);
  return _queue.size();
}

// =============================================================================

bool UnboundedQueue::empty()
{
  std::lock_guard lock(_mutex);
  return _queue.empty();
}

} // namespace dispatcher::queue