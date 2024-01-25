#pragma once

#include <array>
#include <limits>

#include "platform/digital_input/digital_input.hh"

namespace fcp::platform {

/**
 * @brief Counts low frequency pulses with external interrupts.
 *
 * Intended to identify activities with buttons or switches.
 *
 * @pre External interrupt (EXTI) should be enabled for this pin.
 */
class PulseCounter : DigitalInput {
 public:
  using PinBitmaskType = uint16_t;
  static constexpr uint32_t MIN_PULSE_WIDTH_MS = 10U;
  static constexpr uint32_t MAX_PULSE_GAP_MS = 200U;

  PulseCounter(const PulseCounter&) = delete;
  PulseCounter& operator=(const PulseCounter&) = delete;
  PulseCounter(PulseCounter&&) = delete;
  PulseCounter& operator=(PulseCounter&&) = delete;

  PulseCounter(GPIO_TypeDef* const port_ptr, const PinBitmaskType pin_bitmask);
  ~PulseCounter();

  struct State {
    /**
     * @brief Total number of pulses.
     *
     * A pulse is identified as a rising edge followed by a falling edge. A
     * pulse is only counted
     * 1. when it completes, i.e. when the falling edge is detected; and
     * 2. if its width is larger than @p MIN_PULSE_WIDTH_MS to debounce.
     *
     * This value is unique for each pulse thus can be used as an ID.
     */
    uint32_t total_pulse_count;
    /**
     * @brief Number of ongoing consecutive pulses.
     *
     * If the gap between two pulses is no more than @p MAX_PULSE_GAP_MS, these
     * two pulses are considered consecutive.
     */
    uint32_t consecutive_pulse_count;
    /**
     * @brief Pulse width of an active pulse.
     *
     * The time elapsed since the rising edge of an active pulse before
     * detecting its falling edge.
     *
     * This value is set to zero if smaller than @p MIN_PULSE_WIDTH_MS.
     */
    uint32_t current_width_ms;
  };

  /**
   * @brief Gets the current state of the pulse counter.
   *
   * @return State @see @c State.
   */
  State state() const;

  static void on_edge(PinBitmaskType pin_bitmask);

 private:
  static std::array<PulseCounter*, std::numeric_limits<PinBitmaskType>::digits>
      instances;

  uint32_t consecutive_pulse_count() const;
  uint32_t current_width_ms() const;
  void on_edge_impl();

  int pin_bit_position_;

  enum class Edge {
    FALLING,
    RISING,
  };
  Edge previous_edge = Edge::FALLING;
  uint32_t previous_edge_tick = 0;
  uint32_t previous_pulse_width_tick = 0;
  uint32_t previous_pulse_end_tick = 0;
  uint32_t consecutive_pulse_count_ = 0;
  uint32_t total_pulse_count_ = 0;
};

}  // namespace fcp::platform
