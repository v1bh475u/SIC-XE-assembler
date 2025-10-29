. Comprehensive addressing mode test for SIC/XE
. Tests all valid nixbpe bit configurations

PROG    START   2000

. ==========================================
. Simple addressing: n=1, i=1
. nixbpe: 110010 (with PC-relative)
. ==========================================
SIMPLE  LDA     DATA1
        STA     DATA2

. ==========================================
. Immediate addressing: n=0, i=1
. nixbpe: 010000
. ==========================================
IMM     LDA     #5
        LDA     #100
        LDA     #0

. ==========================================
. Indirect addressing: n=1, i=0
. nixbpe: 100010 (with PC-relative)
. ==========================================
IND     LDA     @DATA1
        STA     @DATA2

. ==========================================
. Indexed addressing: n=1, i=1, x=1
. nixbpe: 111010 (with PC-relative)
. ==========================================
IDX     LDA     DATA1,X
        STA     DATA2,X
        LDCH    BUFFER,X

. ==========================================
. Format 4 simple: n=1, i=1, e=1
. nixbpe: 110001
. ==========================================
FMT4    +LDA    DATA1
        +STA    DATA2

. ==========================================
. Format 4 indexed: n=1, i=1, x=1, e=1
. nixbpe: 111001
. ==========================================
F4IDX   +LDA    DATA1,X
        +STA    DATA2,X

. ==========================================
. Format 4 immediate: n=0, i=1, e=1
. nixbpe: 010001
. ==========================================
F4IMM   +LDA    #1000
        +LDA    #65535

. ==========================================
. Format 4 indirect: n=1, i=0, e=1
. nixbpe: 100001
. ==========================================
F4IND   +LDA    @DATA1

. ==========================================
. Data section
. ==========================================
DATA1   WORD    10
DATA2   WORD    20
BUFFER  RESB    100

SUBR    RSUB

        END     SIMPLE
