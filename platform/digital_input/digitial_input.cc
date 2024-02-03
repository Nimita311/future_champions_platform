#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>

#include "platform/digital_input/digital_input.hh"

namespace fcp::platform {

DigitalInput::DigitalInput(GPIO_TypeDef* const port_ptr,
                           const uint16_t pin_bitmask)
    : port_ptr_{port_ptr}, pin_bitmask_{pin_bitmask} {}

bool DigitalInput::is_active() const {
  return HAL_GPIO_ReadPin(port_ptr_, pin_bitmask_) ==
         GPIO_PinState::GPIO_PIN_SET;
}

}  // namespace fcp::platform
