. Test absolute vs relocatable comprehensive
. Verify M records generated only for relocatable symbol references

ABSREL  START   3000

. Section 1: Format 4 with relocatable symbols
SEC1    +LDA    TABLE
        +STA    BUFFER
        +JSUB   PROC1

. Section 2: Format 4 with immediate (absolute) values
SEC2    +LDA    #4096
        +LDX    #10
        +STX    INDEX

. Section 3: Format 3 with relocatable (PC-relative, no M record needed)
SEC3    LDA     VALUE1
        ADD     VALUE2
        STA     SUM

. Section 4: Format 3 with immediate (absolute)
SEC4    LDA     #255
        ADD     #100
        MUL     #2

. Section 5: Mixed format 4 relocatable and immediate
SEC5    +LDA    ARRAY
        ADD     #1
        +STA    RESULT
        +LDX    #0

. Section 6: Format 4 indexed with relocatable base
SEC6    +LDA    TABLE,X
        +STA    BUFFER,X

. Data section
TABLE   WORD    1
        WORD    2
        WORD    3
BUFFER  RESW    10
INDEX   WORD    0
VALUE1  WORD    100
VALUE2  WORD    200
SUM     WORD    0
ARRAY   RESW    50
RESULT  WORD    0

. Subroutine
PROC1   LDA     VALUE1
        RSUB

        END     ABSREL
