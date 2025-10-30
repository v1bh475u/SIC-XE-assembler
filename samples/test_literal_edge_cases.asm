. Test complex literal scenarios
. Including deduplication, =*, and all literal types
PROG    START   0
. Test character literals
        LDA     =C'A'
        LDA     =C'A'     . Should deduplicate
        LDA     =C'TEST'
        LDA     =C'TEST'  . Should deduplicate
        
. Test hex literals
        LDA     =X'05'
        LDA     =X'05'    . Should deduplicate
        LDA     =X'F1'
    LDA     =X'0F1'   . Leading zero should deduplicate
        LDA     =X'ABCD'
        LDA     =X'ABCD'  . Should deduplicate
        
. Test =* literals (should NOT deduplicate)
ADDR1   LDA     =*        . Should have value of next addr (ADDR1+3)
        STA     TEMP1
ADDR2   LDA     =*        . Different value (ADDR2+3)
        STA     TEMP2
ADDR3   LDA     =*        . Another different value (ADDR3+3)
        STA     TEMP3
        
. Mix with regular literals
        LDA     =C'X'
        LDA     =X'FF'
        LDA     =*        . Fourth =* literal
        
. Force literal pool with LTORG
        LTORG
        
. More literals after LTORG
AFTER   LDA     =C'AFTER'
        LDA     =*        . Fifth =* (should have AFTER+3)
        LDA     =C'AFTER' . Should deduplicate with first AFTER
        
        RSUB

TEMP1   RESW    1
TEMP2   RESW    1
TEMP3   RESW    1
        END     PROG
