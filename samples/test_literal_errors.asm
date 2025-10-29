. Test file with literal parsing errors
PROG    START   1000

. Invalid hex literal (odd number of digits)
BAD1    BYTE    X'ABC'

. Invalid hex literal (bad characters)
BAD2    BYTE    X'1G'

. Unknown literal format
BAD3    BYTE    Z'hello'

        END     PROG
