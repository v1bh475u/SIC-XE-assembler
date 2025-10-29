. Test literals with BASE directive
. Literals beyond PC-relative range should use BASE

LITBASE START   3000

. Initial code
START1  LDA     =C'A'
        STA     RESULT

. Local data
RESULT  WORD    0

. Large gap
        RESB    2000

. Set BASE
        BASE    FDATA

. Use literals with base-relative
        LDA     =C'FAR'
        ADD     =X'99'
        STA     FDATA

. Format 4 with literals
        +LDA    =C'TEST'
        +STA    =X'12345'

FDATA   WORD    0

        END     LITBASE
