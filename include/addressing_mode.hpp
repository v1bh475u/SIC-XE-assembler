#ifndef ADDRESSING_MODE_HPP
#define ADDRESSING_MODE_HPP

#include <cstdint>

namespace sicxe {

enum class AddressingMode : uint8_t {
  SIMPLE,        // n=1, i=1: LDA BUFFER
  IMMEDIATE,     // n=0, i=1: LDA #3
  INDIRECT,      // n=1, i=0: LDA @BUFFER
  INDEXED,       // n=1, i=1, x=1: LDA BUFFER,X (can combine with other modes)
  SIC_COMPATIBLE // n=0, i=0: SIC mode (address field is target address)
};

// Flags for nixbpe bits in format 3/4
struct AddressingFlags {
  bool n = true;  // indirect addressing bit (n=1 means NOT immediate)
  bool i = true;  // immediate addressing bit
  bool x = false; // indexed addressing bit
  bool b = false; // base-relative bit
  bool p = false; // PC-relative bit
  bool e = false; // extended format bit (format 4)

  // Helper to create flags from addressing mode and indexed flag
  static AddressingFlags from_mode(AddressingMode mode, bool indexed,
                                   bool is_format4 = false);

  // Get nixbpe as a 6-bit value for debugging
  uint8_t to_byte() const {
    return (n ? 0x20 : 0) | (i ? 0x10 : 0) | (x ? 0x08 : 0) | (b ? 0x04 : 0) |
           (p ? 0x02 : 0) | (e ? 0x01 : 0);
  }
};

} // namespace sicxe

#endif // ADDRESSING_MODE_HPP
