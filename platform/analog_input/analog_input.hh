#pragma once

#include <core_cm7.h>

#include <gsl/pointers>
#include <gsl/span>
#include <limits>

namespace fcp::platform {

class AnalogInput {
 public:
  using ValueType = uint32_t;
  using BufferValueType = const volatile ValueType;

  AnalogInput(const AnalogInput&) = delete;
  AnalogInput& operator=(const AnalogInput&) = delete;
  AnalogInput(AnalogInput&&) = default;
  AnalogInput& operator=(AnalogInput&&) = default;

  AnalogInput(BufferValueType* const buffer_value_ptr)
      : buffer_value_ptr_{buffer_value_ptr} {}

  /**
   * @brief Gets raw ADC measurement value.
   *
   * @return ValueType Raw ADC measurement.
   */
  inline ValueType raw_value() const {
    // Invalidate dcache before reading as DMA has altered the memory
    SCB_InvalidateDCache_by_Addr(
        static_cast<void*>(const_cast<ValueType*>(buffer_value_ptr_.get())),
        sizeof(BufferValueType));
    return *buffer_value_ptr_;
  }

  /**
   * @brief Gets analog input value.
   *
   * @pre ADC should be configured with oversampling and bit shifts so that the
   * dynamic range is 32 bits.
   * @return float Analog input value as the proportion of reference voltage. In
   * range [0.0f, 1.0f].
   */
  inline float value() const {
    return static_cast<float>(raw_value()) / VALUE_MAX;
  }

 private:
  static constexpr float VALUE_MAX =
      static_cast<float>(std::numeric_limits<ValueType>::max());

  const gsl::not_null<BufferValueType*> buffer_value_ptr_;
};

}  // namespace fcp::platform
