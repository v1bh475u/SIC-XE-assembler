. Test all Format 2 instructions with register operations
. Based on Beck Table 2.1 and 2.2
PROG    START   0
. Register-to-register arithmetic
        ADDR    S,A       . Add S to A
        SUBR    T,S       . Subtract T from S
        MULR    S,T       . Multiply S by T
        DIVR    T,X       . Divide T by X
        
. Comparison operations
        COMPR   A,S       . Compare A with S
        
. Register manipulation
        CLEAR   A         . Clear A
        CLEAR   X         . Clear X
        CLEAR   L         . Clear L
        CLEAR   B         . Clear B
        CLEAR   S         . Clear S
        CLEAR   T         . Clear T
        CLEAR   F         . Clear F
        
. Register moves
        RMO     A,X       . Move A to X
        RMO     X,L       . Move X to L
        RMO     L,B       . Move L to B
        RMO     B,S       . Move B to S
        RMO     S,T       . Move S to T
        RMO     T,F       . Move T to F
        RMO     F,A       . Move F to A
        
. Shift operations
        SHIFTL  A,1       . Shift A left 1
        SHIFTL  A,4       . Shift A left 4
        SHIFTR  S,2       . Shift S right 2
        SHIFTR  T,3       . Shift T right 3
        
. Test and increment
        TIXR    X         . Test and increment X
        TIXR    T         . Test and increment T
        
. Supervisor call
        SVC     0         . SVC with value 0
        SVC     5         . SVC with value 5
        SVC     15        . SVC with max value
        
        RSUB
        END     PROG
