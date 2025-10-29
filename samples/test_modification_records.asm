. Test modification records with multiple format 4 instructions
. Each format 4 instruction should generate one modification record

MODREC  START   2000

. Multiple format 4 instructions in sequence
START1  +LDA    DATA1
        +ADD    DATA2
        +SUB    DATA3
        +MUL    DATA4
        +DIV    DATA5

. Format 3 instructions (no modification)
        LDA     LOCAL
        STA     RESULT

. More format 4 with JSUB
        +JSUB   PROC1
        +JSUB   PROC2

. Data
DATA1   WORD    10
DATA2   WORD    20
DATA3   WORD    30
DATA4   WORD    40
DATA5   WORD    50
LOCAL   WORD    100
RESULT  WORD    0

. Procedures
PROC1   LDA     DATA1
        RSUB

PROC2   LDA     DATA2
        RSUB

        END     MODREC
