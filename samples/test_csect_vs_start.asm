. Using CSECT instead of START
WORKER  CSECT
        EXTDEF  DATA,TEMP
        EXTREF  HELPER
START   LDA     DATA
        +JSUB   HELPER
        STA     DATA
        RSUB
DATA    WORD    0
TEMP    RESW    5
        END     START
