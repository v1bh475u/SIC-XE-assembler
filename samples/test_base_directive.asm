. Test BASE directive for base-relative addressing
. Use BASE when target is beyond PC-relative range

BASETEST START  1000

. Set up code that references data far away
FIRST   LDA     ALPHA
        STA     BETA

. Set base register to TABLE
        BASE    TABLE

. Access table entries using base-relative
        LDA     TABLE
        LDA     ENTRY1
        LDA     ENTRY2
        STA     RESULT

. Clear base register
        NOBASE

. This should use PC-relative again
        LDA     LOCAL

. Local data (within PC-relative range from FIRST)
ALPHA   WORD    10
BETA    WORD    20
LOCAL   WORD    30

. Reserve large space to push TABLE beyond PC-relative range
        RESB    3000

. Data section far from code (requires BASE)
TABLE   WORD    100
ENTRY1  WORD    200
ENTRY2  WORD    300
RESULT  WORD    0

        END     BASETEST
