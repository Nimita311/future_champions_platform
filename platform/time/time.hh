#pragma once

#include <FreeRTOS.h>
#include <task.h>

#include <algorithm>
#include <limits>
#include <type_traits>

namespace fcp::platform {

// 32 bit timestamp
using TimestampType = uint32_t;
static_assert(std::is_same_v<TimestampType, TickType_t>);
static_assert(std::numeric_limits<TimestampType>::max() == portMAX_DELAY);

// 1 tick = 1 ms
static_assert(configTICK_RATE_HZ == 1000);
inline constexpr TimestampType ms_to_tick(const TimestampType ms) { return ms; }
inline constexpr TimestampType tick_to_ms(const TimestampType tick) {
  return tick;
}

/**
 * @brief Delays current task for @p ms milliseconds.
 *
 * @attention Should only be used when OS scheduler is enabled which is
 * generally true in application code.
 * @param ms Delay in milliseconds.
 */
inline void delay_ms(const TimestampType ms) { vTaskDelay(ms_to_tick(ms)); }

/**
 * @brief Gets current timestamp.
 *
 * Timestamp is the number of milliseconds elapsed since OS scheduler startup.
 *
 * @return TimestampType Current timestamp in milliseconds.
 */
inline TimestampType timestamp_ms() { return tick_to_ms(xTaskGetTickCount()); }

/**
 * @brief Gets the next timestamp of a periodic schedule following a reference
 * timestamp.
 *
 * @param period_ms Period of the schedule in milliseconds.
 * @param phase_ms Phase of the schedule in milliseconds.
 * @param timestamp_ms Reference timestamp in milliseconds.
 * @return constexpr TimestampType Next timestamp of the schedule. May overflow.
 */
inline constexpr TimestampType next_periodic_timestamp_ms(
    const TimestampType period_ms, const TimestampType phase_ms,
    const TimestampType timestamp_ms) {
  const auto quo = timestamp_ms / period_ms;
  const auto rem = timestamp_ms % period_ms;
  const auto min_phase = phase_ms % period_ms;
  return period_ms * ((min_phase > rem) ? quo : (quo + 1)) + min_phase;
}
inline TimestampType next_periodic_timestamp_ms(const TimestampType period_ms,
                                                const TimestampType phase_ms) {
  return next_periodic_timestamp_ms(period_ms, phase_ms, timestamp_ms());
}

/**
 * @brief Gets the previous timestamp of a periodic schedule following a
 * reference timestamp.
 *
 * @param period_ms Period of the schedule in milliseconds.
 * @param phase_ms Phase of the schedule in milliseconds.
 * @param timestamp_ms Reference timestamp in milliseconds.
 * @return constexpr TimestampType Previous timestamp of the schedule. May
 * underflow.
 */
inline constexpr TimestampType previous_periodic_timestamp_ms(
    const TimestampType period_ms, const TimestampType phase_ms,
    const TimestampType timestamp_ms) {
  return next_periodic_timestamp_ms(period_ms, phase_ms, timestamp_ms) -
         period_ms;
}
inline TimestampType previous_periodic_timestamp_ms(
    const TimestampType period_ms, const TimestampType phase_ms) {
  return previous_periodic_timestamp_ms(period_ms, phase_ms, timestamp_ms());
}

}  // namespace fcp::platform
