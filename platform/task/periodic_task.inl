#include <FreeRTOS.h>
#include <task.h>

#include <gsl/util>

#include "platform/task/periodic_task.hh"
#include "platform/time/time.hh"

namespace fcp::platform {

template <class CallableType>
PeriodicTask<CallableType>::PeriodicTask(const PeriodicTaskConfig& config)
    : period_ms_{config.period_ms}, phase_ms_{config.phase_ms} {
  Expects(config.period_ms != 0);
  Task<CallableType>::create_os_task(config, task_wrapper);
}

template <class CallableType>
PeriodicTask<CallableType>::PeriodicTask(const PeriodicTaskConfig& config,
                                         CallableType&& callable)
    : Task<CallableType>{std::move(callable)},
      period_ms_{config.period_ms},
      phase_ms_{config.phase_ms} {
  Expects(config.period_ms != 0);
  Task<CallableType>::create_os_task(config, task_wrapper);
}

template <class CallableType>
void PeriodicTask<CallableType>::task_wrapper(void* instance_ptr) {
  auto& instance = *static_cast<PeriodicTask<CallableType>*>(instance_ptr);
  instance.previous_wake_time_tick_ = ms_to_tick(
      previous_periodic_timestamp_ms(instance.period_ms_, instance.phase_ms_));
  while (true) {
    vTaskDelayUntil(&(instance.previous_wake_time_tick_),
                    ms_to_tick(instance.period_ms_));
    instance.callable_();
  }
  // Unreachable
  instance.task_ptr_ = nullptr;
  vTaskDelete(nullptr);
  Ensures(false);
}

}  // namespace fcp::platform
