. Comprehensive CSECT test with all features
PROG    CSECT
        EXTDEF  ALPHA,BETA,DELTA
        EXTREF  GAMMA,THETA
. Constants
FIVE    EQU     5
TEN     EQU     10
MAX     EQU     X'FF'
. Main logic
ALPHA   LDA     #FIVE
        +LDB    GAMMA
        STA     BETA
        +STT    THETA
        RSUB
. Data section
BETA    WORD    100
TEMP    RESW    3
BUFFER  RESB    50
DELTA   EQU     *
        END     ALPHA
