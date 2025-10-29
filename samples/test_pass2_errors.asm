. Test file with Pass 2 errors (undefined symbols)
PROG    START   1000

FIRST   LDA     ALPHA
SECOND  STA     BETA
THIRD   LDX     NOTDEFINED

. Define only ALPHA
ALPHA   WORD    5
BETA    WORD    10

        END     FIRST
