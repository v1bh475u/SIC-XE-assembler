. Comprehensive test for =* literals and ORG *
TEST    START   0
. Test =* literals - each should get unique address
        LDA     =*
        STA     ADDR1
        LDA     =*
        STA     ADDR2
        LDA     =*
        STA     ADDR3

. Test ORG with *
        ORG     *
. Should stay at current position
        LDA     #100
        ORG     200
. Jump to address 200
        LDA     #200
        ORG
. Return to previous position
        STA     RESULT

. Test that regular literals are still deduplicated
        LDA     =C'ABC'
        LDA     =C'ABC'
. Both should use same literal
        LDA     =X'FFFF'
        LDA     =X'FFFF'
. Both should use same literal

        RSUB

ADDR1   RESW    1
ADDR2   RESW    1
ADDR3   RESW    1
RESULT  RESW    1
        END     TEST
