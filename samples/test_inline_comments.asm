. Comprehensive test for inline comments and registers
PROG    START   0       . Start of program
FIRST   LDA     #100    . Load value
        STA     TEMP    . Store in TEMP
        ADDR    A,X     . Add X to A
        LDA     =C'TEST'. Load literal with inline comment
        COMPR   A,S     . Compare registers
        CLEAR   A       . Clear accumulator
        RSUB            . Return to caller

    TEMP    WORD    0       . Temporary storage
            END     FIRST   . End with first executable address
