#include <platform/digital_input/pulse_counter.hh>

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin_bitmask) {
  fcp::platform::PulseCounter::on_edge(pin_bitmask);
}
