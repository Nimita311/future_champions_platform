#include "platform/morse/morse.hh"

#include <cstddef>
#include <gsl/util>

namespace fcp::platform {

constexpr Morse Morse::from_ascii(const char ascii) {
  // Converts lower case to upper case
  if (ascii >= 'a' && ascii <= 'z') {
    constexpr auto TO_UPPER = 'a' - 'A';
    return Morse{ASCII_33_95[ascii - TO_UPPER - ASCII_RANGE_FIRST]};
  }
  // Apply lookup table
  if (ascii >= ASCII_RANGE_FIRST && ascii <= ASCII_RANGE_LAST) {
    return Morse{ASCII_33_95[ascii - ASCII_RANGE_FIRST]};
  }
  //
  return Morse{NO_ENCODING};
}

constexpr int Morse::number_of_symbols() const {
  // Get the number of encoding bits including the starting bit
  auto bits_shifted = int{0};
  for (auto byte = encoding; byte != std::byte{0}; byte >>= 1) {
    bits_shifted++;
  }
  // Morse::ERROR has no starting bit and encodes 8 dits
  if (bits_shifted == 0) {
    return 8;
  }
  // Remove starting bit for other morse codes
  return bits_shifted - 1;
}

constexpr int Morse::space_of_symbol(const std::size_t n) const {
  const auto target_symbol =
      encoding & std::byte{gsl::narrow_cast<unsigned char>(1 << n)};
  if (target_symbol == std::byte{0}) {
    return SPACE_OF_DIT;
  }
  return SPACE_OF_DAH;
}

}  // namespace fcp::platform
