. Multiple control sections example
COPY    START   0
        EXTDEF  BUFFER,BUFEND,LENGTH
        EXTREF  RDREC,WRREC
FIRST   STL     RETADR
        +JSUB   RDREC
        LDA     LENGTH
        +JSUB   WRREC
        LDL     RETADR
        RSUB
RETADR  RESW    1
LENGTH  RESW    1
BUFFER  RESB    100
BUFEND  EQU     *
        END     FIRST
