. Test exact boundary: exactly 30 bytes in text record
. 10 format-3 instructions = exactly 30 bytes (60 hex chars)

EXACT30 START   5000

. Exactly 10 instructions (10 × 3 = 30 bytes = 60 hex chars)
        LDA     NUM1
        ADD     NUM2
        SUB     NUM3
        MUL     NUM4
        DIV     NUM5
        LDA     NUM6
        ADD     NUM7
        SUB     NUM8
        MUL     NUM9
        DIV     NUM10

. Reserved block - should be in separate section
SPACE   RESB    100

. One more instruction after reserved block
        LDA     NUM11

. Data
NUM1    WORD    1
NUM2    WORD    2
NUM3    WORD    3
NUM4    WORD    4
NUM5    WORD    5
NUM6    WORD    6
NUM7    WORD    7
NUM8    WORD    8
NUM9    WORD    9
NUM10   WORD    10
NUM11   WORD    11

        END     EXACT30
