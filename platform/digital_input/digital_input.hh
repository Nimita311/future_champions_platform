#pragma once

#include <stm32h743xx.h>

#include <gsl/pointers>

namespace fcp::platform {

/**
 * @brief A sensor providing binary digital input, e.g. a button or a switch.
 *
 * This is an identity type, move only.
 *
 */
class DigitalInput {
 public:
  DigitalInput(const DigitalInput&) = delete;
  DigitalInput& operator=(const DigitalInput&) = delete;
  DigitalInput(DigitalInput&&) = default;
  DigitalInput& operator=(DigitalInput&&) = default;

  /**
   * @brief Construct a new Digital Input object
   *
   * Usage:
   * @code {.cpp}
   * const auto button = fcp::platform::DigitalInput{GPIOC, GPIO_PIN_13};
   * @endcode
   *
   * @param port_ptr Pointer to the GPIO port (GPIOx).
   * @param pin_bitmask Bitmask of the input pin (GPIO_PIN_x).
   */
  DigitalInput(GPIO_TypeDef* const port_ptr, const uint16_t pin_bitmask);

  /**
   * @brief Gets sensor logical input.
   *
   * Assuming the sensor is active high, i.e. reads high when active.
   *
   * @return true If pin reads high.
   * @return false If pin reads low.
   */
  bool is_active() const;

 private:
  const gsl::not_null<GPIO_TypeDef*> port_ptr_;
  const uint16_t pin_bitmask_;
};

}  // namespace fcp::platform
