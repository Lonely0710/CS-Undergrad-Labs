DSEG SEGMENT
    PORTA EQU 280H; Port A
    PORTB EQU 281H; Port B
    PORTD EQU 283H; Command and status port
DSEG ENDS
    
CODE SEGMENT
    ASSUME CS:CODE,DS:DSEG
START:
    MOV AX,DSEG
    MOV DS,AX
    
    ; Hardware initialization
    MOV DX,PORTD
    MOV AL,10010000
    OUT DX,AL
    
    MOV DX,PORTB
    MOV AL,00H
    OUT DX,AL
    
    WAIT1:
    MOV DX,PORTA
    IN AL,DX
    AND AL,11111111B
    JZ WAIT1
    MOV CL,AL
    MOV BL,AL

    LOOP1:
    MOV DX,PORTA

    IN AL,DX
    MOV BH,AL
    
    CMP BH,BL
    JZ LIGHT 
    MOV BL,BH
    MOV CL,BL
    
    LIGHT:
    MOV AL,CL
    MOV DX,PORTB
    OUT DX,AL
    
    CALL DELAY1
    ROR AL,1
    MOV CL,AL
    
    JMP LOOP1
    
    DELAY1 PROC
    MOV CX,00FFH
    DELAY2:
    MOV DX,0FFFFH
    DELAY:
    DEC DX
    JNZ DELAY
    LOOP DELAY2
    RET
    DELAY1 ENDP
    
    CODE ENDS
    END START
