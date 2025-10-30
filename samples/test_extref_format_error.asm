. Test that EXTREF must use format 4
PROG    START   0
        EXTREF  EXTSYM
        LDA     EXTSYM
        END     PROG
