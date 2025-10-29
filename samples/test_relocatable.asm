. Test modification records: relocatable vs absolute operands
. Only relocatable symbol references should generate M records

RELTEST START   1000

. Format 4 with relocatable operand - SHOULD generate M record
        +LDA    BUFFER
        +STA    OUTPUT

. Format 4 with immediate (absolute) - should NOT generate M record
        +LDA    #1000
        +LDA    #500

. Format 3 with relocatable operand - should NOT generate M record (PC-relative)
        LDA     VALUE
        STA     RESULT

. Format 3 with immediate (absolute) - should NOT generate M record
        LDA     #100
        ADD     #50

. Format 4 subroutine call with relocatable - SHOULD generate M record
        +JSUB   SUBR

. More format 4 with relocatable references - SHOULD generate M records
        +LDCH   CHAR
        +STCH   BYTE

. Data section (all labels here are relocatable)
BUFFER  WORD    1000
OUTPUT  WORD    0
VALUE   WORD    500
RESULT  WORD    0
CHAR    BYTE    C'A'
BYTE    BYTE    X'00'

. Subroutine
SUBR    LDA     VALUE
        RSUB

        END     RELTEST
