#pragma once

#include <array>
#include <cstddef>

namespace fcp::platform {

/**
 * @brief Morse code.
 *
 * Encoding inspired by https://kb8ojh.net/msp430/morse_encoding.html
 * MSB to LSB
 * [padding 0s] + [start bit 1] + [character 0 - dit, 1 - dah]
 */
struct Morse {
  // Timing

  static constexpr int SPACE_OF_DIT{1U};
  static constexpr int SPACE_OF_DAH{3U};
  static constexpr int SPACE_BETWEEN_SYMBOLS{1U};
  static constexpr int SPACE_BETWEEN_LETTERS{3U};
  static constexpr int SPACE_BETWEEN_WORDS{7U};

  // Prosigns

  static constexpr std::byte END_OF_WORK{0x45};                     // SK
  static constexpr std::byte ERROR{0x00};                           // HH
  static constexpr std::byte GENERAL_INVITATION_TO_TRANSMIT{0x0D};  // K
  static constexpr std::byte STARTING_SIGNAL{0x35};                 // CT
  static constexpr std::byte NEW_MESSAGE_FOLLOWS{0x2A};             // RN
  static constexpr std::byte VERIFIED{0x22};                        // S hat
  static constexpr std::byte WAIT{0x28};                            // &

  // Characters

  static constexpr std::byte NO_ENCODING{0x01};
  static constexpr char ASCII_RANGE_FIRST{'!'};
  static constexpr char ASCII_RANGE_LAST{'_'};
  static constexpr std::array<std::byte,
                              ASCII_RANGE_LAST - ASCII_RANGE_FIRST + 1>
      ASCII_33_95{
          std::byte{0x6B},  // ! 33 (AS, nonstandard)
          std::byte{0x52},  // "
          NO_ENCODING,      // # 35
          std::byte{0x89},  // $    (SX, nonstandard)
          NO_ENCODING,      // %
          std::byte{0x28},  // &    (AS, nonstandard)
          std::byte{0x5E},  // '
          std::byte{0x36},  // ( 40 (KN)
          std::byte{0x6D},  // )
          NO_ENCODING,      // *
          std::byte{0x2A},  // +    (RN)
          std::byte{0x73},  // ,
          std::byte{0x61},  // - 45
          std::byte{0x55},  // .
          std::byte{0x32},  // /    (DN)
          std::byte{0x3F},  // 0
          std::byte{0x2F},  // 1
          std::byte{0x27},  // 2 50
          std::byte{0x23},  // 3
          std::byte{0x21},  // 4
          std::byte{0x20},  // 5
          std::byte{0x30},  // 6
          std::byte{0x38},  // 7 55
          std::byte{0x3C},  // 8
          std::byte{0x3E},  // 9
          std::byte{0x78},  // :
          std::byte{0x6A},  // ;    (nonstandard)
          NO_ENCODING,      // < 60
          std::byte{0x31},  // =    (BT)
          NO_ENCODING,      // >
          std::byte{0x4C},  // ?
          std::byte{0x5A},  // @    (AC)
          std::byte{0x05},  // A 65
          std::byte{0x18},  // B
          std::byte{0x1A},  // C
          std::byte{0x0C},  // D
          std::byte{0x02},  // E
          std::byte{0x12},  // F 70
          std::byte{0x0E},  // G
          std::byte{0x10},  // H
          std::byte{0x04},  // I
          std::byte{0x17},  // J
          std::byte{0x0D},  // K 75
          std::byte{0x14},  // L
          std::byte{0x07},  // M
          std::byte{0x06},  // N
          std::byte{0x0F},  // O
          std::byte{0x16},  // P 80
          std::byte{0x1D},  // Q
          std::byte{0x0A},  // R
          std::byte{0x08},  // S
          std::byte{0x03},  // T
          std::byte{0x09},  // U 85
          std::byte{0x11},  // V
          std::byte{0x0B},  // W
          std::byte{0x19},  // X
          std::byte{0x1B},  // Y
          std::byte{0x1C},  // Z 90
          NO_ENCODING,      // [
          NO_ENCODING,      // &bsol
          NO_ENCODING,      // ]
          NO_ENCODING,      // ^
          std::byte{0x4D},  // _ 95 (nonstandard)
      };

  /**
   * @brief Converts an ASCII charater to Morse code.
   *
   * Not all ASCII charater has a Morse code. If there is no valid conversion,
   * Morse::NO_ENCODING will be returned.
   *
   * @param ascii Input ASCII character.
   * @return constexpr Morse Resulting Morse code.
   */
  static constexpr Morse from_ascii(const char ascii);

  /**
   * @brief Gets the number of symbols of this Morse code.
   *
   * Each dit and dah is considered a symbol.
   * Morse::NO_ENCODING has 0 symbols.
   * Morse::ERROR has 8 symbols.
   * All other Morse codes have 1 to 7 symbols.
   *
   * @return constexpr int Number of symbols between [0, 8].
   */
  constexpr int number_of_symbols() const;

  /**
   * @brief Gets the timing space of a symbol at a certain index.
   *
   * Dit has 1 space and dah has 3 spaces per standard.
   *
   * @param n Index of the symbol. The first symbol has index 0.
   * @pre Index @p n must be less than the total number of symbols.
   * @return constexpr int Timing space of the symbol.
   */
  constexpr int space_of_symbol(const std::size_t n) const;
  /**
   * @see Same as space_of_symbol().
   */
  inline constexpr int operator[](const std::size_t n) const {
    return space_of_symbol(n);
  }

  std::byte encoding;
};

}  // namespace fcp::platform
