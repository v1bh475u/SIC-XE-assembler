. Control section test - simple case
PROGA   START   0
        EXTDEF  LISTA,ENDA
        EXTREF  LISTB,LISTC
        LDA     LISTA
        +LDB    LISTB
        +LDT    LISTC
        STA     ENDA
LISTA   RESW    100
ENDA    EQU     *
        END     PROGA
