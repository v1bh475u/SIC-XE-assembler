. Test literal deduplication
. Same literals should share the same address

LITDUP  START   2000

. First occurrence of literals
START1  LDA     =C'X'
        ADD     =X'10'
        SUB     =C'ABC'

. Duplicate occurrences (should reuse addresses)
        LDA     =C'X'
        ADD     =X'10'
        SUB     =C'ABC'

. Mix of new and duplicate literals
        LDCH    =C'Y'
        LDA     =C'X'
        ADD     =X'20'
        SUB     =X'10'

. Format 4 with literals
        +LDA    =C'LONG'
        +STA    =X'FFFF'
        +LDA    =C'LONG'

        END     LITDUP
