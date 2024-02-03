#include "autonomy/motor/motor.hh"

#include <algorithm>

namespace fcp::aut {

Motor::Motor(const platform::PWMOutput& drive,
             const platform::PWMOutput& reverse)
    : drive_ref_{drive}, reverse_ref_{reverse} {}

void Motor::drive(const float power) const {
  const auto clamped_power =
      std::clamp(power, POWER_RANGE_MIN, POWER_RANGE_MAX);
  // forward
  if (clamped_power > 0.0f) {
    drive_ref_.get().set_duty(clamped_power);
    reverse_ref_.get().set_duty(POWER_NEUTRAL);
    return;
  }
  // backward
  drive_ref_.get().set_duty(POWER_NEUTRAL);
  reverse_ref_.get().set_duty(-clamped_power);
}

void Motor::brake(const float power) const {
  const auto clamped_power =
      std::clamp(power, BRAKE_RANGE_MIN, BRAKE_RANGE_MAX);
  drive_ref_.get().set_duty(clamped_power);
  reverse_ref_.get().set_duty(clamped_power);
}

void Motor::neutral() const {
  drive_ref_.get().set_duty(POWER_NEUTRAL);
  reverse_ref_.get().set_duty(POWER_NEUTRAL);
}

}  // namespace fcp::aut
