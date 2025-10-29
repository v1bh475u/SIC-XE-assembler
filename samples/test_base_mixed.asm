. Test BASE directive with mixed addressing modes
. Demonstrates PC-relative, base-relative, and format 4

BASEMIX START   2000

. Initial code with PC-relative
START1  LDA     VAL1
        ADD     VAL2
        STA     SUM

. Local data
VAL1    WORD    100
VAL2    WORD    200
SUM     WORD    0

. Large gap to push FAR_DATA out of PC-relative range
        RESB    2500

. Set BASE for far data
        BASE    FDATA

. Access far data with base-relative
        LDA     FDATA
        ADD     FVAL1
        STA     FRES

. Or use format 4 explicitly
        +LDA    FVAL2
        +STA    FRES

. Clear BASE
        NOBASE

. Must use format 4 after NOBASE
        +LDA    FVAL3

. Far data section
FDATA   WORD    1000
FVAL1   WORD    2000
FVAL2   WORD    3000
FVAL3   WORD    4000
FRES    WORD    0

        END     BASEMIX
