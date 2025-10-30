#include "assembler.hpp"
#include "error_handler.hpp"
#include "opcode_encoder.hpp"
#include "pass1.hpp"
#include "pass2.hpp"
#include <fstream>
#include <iostream>

namespace sicxe {

Assembler::Assembler() {}

void Assembler::assemble(const std::string &input_filename,
                         const std::string &output_filename) {
  OpcodeEncoder encoder;

  Pass1 pass1(encoder);
  pass1.process(input_filename);

  if (pass1.has_errors()) {
    std::cerr << "\n";
    pass1.get_error_handler().print_errors();
    std::cerr << "\nAssembly failed due to Pass 1 errors.\n";
    return;
  }

  if (pass1.has_warnings()) {
    std::cerr << "\n";
    pass1.get_warning_handler().print_warnings();
    std::cerr << "\n";
  }

  std::cout << "Pass 1 complete. Symbol table populated.\n";
  std::cout << "Program length: 0x" << std::hex << pass1.get_program_length()
            << std::dec << "\n\n";

  std::vector<std::string> object_code;
  bool has_errors = false;

  if (pass1.uses_control_sections()) {
    for (const auto &section : pass1.get_control_sections()) {
      Pass2 pass2(encoder, section.symbol_table, section.literal_table,
                  section.start_address, section.length);
      auto section_code = pass2.generate_object_code(
          section.lines, section.name, section.extdef_symbols,
          section.extref_symbols);

      if (pass2.has_errors()) {
        std::cerr << "\n";
        pass2.get_error_handler().print_errors();
        std::cerr << "\nAssembly failed due to Pass 2 errors in section "
                  << section.name << ".\n";
        has_errors = true;
      }

      if (pass2.has_warnings()) {
        std::cerr << "\n";
        pass2.get_warning_handler().print_warnings();
        std::cerr << "\n";
      }

      if (has_errors) {
        return;
      }

      object_code.insert(object_code.end(), section_code.begin(),
                         section_code.end());
    }
  } else {
    // Backward compatibility: old programs without START or CSECT
    Pass2 pass2(encoder, pass1.get_symbol_table(), pass1.get_literal_table(),
                pass1.get_start_address(), pass1.get_program_length());
    object_code = pass2.generate_object_code(
        pass1.get_lines(), pass1.get_program_name(), pass1.get_extdef_symbols(),
        pass1.get_extref_symbols());

    if (pass2.has_errors()) {
      std::cerr << "\n";
      pass2.get_error_handler().print_errors();
      std::cerr << "\nAssembly failed due to Pass 2 errors.\n";
      return;
    }

    if (pass2.has_warnings()) {
      std::cerr << "\n";
      pass2.get_warning_handler().print_warnings();
      std::cerr << "\n";
    }
  }

  std::cout << "Pass 2 complete. Object code generated.\n\n";

  FileErrorHandler file_error_handler;
  std::ofstream outfile(output_filename);
  if (!outfile.is_open()) {
    file_error_handler.add_file_write_error(output_filename);
    file_error_handler.print_errors();
    return;
  }

  for (const auto &code : object_code) {
    outfile << code << "\n";
  }

  outfile.close();

  std::cout << "Assembly complete. Output written to: " << output_filename
            << "\n";
}

} // namespace sicxe
