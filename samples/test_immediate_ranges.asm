. Test immediate mode with various value ranges
PROG    START   0
. Small immediate values (fit in 12 bits with PC-relative)
        LDA     #0        . Zero
        LDA     #1        . One
        LDA     #255      . Fits in one byte
        LDA     #2047     . Max positive for 12-bit signed
        LDA     #4095     . Max for 12-bit unsigned

. Negative immediate (if supported)
        LDA     #-1
        LDA     #-2048    . Min for 12-bit signed

. Large immediate values (require Format 4)
        +LDA    #4096     . Requires extended format
        +LDA    #65535    . 16-bit value
        +LDA    #1048575  . Max 20-bit value

. Immediate with hex notation
        LDA     #X'0F'
        LDA     #X'FF'
        +LDA    #X'FFFF'

. Immediate mode with different instructions
        COMP    #0
        AND     #X'FF'
        OR      #X'0F'
        ADD     #100
        SUB     #50

        RSUB
        END     PROG
