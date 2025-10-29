. Test directives: BYTE, WORD, RESB, RESW

DIRTEST START   3000

. BYTE directive with character literals
MSG1    BYTE    C'HELLO'
MSG2    BYTE    C'WORLD'
MSG3    BYTE    C'X'

. BYTE directive with hex literals
HEX1    BYTE    X'0F'
HEX2    BYTE    X'A1B2'
HEX3    BYTE    X'FF00AA'

. WORD directive
NUM1    WORD    10
NUM2    WORD    -5
NUM3    WORD    1000

. RESB directive (reserve bytes)
BUF1    RESB    10
BUF2    RESB    100

. RESW directive (reserve words)
ARR1    RESW    5
ARR2    RESW    20

. Test label references
        LDA     NUM1
        STA     NUM2
        LDCH    MSG1,X

        END     DIRTEST
