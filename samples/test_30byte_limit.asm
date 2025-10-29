. Test text record 30-byte (60 hex char) limit
. Each format-3 instruction is 3 bytes (6 hex chars)
. So 10 instructions = 30 bytes = 60 hex chars (should fit in one text record)
. 11 instructions = 33 bytes = 66 hex chars (should create a new text record)

LIMIT30 START   4000

. First 10 instructions = exactly 30 bytes (60 hex chars)
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

. 11th instruction should start a new text record
        LDA     VAL11

. Reserved space - should terminate text record
BUFFER  RESB    50

. More code after reserved space
        ADD     VAL12
        SUB     VAL13

. Data section
VAL1    WORD    10
VAL2    WORD    20
VAL3    WORD    30
VAL4    WORD    40
VAL5    WORD    50
VAL6    WORD    60
VAL7    WORD    70
VAL8    WORD    80
VAL9    WORD    90
VAL10   WORD    100
VAL11   WORD    110
VAL12   WORD    120
VAL13   WORD    130

        END     LIMIT30
