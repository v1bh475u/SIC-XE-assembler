. Test format 3 with SIC-compatible addressing
. SIC mode uses n=0, i=0 and direct addressing
. Should generate modification records for relocatable operands

SICMODE START   2000

. Note: To use SIC-compatible mode, we would need special syntax
. For now, regular format 3 uses n=1, i=1 with PC-relative addressing
. This test focuses on format 4 relocatable vs immediate

. Format 4 with symbol references (relocatable) - SHOULD have M records
START1  +LDA    DATA1
        +ADD    DATA2
        +SUB    DATA3

. Format 4 with immediate values (absolute) - should NOT have M records  
        +LDA    #100
        +ADD    #200
        +SUB    #50

. Mixed format 3 and 4
        LDA     LOCAL
        +STA    RESULT
        ADD     #10
        +ADD    DATA1

. Data section
DATA1   WORD    10
DATA2   WORD    20
DATA3   WORD    30
LOCAL   WORD    100
RESULT  WORD    0

        END     SICMODE
