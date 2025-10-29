. Test boundary case: format 4 at text record boundary
. Ensures modification records are correct even when text records split

FMT4BND START   4000

. Fill up text record with format 3 instructions
        LDA     VAL1
        ADD     VAL2
        SUB     VAL3
        MUL     VAL4
        DIV     VAL5
        LDA     VAL6
        ADD     VAL7
        SUB     VAL8

. Format 4 instruction near boundary
        +LDA    VAL9

. More format 3
        STA     VAL10

. More format 4
        +JSUB   SUBR
        +LDA    RESULT

. Reserved data
BUFFER  RESB    100

. Format 4 after reserved data
        +LDA    VAL11
        +STA    VAL12

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
RESULT  WORD    0

. Subroutine
SUBR    LDA     VAL1
        RSUB

        END     FMT4BND
