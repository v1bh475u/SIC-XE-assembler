PROG    START   1000
FIRST   LDA     =C'TEST'
        STA     BUFFER
        LTORG
SECOND  LDB     =X'F1'
        STB     BUFFER
        LTORG
BUFFER  RESW    1
        END     FIRST
