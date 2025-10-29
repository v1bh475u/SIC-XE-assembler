. Test format 4 with indexed addressing
. Format 4 with X register should also generate modification records

FMT4IDX START   3000

. Format 4 with indexed addressing
        +LDA    TABLE,X
        +STA    RESULT,X
        +LDCH   STRING,X

. Format 3 with indexed addressing (no modification)
        LDA     LOCAL,X
        STA     TEMP,X

. More format 4 combinations
        +LDX    #4096
        +STX    INDEX
        +LDA    ARRAY,X

. Data
TABLE   WORD    1
        WORD    2
        WORD    3
        WORD    4
        WORD    5
STRING  BYTE    C'HELLO'
RESULT  RESW    10
LOCAL   WORD    100
TEMP    WORD    0
INDEX   WORD    0
ARRAY   RESW    20

        END     FMT4IDX
