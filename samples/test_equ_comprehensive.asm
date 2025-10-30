. Test EQU with expressions and special cases
PROG    START   1000
. Basic EQU
MAXLEN  EQU     4096
BUFEND  EQU     MAXLEN

. EQU with *
HERE    EQU     *         . Current location counter
        LDA     #HERE     . Should be 1000
        
. EQU with symbols
FIRST   RESW    1
SECOND  EQU     FIRST
THIRD   EQU     SECOND
        
. Use EQU values
        LDA     #MAXLEN
        STA     FIRST
        LDA     SECOND
        
. EQU after code
        LDA     #100
LATER   EQU     *         . Should be current address
        STA     TEMP

. Test EQU in expressions
SIZE    EQU     100
HALF    EQU     50
TOTAL   EQU     SIZE
        LDA     #TOTAL
        
        RSUB

TEMP    RESW    1
BUFFER  RESB    100
        END     PROG
