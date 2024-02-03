#include "autonomy/motor/diff_drive.hh"

#include <algorithm>

namespace fcp::aut {

DiffDrive::DiffDrive(const Motor& left, const Motor& right)
    : left_ref_{left}, right_ref_{right} {}

void DiffDrive::drive(const float steer, const float power) const {
  // Solve power differential
  const auto clamped_steer =
      std::clamp(steer, STEER_RANGE_MIN, STEER_RANGE_MAX);
  auto right_power = 0.0f;
  auto left_power = 0.0f;
  if (clamped_steer > 0) {
    // turn left, right motor is faster
    right_power = Motor::POWER_RANGE_MAX;
    left_power = right_power - clamped_steer;
  } else {
    // turn right, left motor is faster
    left_power = Motor::POWER_RANGE_MAX;
    right_power = left_power + clamped_steer;
  }

  // Scale by power multiplier
  right_power *= power;
  left_power *= power;

  // Send to motor
  left_ref_.get().drive(left_power);
  right_ref_.get().drive(right_power);
}

}  // namespace fcp::aut
