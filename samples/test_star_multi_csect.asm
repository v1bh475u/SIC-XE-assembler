. Test =* literals in multiple control sections
SECT1   START   0
        LDA     =*
        STA     TEMP1
        RSUB

TEMP1   RESW    1
        END

SECT2   CSECT
        LDA     =*
        STA     TEMP2
        RSUB

TEMP2   RESW    1
        END
