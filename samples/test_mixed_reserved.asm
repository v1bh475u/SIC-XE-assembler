. Test multiple reserved data blocks with mixed initialized data
. This tests:
. 1. Multiple RESB/RESW blocks in different locations
. 2. Mixing BYTE/WORD directives with reserved blocks
. 3. Code sections before and after reserved blocks

MIXTEST START   4000

. Initial code section
BEGIN   LDA     DATA1
        ADD     DATA2
        STA     RESULT

. First initialized data block
DATA1   WORD    100
DATA2   WORD    200
RESULT  WORD    0

. First reserved block
BUF1    RESB    50

. Code after first reserved block
        LDA     DATA3
        MUL     DATA4

. Mixed data with BYTE directives
DATA3   WORD    300
MSG     BYTE    C'HELLO'
DATA4   WORD    400

. Second reserved block
BUF2    RESW    10

. More code
        DIV     DATA5
        STA     OUTPUT

. Final data section
DATA5   WORD    500
OUTPUT  WORD    0

. Third reserved block at the end
BUF3    RESB    100

        END     BEGIN
