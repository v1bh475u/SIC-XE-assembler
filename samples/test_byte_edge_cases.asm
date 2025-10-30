. Test BYTE directive edge cases
PROG    START   0
. Single byte
VAL1    BYTE    C'A'      . Single character
VAL2    BYTE    X'0F'     . Single hex byte

. Multiple bytes
VAL3    BYTE    C'HELLO'  . String constant
VAL4    BYTE    X'0123'   . Two hex bytes
VAL5    BYTE    X'ABCDEF' . Three hex bytes

. Odd number of hex digits should error (handled by parser)
VAL6    BYTE    X'F0F0F0' . Six hex digits (3 bytes)

. Empty and special cases
VAL7    BYTE    C''       . Empty string (0 bytes)
VAL8    BYTE    X'00'     . Zero byte
VAL9    BYTE    X'FF'     . Max byte

. Mixed with other directives
VAL10   BYTE    C'TEST'
        WORD    100
VAL11   BYTE    X'AA'
        RESB    5
VAL12   BYTE    C'END'

. Test that BYTE affects location counter correctly
START1  LDA     VAL3
        STA     TEMP
        RSUB

TEMP    RESW    1
        END     PROG
