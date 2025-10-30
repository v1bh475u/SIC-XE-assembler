. Test tricky addressing mode combinations and edge cases
. Based on Beck System Software Chapter 2
COPY    START   1000
FIRST   STL     RETADR    . Simple direct
        LDB     #LENGTH   . Immediate mode
        BASE    LENGTH    . Set base register
CLOOP   +JSUB   WRREC     . Format 4 (extended)
        LDA     LENGTH    . PC-relative
        COMP    #0        . Immediate comparison
        JEQ     ENDFIL    . PC-relative jump
        +LDT    #4096     . Format 4 immediate (>12 bits)
        LDA     @RETADR   . Indirect addressing
        LDCH    BUFFER,X  . Indexed addressing
        +LDA    BUFFER    . Format 4 direct
        J       CLOOP     . Short jump
ENDFIL  LDA     EOF       . Near symbol
        STA     BUFFER
        LDA     #3
        STA     LENGTH
        +JSUB   WRREC     . Another extended call
        LDL     RETADR
        RSUB              . Return
EOF     BYTE    C'EOF'
RETADR  RESW    1
LENGTH  RESW    1
BUFFER  RESB    4096
WRREC   STX     @LENGTH   . Indirect indexed
        RSUB
        END     FIRST
