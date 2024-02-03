#pragma once

#include <functional>

#include "autonomy/motor/motor.hh"

namespace fcp::aut {

/**
 * @brief A drive train with two motors that steers by power differential.
 */
class DiffDrive {
 public:
  static constexpr float STEER_NEUTRAL = 0.0f;
  static constexpr float STEER_RANGE_MAX =
      Motor::POWER_RANGE_MAX - Motor::POWER_RANGE_MIN;
  static constexpr float STEER_RANGE_MIN = -STEER_RANGE_MAX;

  /**
   * @param left Motor on the left side when facing forward.
   * @param right Motor on the right side when facing forward.
   */
  DiffDrive(const Motor& left, const Motor& right);

  /**
   * @brief Drives the differential drive train.
   *
   * @param steer Power differential in range [-2.0f, 2.0f] defined by the power
   * of right motor minus the power of left motor when the faster motor drives
   * with power 1.0f. When this value is above zero, it steers to the left. This
   * value influences turning radius.
   * @param power Power multiplier that applies to both motors. This value
   * influences speed.
   */
  void drive(const float steer, const float power) const;

 private:
  const std::reference_wrapper<const Motor> left_ref_;
  const std::reference_wrapper<const Motor> right_ref_;
};

}  // namespace fcp::aut
