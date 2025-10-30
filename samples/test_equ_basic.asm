PROG    START   1000
FIVE    EQU     5
TEN     EQU     10
TWENTY  EQU     20
MAXLEN  EQU     X'FF'
THIRTY  EQU     TEN
        LDA     #FIVE
        LDB     #TEN
        LDT     #MAXLEN
        LDS     #THIRTY
BUFFER  RESW    1
        END     PROG
