. Test mixed instructions, BYTE, and WORD with 30-byte limit
. Tests various combinations and reserved data splitting

MIXED30 START   7000

. Mix of instructions and data
START1  LDA     ALPHA
        ADD     BETA
        STA     GAMMA
        LDA     DELTA
        ADD     EPSILON
        STA     ZETA
ALPHA   WORD    100
BETA    WORD    200
GAMMA   WORD    0
MSG1    BYTE    C'DATA'
DELTA   WORD    300
EPSILON WORD    400
ZETA    WORD    0

. Reserved block
BUFFER  RESB    50
ARRAY   RESW    10

. New section after reserved
START2  LDA     ETA
        ADD     THETA
ETA     WORD    500
THETA   WORD    600
        STA     IOTA
IOTA    WORD    0

        END     MIXED30
