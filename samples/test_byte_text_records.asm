. Test BYTE directives with text record management
. BYTE directives have variable length, so this tests proper counting

BYTETEST START  6000

. Code section
        LDA     MSG1
        LDCH    MSG2,X

. BYTE directives with different lengths
MSG1    BYTE    C'A'           . 1 byte
MSG2    BYTE    C'HELLO'       . 5 bytes  
MSG3    BYTE    X'0F'          . 1 byte
MSG4    BYTE    X'A1B2C3'      . 3 bytes
MSG5    BYTE    C'WORLD'       . 5 bytes
MSG6    BYTE    X'FF'          . 1 byte
MSG7    BYTE    C'TEST'        . 4 bytes
MSG8    BYTE    X'1234'        . 2 bytes
MSG9    BYTE    C'DATA'        . 4 bytes
MSG10   BYTE    X'ABCD'        . 2 bytes

. Total so far: 6 bytes (2 instructions) + 28 bytes (data) = 34 bytes
. Add more to approach 60 bytes

MSG11   BYTE    C'ABCDEFGHIJ'  . 10 bytes  (total: 44)
MSG12   BYTE    X'0102030405'  . 5 bytes   (total: 49)
MSG13   BYTE    C'KLMNOPQR'    . 8 bytes   (total: 57)
MSG14   BYTE    X'FF00AA'      . 3 bytes   (total: 60)

. Reserved space - should start new text record
SPACE   RESB    50

. More data after reserved space
MSG15   BYTE    C'AFTER'       . 5 bytes
NUM     WORD    123            . 3 bytes

        END     BYTETEST
