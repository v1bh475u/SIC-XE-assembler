. Test Text record boundaries (max 30 bytes per record)
. Based on Beck Section 2.3.5
PROG    START   0
. Generate exactly 30 bytes of instructions
        LDA     VAR1      . 3 bytes (0-2)
        ADD     VAR2      . 3 bytes (3-5)
        SUB     VAR3      . 3 bytes (6-8)
        MUL     VAR4      . 3 bytes (9-11)
        DIV     VAR5      . 3 bytes (12-14)
        COMP    VAR6      . 3 bytes (15-17)
        JEQ     LABEL1    . 3 bytes (18-20)
        JGT     LABEL2    . 3 bytes (21-23)
        JLT     LABEL3    . 3 bytes (24-26)
        RSUB              . 3 bytes (27-29) = exactly 30 bytes

. Next instruction should start new T record
LABEL1  LDA     VAR7      . New T record starts here
LABEL2  STA     VAR8
LABEL3  RSUB

. Test with RESW/RESB breaking T records
        LDA     VAR1
        RESW    1         . Forces new T record for next instruction
        STA     VAR2
        
. Test with ORG breaking T records
        LDA     VAR3
        ORG     500
        STA     VAR4
        ORG
        LDA     VAR5

        RSUB

VAR1    WORD    1
VAR2    WORD    2
VAR3    WORD    3
VAR4    WORD    4
VAR5    WORD    5
VAR6    WORD    6
VAR7    WORD    7
VAR8    WORD    8
        END     PROG
