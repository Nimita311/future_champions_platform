#pragma once

#include <stm32h7xx_hal.h>

#include <gsl/pointers>

namespace fcp::platform {

class PWMOutput {
 public:
  PWMOutput(const PWMOutput&) = delete;
  PWMOutput& operator=(const PWMOutput&) = delete;
  PWMOutput(PWMOutput&&) = default;
  PWMOutput& operator=(PWMOutput&&) = default;

  PWMOutput(TIM_HandleTypeDef* const timer_ptr, const uint32_t channel_bitmask);
  ~PWMOutput();

  /**
   * @brief Gets current PWM duty cycle.
   *
   * @return float Current duty cycle in range [0.0f, 1.0f].
   */
  float duty() const;

  /**
   * @brief Sets PWM duty cycle.
   *
   * @param duty Clamped to [0.0f, 1.0f] which maps to [0%, 100%] duty cycle.
   */
  void set_duty(const float duty) const;

 private:
  const gsl::not_null<TIM_HandleTypeDef*> timer_ptr_;
  uint32_t channel_bitmask_;
  volatile uint32_t* ccr_ptr_;
};

}  // namespace fcp::platform
