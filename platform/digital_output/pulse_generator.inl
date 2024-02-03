#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <gsl/assert>

namespace fcp::platform {

template <bool is_active_low>
PulseGenerator<is_active_low>::PulseGenerator(GPIO_TypeDef* const port,
                                              const uint16_t pin)
    : DigitalOutput<is_active_low>{port, pin},
      timer_ptr_{xTimerCreate(
          "pulse_generator_timer",                // pcTimerName
          pdMS_TO_TICKS(DEFAULT_PULSE_WIDTH_MS),  // xTimerPeriod
          pdFALSE,                                // uxAutoReload
          this,                                   // pvTimerID
          on_timer                                // pxCallbackFunction
          )} {}

template <bool is_active_low>
PulseGenerator<is_active_low>::PulseGenerator(PulseGenerator&& rhs)
    : DigitalOutput<is_active_low>{std::move(
          static_cast<DigitalOutput<is_active_low>>(rhs))},
      timer_ptr_{rhs.timer_ptr_} {
  rhs.timer_ptr_ = nullptr;
}

template <bool is_active_low>
PulseGenerator<is_active_low>& PulseGenerator<is_active_low>::operator=(
    PulseGenerator&& rhs) {
  Ensures(xTimerDelete(timer_ptr_, 0));
  timer_ptr_ = rhs.timer_ptr_;
  rhs.timer_ptr_ = nullptr;
  return *this;
}

template <bool is_active_low>
PulseGenerator<is_active_low>::~PulseGenerator() {
  if (timer_ptr_ == nullptr) {
    return;
  }
  Ensures(xTimerDelete(timer_ptr_, 0));
}

template <bool is_active_low>
void PulseGenerator<is_active_low>::pulse(const uint32_t width_ms) const {
  const auto period_tick = pdMS_TO_TICKS(width_ms);
  // No active pulse
  if (!is_active()) {
    this->toggle();
    Ensures(xTimerChangePeriod(timer_ptr_, period_tick, 0));
    Ensures(xTimerStart(timer_ptr_, 0));
    return;
  }
  // With active pulse
  const auto active_edge_tick = xTimerGetExpiryTime(timer_ptr_);
  const auto requested_edge_tick =
      xTaskGetTickCount() + pdMS_TO_TICKS(width_ms);
  if (requested_edge_tick > active_edge_tick) {
    Ensures(xTimerChangePeriod(timer_ptr_, period_tick, 0));
  }
}

template <bool is_active_low>
bool PulseGenerator<is_active_low>::is_active() const {
  return xTimerIsTimerActive(timer_ptr_);
}

template <bool is_active_low>
void PulseGenerator<is_active_low>::stop() const {
  if (!is_active()) {
    return;
  }
  Ensures(xTimerStop(timer_ptr_, 0));
  this->toggle();
}

template <bool is_active_low>
void PulseGenerator<is_active_low>::on_timer(TimerHandle_t timer_ptr) {
  const auto pulse_generator_ptr_ =
      static_cast<PulseGenerator<is_active_low>*>(pvTimerGetTimerID(timer_ptr));
  pulse_generator_ptr_->toggle();
}

}  // namespace fcp::platform
