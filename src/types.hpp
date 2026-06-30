#pragma once

#include <memory>
#include <typeinfo>

namespace dispatcher {

enum class TaskPriority { High, Normal };

using Action = std::function<void()>;

inline bool IsActionEmpty(const Action& a)
{
  return a.target_type() == typeid(void);
}

}  // namespace dispatcher