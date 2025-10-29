. Test basic literal support
. Literals should be dumped at END directive

LITTEST START   1000

. Use character literals
        LDA     =C'EOF'
        LDCH    =C'A'
        STCH    =C'Z'

. Use hex literals
        LDA     =X'05'
        ADD     =X'0A'
        SUB     =X'FF'

. Use same literal multiple times (should deduplicate)
        LDA     =C'EOF'
        ADD     =X'05'
        LDCH    =C'A'

. Regular symbol for comparison
BUFFER  WORD    1000
        LDA     BUFFER

. More literals
        +LDA    =C'HELLO'
        +STA    =X'AABBCC'

        END     LITTEST
