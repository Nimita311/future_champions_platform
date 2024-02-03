#include "platform/digital_input/pulse_counter.hh"

#include <FreeRTOS.h>
#include <task.h>

#include <array>
#include <gsl/util>

namespace fcp::platform {

std::array<PulseCounter*,
           std::numeric_limits<PulseCounter::PinBitmaskType>::digits>
    PulseCounter::instances{};

PulseCounter::PulseCounter(GPIO_TypeDef* const port_ptr,
                           const PulseCounter::PinBitmaskType pin_bitmask)
    : DigitalInput{port_ptr, pin_bitmask} {
  auto bits_shifted = size_t{0};
  for (auto mask = pin_bitmask; mask != 0; mask >>= 1) {
    bits_shifted++;
  }
  Expects(bits_shifted != 0);
  pin_bit_position_ = bits_shifted - 1;
  PulseCounter::instances[pin_bit_position_] = this;
}

PulseCounter::~PulseCounter() {
  if (instances[pin_bit_position_] == this) {
    instances[pin_bit_position_] = nullptr;
  }
}

PulseCounter::State PulseCounter::state() const {
  return State{
      .total_pulse_count = total_pulse_count_,
      .consecutive_pulse_count = consecutive_pulse_count(),
      .current_width_ms = current_width_ms(),
  };
}

uint32_t PulseCounter::consecutive_pulse_count() const {
  if (previous_edge == Edge::RISING) {
    return consecutive_pulse_count_;
  }
  const auto gap_tick = xTaskGetTickCount() - previous_edge_tick;
  if (gap_tick > MAX_PULSE_GAP_MS) {
    return 0;
  }
  return consecutive_pulse_count_;
}

uint32_t PulseCounter::current_width_ms() const {
  if (previous_edge == Edge::RISING) {
    const auto width_tick = xTaskGetTickCount() - previous_edge_tick;
    if (width_tick < MIN_PULSE_WIDTH_MS) {
      return 0;
    }
    return width_tick;
  }
  return 0;
}

void PulseCounter::on_edge_impl() {
  // Skip consecutive same edge. Normally it should not happen.
  const auto current_edge = is_active() ? Edge::RISING : Edge::FALLING;
  if (previous_edge == current_edge) {
    return;
  }

  const auto current_edge_tick = xTaskGetTickCount();
  if (current_edge == Edge::RISING) {
    const auto gap_tick = current_edge_tick - previous_edge_tick;
    if (gap_tick <= MAX_PULSE_GAP_MS) {
      // Start a consecutive pulse
      previous_edge = Edge::RISING;
      previous_edge_tick = current_edge_tick;
    } else {
      // Break consecutive pulse count
      consecutive_pulse_count_ = 0;
      previous_edge = Edge::RISING;
      previous_edge_tick = current_edge_tick;
    }
    return;
  }

  const auto width_tick = current_edge_tick - previous_edge_tick;
  if (width_tick < MIN_PULSE_WIDTH_MS) {
    // Debounce, ignore current pulse
    previous_edge = Edge::FALLING;
    previous_edge_tick = previous_pulse_end_tick;
  } else {
    // Commit current pulse
    consecutive_pulse_count_++;
    total_pulse_count_++;
    previous_edge = Edge::FALLING;
    previous_edge_tick = current_edge_tick;
    previous_pulse_end_tick = current_edge_tick;
  }
}

void PulseCounter::on_edge(PulseCounter::PinBitmaskType pin_bitmask) {
  // Get pin bit position
  auto bits_shifted = size_t{0};
  for (auto mask = pin_bitmask; mask != 0; mask >>= 1) {
    bits_shifted++;
  }
  Expects(bits_shifted != 0);
  Expects(bits_shifted <= PulseCounter::instances.size());

  // Call handler for that pin
  const auto instance_ptr = PulseCounter::instances[bits_shifted - 1];
  // The handler might be nullptr if the interrupt happens before the pulse
  // counter object is created
  if (instance_ptr != nullptr) {
    instance_ptr->on_edge_impl();
  }
}

}  // namespace fcp::platform
