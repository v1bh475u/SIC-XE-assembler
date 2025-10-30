. Test EQU with various formats
PROG    START   X'1000'
. Decimal constants
FIVE    EQU     5
TEN     EQU     10
. Hex constant
MASK    EQU     X'FF'
. Using another EQU symbol
DOUBLE  EQU     TEN
. Instructions using EQU symbols
FIRST   LDA     #FIVE
        LDB     #MASK
        LDT     #DOUBLE
        END     PROG
