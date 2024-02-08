#pragma once

#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>

#include <gsl/pointers>

#include "platform/analog_input/analog_input.hh"

namespace fcp::platform {

class ADC {
 public:
  ADC(const ADC&) = delete;
  ADC& operator=(const ADC&) = delete;
  ADC(ADC&&) = delete;
  ADC& operator=(ADC&&) = delete;

  ADC(ADC_HandleTypeDef* const hal_adc_ptr);
  ~ADC();

  /**
   * @brief Produces an analog input channel of this ADC.
   *
   * @param rank Rank in the sequencer.
   * @return AnalogInput Analog input channel of a target rank.
   */
  AnalogInput make_analog_input(const size_t rank) const;

 private:
  static constexpr size_t ADC_CHANNEL_COUNT_MAX = 19;
  using BufferType =
      std::array<AnalogInput::BufferValueType, ADC_CHANNEL_COUNT_MAX>;
  static BufferType adc3_dma_buffer;

  size_t channel_count() const;
  uint32_t* buffer_ptr() const;

  const gsl::not_null<ADC_HandleTypeDef*> hal_adc_ptr_;
};

}  // namespace fcp::platform
