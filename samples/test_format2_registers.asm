. Test file for Format 2 instructions and inline comments
PROG    START   0
        LDA     #100      . Load immediate value
        ADDR    A,X       . Add register X to A
        COMPR   A,S       . Compare A with S
        CLEAR   A         . Clear register A
        TIXR    X         . Test and increment X
        SHIFTL  A,2       . Shift A left 2 bits
        SHIFTR  A,1       . Shift A right 1 bit
        RMO     A,T       . Move A to T
        SVC     1         . Supervisor call
        RSUB              . Return

BUFFER  RESW    10
        END     PROG
