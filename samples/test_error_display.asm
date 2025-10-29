. Test file with multiple errors to demonstrate error handling
COPY    START   1000

. Duplicate symbol error
FIRST   LDA     ALPHA
FIRST   STA     BETA

. Invalid opcode
        INVALIDOP  X

. Undefined symbol
        LDA     UNDEFINED

. Invalid START address (in another file this would be tested)
ALPHA   WORD    5
BETA    RESW    1

        END     COPY
