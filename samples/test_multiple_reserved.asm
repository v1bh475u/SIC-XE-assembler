. Test multiple reserved data blocks interspersed with code
. Each reserved block should terminate the current text record

MULTRES START   8000

. First code block
BLOCK1  LDA     VAL1
        ADD     VAL2
        STA     VAL3

. First reserved block
BUF1    RESB    50

. Second code block
BLOCK2  LDA     VAL4
        ADD     VAL5
        STA     VAL6

. Second reserved block
ARRAY1  RESW    20

. Third code block
BLOCK3  LDA     VAL7
        ADD     VAL8
        STA     VAL9

. Third reserved block
BUF2    RESB    100

. Fourth code block with data
BLOCK4  LDA     VAL10
VAL1    WORD    1
VAL2    WORD    2
VAL3    WORD    3
VAL4    WORD    4
VAL5    WORD    5
VAL6    WORD    6
VAL7    WORD    7
VAL8    WORD    8
VAL9    WORD    9
VAL10   WORD    10

        END     MULTRES
