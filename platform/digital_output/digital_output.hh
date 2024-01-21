#pragma once

#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>

namespace fcp::platform {

/**
 * @brief A peripheral controlled by a single digital output, e.g. an LED.
 *
 * This is an identity type, move only.
 *
 * @tparam is_active_low True if the peripheral is logically on when the
 * output pin is set to digital low.
 */
template <bool is_active_low = false>
class DigitalOutput {
 public:
  static constexpr bool IS_ACTIVE_LOW = is_active_low;
  DigitalOutput(const DigitalOutput&) = delete;
  DigitalOutput& operator=(const DigitalOutput&) = delete;
  DigitalOutput(DigitalOutput&&) = default;
  DigitalOutput& operator=(DigitalOutput&&) = default;

  /**
   * @brief Construct a new Digital Output object
   *
   * Usage:
   * @code {.cpp}
   *  const auto led_green = DigitalOutput{GPIOB, GPIO_PIN_0};
   * @endcode
   *
   * @param port Address of the GPIO port (GPIOx).
   * @param pin Bitmask of the pin (GPIO_PIN).
   */
  DigitalOutput(GPIO_TypeDef* const port, const uint16_t pin);

  /**
   * @brief Turns on the peripheral.
   */
  void on() const;

  /**
   * @brief Turns off the peripheral.
   */
  void off() const;

  /**
   * @brief Toggles the logical state of the peripheral.
   */
  void toggle() const;

  /**
   * @brief Gets the logical state of the peripheral.
   *
   * @return true The peripheral is on.
   * @return false The peripheral is off.
   */
  bool is_on() const;

 private:
  GPIO_TypeDef* const port_;
  const uint16_t pin_;
};

}  // namespace fcp::platform

#include "platform/digital_output/digital_output.inl"
