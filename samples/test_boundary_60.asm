. Test boundary condition: exactly 60 bytes in text record
. 20 format-3 instructions = exactly 60 bytes

BOUND60 START   5000

. Exactly 20 instructions (20 × 3 = 60 bytes)
        LDA     VAL1
        ADD     VAL2
        SUB     VAL3
        MUL     VAL4
        DIV     VAL5
        LDA     VAL6
        ADD     VAL7
        SUB     VAL8
        MUL     VAL9
        DIV     VAL10
        LDA     VAL11
        ADD     VAL12
        SUB     VAL13
        MUL     VAL14
        DIV     VAL15
        LDA     VAL16
        ADD     VAL17
        SUB     VAL18
        MUL     VAL19
        DIV     VAL20

. Reserved block - should be in separate section
BUFFER  RESB    100

. One more instruction after reserved block
        LDA     VAL21

. Data
VAL1    WORD    1
VAL2    WORD    2
VAL3    WORD    3
VAL4    WORD    4
VAL5    WORD    5
VAL6    WORD    6
VAL7    WORD    7
VAL8    WORD    8
VAL9    WORD    9
VAL10   WORD    10
VAL11   WORD    11
VAL12   WORD    12
VAL13   WORD    13
VAL14   WORD    14
VAL15   WORD    15
VAL16   WORD    16
VAL17   WORD    17
VAL18   WORD    18
VAL19   WORD    19
VAL20   WORD    20
VAL21   WORD    21

        END     BOUND60
