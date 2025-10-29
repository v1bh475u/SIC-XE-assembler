. Test text record generation with reserved data
. This file tests:
. 1. Text record splits when exceeding 60 bytes (30 instructions)
. 2. Text record termination before RESB/RESW
. 3. New text record after reserved data

TXTTEST START   2000

. First block of code (should be in one text record)
FIRST   LDA     ALPHA
        ADD     BETA
        STA     GAMMA
        LDA     NUM1
        ADD     NUM2

. Some initialized data
ALPHA   WORD    100
BETA    WORD    200
GAMMA   WORD    300

. More code after initialized data
        LDA     DELTA
        STA     EPSILON

. Reserved data (should terminate current text record)
BUFFER  RESB    100
ARRAY   RESW    20

. Code after reserved data (should start new text record)
SECOND  LDA     ZETA
        ADD     ETA
        STA     THETA

. More initialized data
NUM1    WORD    50
NUM2    WORD    75
DELTA   WORD    400
EPSILON WORD    500
ZETA    WORD    600
ETA     WORD    700
THETA   WORD    800

        END     FIRST
