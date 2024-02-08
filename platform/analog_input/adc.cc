#include "platform/analog_input/adc.hh"

#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>

#include <gsl/util>

namespace fcp::platform {

ADC::BufferType ADC::adc3_dma_buffer{};

ADC::ADC(ADC_HandleTypeDef* const hal_adc_ptr) : hal_adc_ptr_{hal_adc_ptr} {
  Ensures(HAL_ADC_Start_DMA(hal_adc_ptr_, buffer_ptr(), channel_count()) ==
          HAL_StatusTypeDef::HAL_OK);
}

ADC::~ADC() {
  Ensures(HAL_ADC_Stop_DMA(hal_adc_ptr_) == HAL_StatusTypeDef::HAL_OK);
}

AnalogInput ADC::make_analog_input(const size_t rank) const {
  Expects(rank < channel_count());
  if (hal_adc_ptr_->Instance == ADC3) {
    return AnalogInput(&adc3_dma_buffer[rank]);
  }
  Ensures(false);
  return AnalogInput(nullptr);
}

size_t ADC::channel_count() const { return hal_adc_ptr_->Init.NbrOfConversion; }

uint32_t* ADC::buffer_ptr() const {
  if (hal_adc_ptr_->Instance == ADC3) {
    return const_cast<uint32_t*>(adc3_dma_buffer.data());
  }
  Ensures(false);
  return nullptr;
}

}  // namespace fcp::platform
