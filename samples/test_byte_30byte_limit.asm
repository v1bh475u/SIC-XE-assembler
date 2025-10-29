. Test BYTE directives with 30-byte (60 hex char) limit
. BYTE directives have variable length

BYTELIM START   6000

. Code
        LDA     MSG1
        LDCH    MSG2,X

. BYTE directives - total approaching 30 bytes
MSG1    BYTE    C'HELLO'
MSG2    BYTE    C'WORLD'
MSG3    BYTE    X'AABBCC'
MSG4    BYTE    C'TEST'
MSG5    BYTE    X'112233'
MSG6    BYTE    C'AB'
MSG7    BYTE    X'FF'
MSG8    BYTE    X'00'

. This should start a new text record
MSG9    BYTE    C'NEW'

. Reserved space
BUFFER  RESB    100

. More data after reserved
MSG10   BYTE    C'AFTER'
NUM     WORD    999

        END     BYTELIM
