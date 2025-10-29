. Test file with long names (> 6 characters)
VERYLONGPROGRAMNAME    START   1000

. Long symbol names should be truncated
VERYLONGSYMBOL1   LDA     ALPHA
ANOTHERLONGSYMBOL STA     BETA
NORMALNAME        LDX     #5

. Define symbols
ALPHA             WORD    10
BETA              WORD    20
EXTRALONGNAME     WORD    30

                  END     VERYLONGSYMBOL1
