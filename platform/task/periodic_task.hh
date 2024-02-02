#pragma once

#include "platform/task/task.hh"
#include "platform/time/time.hh"

namespace fcp::platform {

struct PeriodicTaskConfig : public TaskConfig {
  TimestampType period_ms;
  TimestampType phase_ms;
};

template <class CallableType>
class PeriodicTask : public Task<CallableType> {
 public:
  PeriodicTask(const PeriodicTask&) = delete;
  PeriodicTask& operator=(const PeriodicTask&) = delete;
  PeriodicTask(PeriodicTask&&) = delete;
  PeriodicTask& operator=(PeriodicTask&&) = delete;

  PeriodicTask(const PeriodicTaskConfig& config);
  PeriodicTask(const PeriodicTaskConfig& config, CallableType&& callable_);

 private:
  const TimestampType period_ms_;
  const TimestampType phase_ms_;
  TimestampType previous_wake_time_tick_;

  static void task_wrapper(void* instance_ptr);
};

}  // namespace fcp::platform

#include "platform/task/periodic_task.inl"
