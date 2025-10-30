. Test boundary conditions for PC-relative and Base-relative addressing
. PC-relative: -2048 to +2047 displacement
. Base-relative: 0 to 4095 displacement
PROG    START   0
        LDA     NEAR      . Should use PC-relative
        +LDB     FAR       . Should use Base-relative
        BASE    FAR
        LDA     FAR       . Now can use Base-relative
        LDA     NEAR      . Still PC-relative
. Test exactly at boundaries
        LDA     PC2047    . At +2047 boundary
        LDA     PCNEG     . Negative PC-relative
        RSUB

NEAR    WORD    1
PCNEG   WORD    2
        RESB    2035      . Gap to test boundary
PC2047  WORD    3         . Exactly at PC+2047
        RESB    100
FAR     WORD    4         . Beyond PC-relative range
        RESB    4000
FAREND  WORD    5         . At base+4095
        END     PROG
