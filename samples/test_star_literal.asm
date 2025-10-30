. Test file for =* literal support
COPY    START   0
FIRST   LDA     =*
        STA     BUFFER
        LDA     =*
        LDA     =C'EOF'
        LDA     =C'EOF'
        RSUB
        CLEAR   F
        COMPR   F,S

BUFFER  RESB    100
        END     FIRST
