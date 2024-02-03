#include <stm32h7xx_hal.h>

namespace fcp::platform {

template <bool is_active_low>
DigitalOutput<is_active_low>::DigitalOutput(GPIO_TypeDef* const port,
                                            const uint16_t pin)
    : port_{port}, pin_{pin} {}

template <bool is_active_low>
void DigitalOutput<is_active_low>::on() const {
  HAL_GPIO_WritePin(port_, pin_, is_active_low ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

template <bool is_active_low>
void DigitalOutput<is_active_low>::off() const {
  HAL_GPIO_WritePin(port_, pin_, is_active_low ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

template <bool is_active_low>
void DigitalOutput<is_active_low>::toggle() const {
  HAL_GPIO_TogglePin(port_, pin_);
}

template <bool is_active_low>
bool DigitalOutput<is_active_low>::is_on() const {
  return static_cast<bool>(port_->ODR & pin_) != is_active_low;
}

}  // namespace fcp::platform
