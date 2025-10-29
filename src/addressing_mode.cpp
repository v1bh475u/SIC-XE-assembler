#include "addressing_mode.hpp"

namespace sicxe {

AddressingFlags AddressingFlags::from_mode(AddressingMode mode, bool indexed,
                                           bool is_format4) {
  AddressingFlags flags;
  flags.e = is_format4;
  flags.x = indexed;

  switch (mode) {
  case AddressingMode::IMMEDIATE:
    // n=0, i=1: Immediate addressing
    flags.n = 0;
    flags.i = 1;
    break;

  case AddressingMode::INDIRECT:
    // n=1, i=0: Indirect addressing
    flags.n = 1;
    flags.i = 0;
    break;

  case AddressingMode::SIMPLE:
  case AddressingMode::INDEXED:
    // n=1, i=1: Simple/Direct addressing (indexed handled by x bit)
    flags.n = 1;
    flags.i = 1;
    break;

  case AddressingMode::SIC_COMPATIBLE:
    // n=0, i=0: SIC compatible mode
    flags.n = 0;
    flags.i = 0;
    break;
  }

  return flags;
}

} // namespace sicxe
