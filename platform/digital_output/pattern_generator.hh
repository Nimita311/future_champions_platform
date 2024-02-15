#pragma once

#include <functional>
#include <gsl/span>
#include <string_view>

#include "platform/digital_output/digital_output.hh"

namespace fcp::platform {

template <bool is_active_low = false>
class PulseGenerator : public DigitalOutput<is_active_low> {
 public:
  static constexpr uint32_t DEFAULT_PULSE_WIDTH_MS{50};

  PulseGenerator(const PulseGenerator&) = delete;
  PulseGenerator& operator=(const PulseGenerator&) = delete;
  PulseGenerator(PulseGenerator&&) = default;
  PulseGenerator& operator=(PulseGenerator&&) = default;

  /**
   * @see Same as DigitalOutput::DigitalOutput()
   */
  PulseGenerator(GPIO_TypeDef* const port, const uint16_t pin);

  /**
   * @brief
   *
   * @param width_ms
   */
  bool pulse(const uint32_t width_ms = DEFAULT_DIT_WIDTH_MS);

  /**
   * @brief
   *
   * @param widths
   */
  bool send_pulses(const gsl::span<uint32_t> widths_ms);

  /**
   * @brief
   *
   * @param message
   */
  bool send_morse(const std::string_view message);

  /**
   * @brief Checks if there is an ongoing flashing pattern.
   *
   * @return true
   * @return false
   */
  bool is_busy();

  /**
   * @brief Stops ongoing flashing pattern.
   *
   * It stops the pattern immediately and does not try to reset PulseGenerator
   * state.
   */
  void stop();

  /**
   * @brief Blocks current task until the ongoing flashing patten finishes.
   *
   */
  void wait_until_sent();

 private:
  static void on_timer();
};

}  // namespace fcp::platform
