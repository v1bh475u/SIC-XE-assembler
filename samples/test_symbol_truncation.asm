. Test maximum symbol length and truncation
PROG    START   0
. Standard length symbols (6 chars max in SIC/XE)
ABCDEF  WORD    1         . Exactly 6 chars
ABC     WORD    2         . Less than 6
A       WORD    3         . Single char

. Symbols exceeding 6 chars (should be truncated with warning)
ABCDEFG WORD    4         . 7 chars -> ABCDEF
ABCDEFGH WORD   5         . 8 chars -> ABCDEF  
VERYLONGSYMBOLNAME WORD 6 . Very long -> VERYLO

. Test truncated symbols don't conflict
ABCDEF1 WORD    7         . Truncates to ABCDEF (duplicate!)
ABCDEF2 WORD    8         . Truncates to ABCDEF (duplicate!)

. Use symbols
        LDA     ABCDEF
        LDA     ABC
        LDA     A
        LDA     ABCDEFG   . Uses truncated version
        RSUB
        END     PROG
