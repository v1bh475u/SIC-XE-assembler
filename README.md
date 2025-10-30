# SIC/XE Assembler

A modern C++20 assembler for the SIC/XE (Simplified Instructional Computer - Extended) architecture.

## Features

### Modern C++ Design
- **C++17 standard** with modern features (`std::variant`, `std::optional`)
- **Variant-based instruction formats** using `std::variant` for Format 1/2/3/4
- **Robust error handling** with custom exception hierarchy
- **Extensible directive system** using registry pattern
- **Efficient symbol table** with hash-based lookup

### Addressing Modes
- **Simple** (direct): `LDA BUFFER`
- **Immediate**: `LDA #3` (numeric value) or `LDA #SYMBOL` (symbol address)
- **Indirect**: `LDA @POINTER`
- **Indexed**: `LDA BUFFER,X`
- **PC-relative** (automatic during encoding, even for immediate mode with symbols)
- **Base-relative** (when BASE directive is used)
- **Extended format (Format 4)**: `+LDA BUFFER`

**Note on Immediate Mode with Symbols**: When using immediate addressing with a symbol (e.g., `LDB #LENGTH`), the assembler uses PC-relative or base-relative addressing to calculate the symbol's address, then stores it in the displacement field. This is the correct SIC/XE behavior as described in Leland Beck's "System Software".

### Instruction Formats
- **Format 1** (1 byte): `FIX`, `FLOAT`, `NORM`, etc.
- **Format 2** (2 bytes): `ADDR`, `CLEAR`, `COMPR`, etc.
- **Format 3** (3 bytes): Most instructions (`LDA`, `STA`, `ADD`, etc.)
- **Format 4** (4 bytes): Extended format with `+` prefix

### Directives
All directives are handled through an extensible registry system:
- `START` - Define program start address
- `END` - End of program
- `BYTE` - Define byte constant (`C'text'` or `X'hex'`)
- `WORD` - Define 3-byte word constant
- `RESB` - Reserve bytes
- `RESW` - Reserve words
- `BASE` - Set base register for base-relative addressing
- `NOBASE` - Clear base register
- `LTORG` - Place literal pool
- `ORG` - Set location counter
- `EQU` - Define symbol with value
- `CSECT` - Define control section
- `EXTDEF` - Declare external definitions
- `EXTREF` - Declare external references
- `USE` - **Not supported** (error message will be displayed)

### Literal Support
- **Hexadecimal literals**: `X'A1B2'` → bytes `0xA1 0xB2`
- **Character literals**: `C'HELLO'` → ASCII bytes

### Object Code Format
Generates standard SIC/XE object code with:
- **H (Header) record**: Program name, start address, length
- **T (Text) record**: Object code segments
- **E (End) record**: First executable address

## Building

### Quick Setup (Bash)
```bash
./setup_build.sh
```

### Manual Build
```bash
mkdir -p build && cd build
cmake ..
make
```

The build script will:
1. Create the `build` directory if it doesn't exist
2. Configure the project with CMake
3. Compile the assembler
4. Display the executable location

## Usage

```bash
./build/sicxe-assembler <input.asm> <output.obj>
```

### Example

```assembly
COPY    START   1000
FIRST   LDA     ALPHA
        ADD     BETA
        STA     GAMMA
ALPHA   RESW    1
BETA    RESW    1
GAMMA   RESW    1
        END     FIRST
```

Run assembler:
```bash
./build/sicxe-assembler test.asm output.obj
```

Output (`output.obj`):
```
H^COPY  ^001000^000012
T^001000^09^0320061B20060F2006
E^001000
```

## Testing

The `samples/` directory contains numerous test files covering various features:

- **Basic tests**: `test_basic.asm`, `test_directives.asm`
- **Addressing modes**: `test_addressing_modes.asm`, `test_format4.asm`
- **BASE directive**: `test_base_directive.asm`, `test_base_mixed.asm`
- **Literals**: `test_literals.asm`, `test_ltorg_basic.asm`
- **Control sections**: `test_csect_simple.asm`, `test_multi_csect.asm`
- **Beck's examples**: Tests based on Leland Beck's "System Software" textbook

