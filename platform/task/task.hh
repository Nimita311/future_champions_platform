#pragma once

#include <FreeRTOS.h>
#include <task.h>

#include <gsl/pointers>
#include <string_view>

namespace fcp::platform {

struct TaskConfig {
  std::string_view name;
  configSTACK_DEPTH_TYPE stack_depth;
  UBaseType_t priority;

  static constexpr UBaseType_t DEFAULT_PRIORITY = 3;
  static constexpr configSTACK_DEPTH_TYPE DEFAULT_STACK_DEPTH =
      configMINIMAL_STACK_SIZE;
};

template <class CallableType>
class Task {
 public:
  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;
  Task(Task&&) = delete;
  Task& operator=(Task&&) = delete;

  Task(const TaskConfig& config);
  Task(const TaskConfig& config, CallableType&& callable);
  ~Task();

 protected:
  CallableType callable_;
  gsl::owner<TaskHandle_t> task_ptr_;

  Task() = default;
  Task(CallableType&& callable);
  void create_os_task(const TaskConfig& config,
                      const TaskFunction_t task_wrapper_ptr);

  static void task_wrapper(void* instance_ptr);
};

}  // namespace fcp::platform

#include "platform/task/task.inl"
