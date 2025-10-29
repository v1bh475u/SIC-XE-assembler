. Test BASE directive with multiple data sections
. Switch BASE as needed for different data sections

BASESWITCH START 3000

. Code section
MAIN    LDA     #0
        STA     COUNT

. Set BASE to ARRAY1
        BASE    ARRAY1

. Access ARRAY1 with base-relative
        LDA     ARRAY1
        ADD     ARRAY1+3
        ADD     ARRAY1+6

. Switch BASE to ARRAY2
        BASE    ARRAY2

. Access ARRAY2 with base-relative  
        LDA     ARRAY2
        ADD     ARRAY2+3
        ADD     ARRAY2+6

. Switch BASE to ARRAY3
        BASE    ARRAY3

. Access ARRAY3 with base-relative
        LDA     ARRAY3
        STA     RESULT

. Local data
COUNT   WORD    0
RESULT  WORD    0

. Gap
        RESB    1500

. First array section
ARRAY1  WORD    10
        WORD    20
        WORD    30
        WORD    40
        WORD    50

. Gap
        RESB    1500

. Second array section
ARRAY2  WORD    60
        WORD    70
        WORD    80
        WORD    90
        WORD    100

. Gap
        RESB    1500

. Third array section
ARRAY3  WORD    110
        WORD    120
        WORD    130

        END     BASESWITCH