To test with Beck's sample program:
```bash
./build/sicxe-assembler build/test4.asm build/output.obj
```

## Architecture

### Components

#### Core Types
- **`Instruction`**: Variant holding Format1/2/3/4 instruction data
- **`OpcodeDefinition`**: Opcode with supported formats list
- **`Line`**: Parsed source line with label, mnemonic, operand, addressing mode
- **`AddressingMode`**: Enum for all addressing modes
- **`AddressingFlags`**: nixbpe bits for format 3/4

#### Passes
- **Pass 1** (`pass1.cpp`): 
  - Parse source lines
  - Build symbol table
  - Calculate addresses and program length
  - Handle START/END directives
  
- **Pass 2** (`pass2.cpp`):
  - Encode instructions to object code
  - Generate H/T/E records
  - Resolve symbols and calculate displacements
  - Apply PC-relative addressing

#### Support Systems
- **`DirectiveRegistry`**: Extensible directive handler system
- **`LiteralParser`**: Parse `X'...'` and `C'...'` literals
- **`OpcodeEncoder`**: Opcode lookup with multi-format support
- **Error hierarchy**: `AssemblerError`, `SyntaxError`, `UndefinedSymbolError`, etc.

### Adding New Features

#### Add a New Directive
```cpp
// 1. Create handler class in directive.hpp
class MyDirective : public DirectiveHandler {
public:
    DirectiveResult process_pass1(const Line& line, int current_address) override;
    std::string generate_object_code(const Line& line, const SymbolTable& symtab) override;
};

// 2. Register in DirectiveRegistry::initialize_standard_directives()
register_directive("MYDIRECTIVE", std::make_unique<MyDirective>());
```

#### Add New Opcodes
```cpp
// In opcode_encoder.cpp initialize_opcodes()
add_opcode("NEWOP", 0xXX, {3, 4});  // Supports formats 3 and 4
add_opcode("NEWOP2", 0xYY, {2});     // Format 2 only
```

## Error Handling

The assembler provides detailed error messages with line numbers:
```
Assembly error: Line 5: Duplicate symbol: ALPHA
Assembly error: Line 10: Invalid opcode: INVALID
Assembly error: Line 15: Undefined symbol: MISSING
```

## Important Implementation Notes

### Immediate Addressing with Symbols

When using immediate addressing mode with a **symbol** (e.g., `LDB #LENGTH`), the assembler correctly uses PC-relative or base-relative addressing to encode the symbol's address, rather than treating it as a literal numeric value. This follows the SIC/XE specification:

```assembly
LDB     #LENGTH    ; Generates: 69202D (with PC-relative addressing)
                   ; NOT: 690033 (direct displacement)
```

The key distinction:
- `LDA #100` → Direct value 100 in displacement field
- `LDA #SYMBOL` → PC/base-relative displacement to SYMBOL's address

This allows immediate mode to reference symbol addresses that are beyond the 12-bit immediate value range by using relative addressing.

## Limitations and Unsupported Features

### USE Directive
The `USE` directive for managing multiple control sections is **not supported**. If encountered, the assembler will display an error message:
```
[Error] Line X: USE directive is not supported. This assembler does not support control sections with USE.
```

**Workaround**: Use `CSECT` for multiple control sections, but note that they will be processed sequentially without the ability to interleave sections.

### Other Known Limitations
- No support for floating-point immediate values
- No support for assembler expressions in operands (e.g., `SYMBOL+10`)
- Limited support for forward references in EQU directives

## Opcodes Supported

### Format 3/4 Instructions (70+ opcodes)
ADD, ADDF, AND, COMP, COMPF, DIV, DIVF, J, JEQ, JGT, JLT, JSUB, LDA, LDB, LDCH, LDF, LDL, LDS, LDT, LDX, LPS, MUL, MULF, OR, RD, RSUB, SSK, STA, STB, STCH, STF, STI, STL, STS, STSW, STT, STX, SUB, SUBF, TD, TIX, WD

### Format 2 Instructions
ADDR, CLEAR, COMPR, DIVR, MULR, RMO, SHIFTL, SHIFTR, SUBR, SVC, TIXR

### Format 1 Instructions
FIX, FLOAT, HIO, NORM, SIO, TIO