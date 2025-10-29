. Test format 4 (extended) instructions and modification records
. Format 4 instructions use + prefix and generate modification records

FMT4TST START   1000

. Format 4 instructions (extended addressing)
        +LDA    BUFFER
        +STA    OUTPUT
        +JSUB   SUBR

. Format 3 instructions (no modification records)
        LDA     VALUE
        ADD     VALUE
        STA     RESULT

. More format 4 instructions
        +LDCH   CHAR
        +STCH   BYTE

. Mixed format 3 and 4
        LDA     NUM1
        +ADD    NUM2
        STA     SUM

. Data section
BUFFER  WORD    1000
OUTPUT  WORD    0
VALUE   WORD    500
RESULT  WORD    0
CHAR    BYTE    C'A'
BYTE    BYTE    X'00'
NUM1    WORD    100
NUM2    WORD    200
SUM     WORD    0

. Subroutine
SUBR    LDA     VALUE
        RSUB

        END     FMT4TST
