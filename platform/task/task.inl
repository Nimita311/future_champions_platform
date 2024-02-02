#include <FreeRTOS.h>
#include <task.h>

#include <gsl/util>

#include "platform/task/task.hh"

namespace fcp::platform {

template <class CallableType>
Task<CallableType>::Task(const TaskConfig& config) : callable_{} {
  create_os_task(config, task_wrapper);
}

template <class CallableType>
Task<CallableType>::Task(const TaskConfig& config, CallableType&& callable)
    : callable_{std::move(callable)} {
  create_os_task(config, task_wrapper);
}

template <class CallableType>
Task<CallableType>::Task(CallableType&& callable)
    : callable_{std::move(callable)} {}

template <class CallableType>
Task<CallableType>::~Task() {
  if (task_ptr_ != nullptr) {
    vTaskDelete(task_ptr_);
  }
}

template <class CallableType>
void Task<CallableType>::create_os_task(const TaskConfig& config,
                                        const TaskFunction_t task_wrapper_ptr) {
  Expects(xTaskCreate(task_wrapper_ptr,          // pvTaskCode
                      config.name.data(),        // pcName
                      config.stack_depth,        // usStackDepth
                      static_cast<void*>(this),  // pvParameters
                      config.priority,           // uxPriority
                      &task_ptr_                 // pxCreatedTask
                      ) == pdPASS);
}

template <class CallableType>
void Task<CallableType>::task_wrapper(void* instance_ptr) {
  auto& instance = *static_cast<Task<CallableType>*>(instance_ptr);
  while (true) {
    instance.callable_();
  }
  // Unreachable
  instance.task_ptr_ = nullptr;
  vTaskDelete(nullptr);
  Ensures(false);
}

}  // namespace fcp::platform
