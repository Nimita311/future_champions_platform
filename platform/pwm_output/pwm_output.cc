#include "platform/pwm_output/pwm_output.hh"

#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>

#include <algorithm>
#include <gsl/util>
#include <limits>

namespace fcp::platform {

PWMOutput::PWMOutput(TIM_HandleTypeDef* const timer_ptr,
                     const uint32_t channel_bitmask)
    : timer_ptr_{timer_ptr}, channel_bitmask_{channel_bitmask} {
  Expects(timer_ptr_->Instance->ARR < std::numeric_limits<uint32_t>::max());
  Ensures(HAL_TIM_PWM_Start(timer_ptr_, channel_bitmask_) == HAL_OK);
  if (channel_bitmask_ == TIM_CHANNEL_1) {
    ccr_ptr_ = &(timer_ptr_->Instance->CCR1);
    return;
  }
  if (channel_bitmask_ == TIM_CHANNEL_2) {
    ccr_ptr_ = &(timer_ptr_->Instance->CCR2);
    return;
  }
  if (channel_bitmask_ == TIM_CHANNEL_3) {
    ccr_ptr_ = &(timer_ptr_->Instance->CCR3);
    return;
  }
  if (channel_bitmask_ == TIM_CHANNEL_4) {
    ccr_ptr_ = &(timer_ptr_->Instance->CCR4);
    return;
  }
  if (channel_bitmask_ == TIM_CHANNEL_5) {
    ccr_ptr_ = &(timer_ptr_->Instance->CCR5);
    return;
  }
  if (channel_bitmask_ == TIM_CHANNEL_6) {
    ccr_ptr_ = &(timer_ptr_->Instance->CCR6);
    return;
  }
  Ensures(ccr_ptr_ != nullptr);
}

PWMOutput::~PWMOutput() {
  Ensures(HAL_TIM_PWM_Stop(timer_ptr_, channel_bitmask_) == HAL_OK);
}

float PWMOutput::duty() const {
  const auto raw_duty = static_cast<float>(*ccr_ptr_) /
                        (static_cast<float>(timer_ptr_->Instance->ARR) + 1.0f);
  return std::clamp(raw_duty, 0.0f, 1.0f);
}

void PWMOutput::set_duty(const float duty) const {
  const auto clamped_duty = std::clamp(duty, 0.0f, 1.0f);
  *ccr_ptr_ = static_cast<uint32_t>(
      (static_cast<float>(timer_ptr_->Instance->ARR) + 1.0f) * clamped_duty);
}

}  // namespace fcp::platform
