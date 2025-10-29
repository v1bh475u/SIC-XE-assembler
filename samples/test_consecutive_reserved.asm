. Test consecutive reserved blocks
. Multiple RESB/RESW in a row should not create empty text records

CONSEC  START   7000

. Initial code
        LDA     DATA1
        STA     DATA2

. Data
DATA1   WORD    100
DATA2   WORD    200

. Multiple consecutive reserved blocks
BUF1    RESB    10
BUF2    RESB    20
ARR1    RESW    5
BUF3    RESB    30
ARR2    RESW    10

. Code after all reserved blocks
        LDA     DATA3
        ADD     DATA4

. Final data
DATA3   WORD    300
DATA4   WORD    400

        END     CONSEC
