. Test ORG directive with various scenarios
PROG    START   100
. Basic ORG usage
        LDA     #1
        STA     TEMP
        ORG     200       . Jump to address 200
        LDA     #2        . This is at 200
        ORG               . Return to saved location
        STA     TEMP2     . Continue from where we left off

. ORG with *
        LDA     #3
        ORG     *         . Stay at current location (no-op)
        STA     TEMP3

TEMP    RESW    1
TEMP2   RESW    1
TEMP3   RESW    1
TEMP4   RESW    1
TEMP5   RESW    1
        RESB    50
TABLE   RESB    10

. ORG with symbol
        ORG     TABLE
        BYTE    C'A'
        BYTE    C'B'
        BYTE    C'C'
        ORG               . Return

        RSUB
        END     PROG
