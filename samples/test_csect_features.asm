. Control section with various features
MAIN    CSECT
        EXTDEF  INIT,BUFF,SIZE
        EXTREF  READ,WRITE,ERROR
. Initialize
INIT    STL     RETADR
        LDA     #100
        STA     SIZE
. Read data
        +JSUB   READ
        LDA     SIZE
        COMP    #0
        JEQ     DONE
. Write data  
        +JSUB   WRITE
. Done
DONE    LDL     RETADR
        RSUB
. Handle error
ERRHND  +JSUB   ERROR
        J       DONE
. Data area
RETADR  RESW    1
SIZE    RESW    1
BUFF    RESB    200
        END     INIT
