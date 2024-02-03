#include "platform/robot/robot.hh"

#include <stm32h743xx.h>
#include <stm32h7xx_hal.h>

// Defined in main.c
extern TIM_HandleTypeDef htim3;

namespace fcp::platform {

Robot& Robot::instance() {
  static auto robot = Robot{};
  return robot;
}

Robot::Robot()
    : led_green{GPIOB, GPIO_PIN_0},
      led_yellow{GPIOE, GPIO_PIN_1},
      led_red{GPIOB, GPIO_PIN_14},
      button{GPIOC, GPIO_PIN_13},
      pwm_left_drive{&htim3, TIM_CHANNEL_1},
      pwm_left_reverse{&htim3, TIM_CHANNEL_2},
      pwm_right_drive{&htim3, TIM_CHANNEL_3},
      pwm_right_reverse{&htim3, TIM_CHANNEL_4},
      motor_left{pwm_left_drive, pwm_left_reverse},
      motor_right{pwm_right_drive, pwm_right_reverse},
      diff_drive{motor_left, motor_right} {}

}  // namespace fcp::platform
