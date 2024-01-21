#include <FreeRTOS.h>
#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>
#include <task.h>

#include "platform/digital_output/digital_output.hh"

void halt() {
  while (true) {
    // halt
  }
}

/**
 * @brief LED demo.
 */
extern "C" __attribute__((weak)) void main_task(const void*) {
  using fcp::platform::DigitalOutput;
  const auto led_green = DigitalOutput{GPIOB, GPIO_PIN_0};
  const auto led_yellow = DigitalOutput{GPIOE, GPIO_PIN_1};
  const auto led_red = DigitalOutput<true>{GPIOB, GPIO_PIN_14};
  led_green.off();
  led_yellow.off();
  led_red.on();

  while (true) {
    led_green.on();
    if (!led_green.is_on()) {
      halt();
    }
    vTaskDelay(500U);
    led_green.off();
    if (led_green.is_on()) {
      halt();
    }
    led_yellow.on();
    vTaskDelay(500U);
    led_yellow.off();
    led_red.off();
    if (led_red.is_on()) {
      halt();
    }
    vTaskDelay(500U);
    led_red.on();
    if (!led_red.is_on()) {
      halt();
    }

    led_green.toggle();
    led_yellow.toggle();
    led_red.toggle();
    vTaskDelay(500U);
    led_green.toggle();
    led_yellow.toggle();
    led_red.toggle();
  }
}
