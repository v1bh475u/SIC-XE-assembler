. Test file for ORG with * support
PROG    START   0
        LDA     #100
        STA     VAR1
        ORG     *
. Should maintain current address
        LDA     #200
        STA     VAR2
        ORG     80
. Should jump to address 80 (decimal)
        LDA     #300
        ORG
. Should return to previous address (after STA VAR2)
        STA     VAR3
        RSUB

VAR1    RESW    1
VAR2    RESW    1
VAR3    RESW    1
        END     PROG
