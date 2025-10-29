#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>

namespace sicxe {

class Assembler {
public:
  Assembler();

  void assemble(const std::string &input_filename,
                const std::string &output_filename);
};

} // namespace sicxe

#endif // ASSEMBLER_HPP
