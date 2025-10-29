PROG    START   1000
BUFFER  RESW    10
TABLE   ORG     BUFFER
        WORD    1
        WORD    2
        WORD    3
        ORG
NEXT    LDA     TABLE
        END     PROG
