PROGA   START   0
        EXTDEF  ALPHA,BETA
        EXTREF  GAMMA
        
ALPHA   LDA     #10
        STA     TEMP
        +JSUB   GAMMA
        RSUB

TEMP    RESW    1
BETA    WORD    100
        END

PROGB   CSECT
        EXTDEF  DELTA,EPSILON
        EXTREF  ALPHA,BETA
        
DELTA   LDA     #20
        STA     TEMP
        +LDA    ALPHA
        +STA    BETA
        RSUB

TEMP    RESW    1
EPSILON WORD    200
        END

PROGC   CSECT
        EXTDEF  GAMMA
        EXTREF  DELTA
        
GAMMA   LDA     #30
        STA     TEMP
        +JSUB   DELTA
        RSUB

TEMP    RESW    1
        END
