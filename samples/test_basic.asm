. Test basic SIC/XE assembly features
. Simple program with labels and basic instructions

COPY    START   1000
FIRST   LDA     ALPHA
        ADD     BETA
        STA     GAMMA
ALPHA   RESW    1
BETA    RESW    1
GAMMA   RESW    1
        END     FIRST
