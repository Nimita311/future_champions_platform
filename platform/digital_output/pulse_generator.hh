#pragma once

#include <FreeRTOS.h>
#include <timers.h>

#include <gsl/pointers>

#include "platform/digital_output/digital_output.hh"

namespace fcp::platform {

/**
 * @brief Asynchronous pulse generator.
 *
 * Used to generate one shot pulses at a low frequency. Time resolution is
 * bounded by OS tick.
 *
 * A Typical use case is transmission activity indicator LED.
 *
 * @attention Not ISR safe.
 * @tparam is_active_low @see DigitalOutput::IS_ACTIVE_LOW
 */
template <bool is_active_low = false>
class PulseGenerator : public DigitalOutput<is_active_low> {
 public:
  static constexpr uint32_t DEFAULT_PULSE_WIDTH_MS{50};

  PulseGenerator(const PulseGenerator&) = delete;
  PulseGenerator& operator=(const PulseGenerator&) = delete;

  PulseGenerator(PulseGenerator&&);
  PulseGenerator& operator=(PulseGenerator&&);
  ~PulseGenerator();

  /**
   * @see Same as DigitalOutput::DigitalOutput()
   */
  PulseGenerator(GPIO_TypeDef* const port, const uint16_t pin);

  /**
   * @brief Sends a pulse asynchronously.
   *
   * A pulse consists of two signal edges made by toggling the digital output.
   * The duration between two edges is known as the width of the pulse.
   *
   * If there is not an active pulse, the first edge will be generated
   * immediately.
   *
   * If there is an active pulse, it will be merged the requested pulse so
   * that the resulting second edge will be the latest of the two. That is, if
   * the requested pulse would finish after the active pulse, the width will be
   * extended. If the requested pulse would finish before the active pulse, this
   * request has no effect.
   *
   * To avoid pulses being merged, send a pulse if there is no active pulse:
   * @code {.cpp}
   * if (!pulse_generator.is_active()) {
   *   pulse_generator.pulse();
   * }
   * @endcode
   *
   * @param width_ms Width of the pulse in milliseconds.
   */
  void pulse(const uint32_t width_ms = DEFAULT_PULSE_WIDTH_MS) const;

  /**
   * @brief Checks for an active pulse.
   *
   * A pulse is active when the second edge has not yet been generated.
   *
   * @return true If there is an active pulse.
   * @return false If there is no active pulse.
   */
  bool is_active() const;

  /**
   * @brief Completes any active pulse.
   *
   * If there is an active pulse, it generates the second edge immediately.
   * If there is no active pulse, it has no effect.
   */
  void stop() const;

 private:
  gsl::owner<TimerHandle_t> timer_ptr_;
  static void on_timer(TimerHandle_t timer_ptr);
};

}  // namespace fcp::platform

#include "platform/digital_output/pulse_generator.inl"
