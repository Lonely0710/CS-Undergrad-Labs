CODE SEGMENT 		            ;初始化代码段
ASSUME CS:CODE
START:		                    ;初始化，设置A口B口工作方式
    MOV DX,293H 	            ;送入命令口地址
    MOV AL,10001001B 		    ;传入命令字
    OUT DX,AL

;显示A口输入的数并循环移位
    XOR AX,AX
    MOV BL,0
LOP:      
    MOV DX,292H  
    IN AL,DX   
    CMP AL,AH  
    JZ NEXT   
    MOV AH,AL
    MOV BL,AH
    ROL BL,1    
NEXT: 
    ROR BL,1   
    MOV AL,BL
    MOV DX,291H
    OUT DX,AL
    MOV CX,3FFFH                  ;控制显示次数

;显示8255-A 
DISPLAY:
    ;显示8
    ;位码op1
    MOV DX,291H
    MOV AL,00000000B
    OUT DX,AL                   ;位码控制，关闭所有灯

    ;段码op1
    MOV DX,290H
    MOV AL,7FH                  ;数字8所对应的段码
    OUT DX,AL

    ;位码op2
    MOV DX,291H
    MOV AL,00100000B            ;*
    OUT DX,AL
    CALL DELAY		            ;延时程序，防止数字跳转过快，便于观察
    
    ;显示2
    MOV DX,291H
    MOV AL,00000000B
    OUT DX,AL
    MOV DX,290H
    MOV AL,5BH                  ;数字2所对应的段码
    OUT DX,AL
    MOV DX,291H
    MOV AL,00010000B            ;*
    OUT DX,AL
    CALL DELAY

    ;显示5
    MOV DX,291H
    MOV AL,00000000B
    OUT DX,AL
    MOV DX,290H
    MOV AL,6DH                  ;数字5所对应的段码
    OUT DX,AL
    MOV DX,291H
    MOV AL,00001000B            ;*
    OUT DX,AL
    CALL DELAY

    ;显示5
    MOV DX,291H
    MOV AL,00000000B
    OUT DX,AL
    MOV DX,290H
    MOV AL,6DH                  ;数字5所对应的段码
    OUT DX,AL
    MOV DX,291H
    MOV AL,00000100B            ;*
    OUT DX,AL
    CALL DELAY

    ;显示-
    MOV DX,291H
    MOV AL,00000000B
    OUT DX,AL
    MOV DX,290H
    MOV AL,40H                  ;数字-所对应的段码
    OUT DX,AL
    MOV DX,291H
    MOV AL,00000010B            ;*
    OUT DX,AL
    CALL DELAY
    
    ;显示A
    MOV DX,291H
    MOV AL,00000000B
    OUT DX,AL
    MOV DX,290H
    MOV AL,77H                  ;数字A所对应的段码
    OUT DX,AL
    MOV DX,291H
    MOV AL,00000001B            ;*
    OUT DX,AL
    CALL DELAY

    LOOP DISPLAY                ;重复显示'8255-A'，LOOP一次，CX--
    JMP LOP                     ;

;延时程序 
DELAY PROC 
    PUSH BX
    PUSH CX
    MOV BX,0FFFFH               ;i
DL1:MOV CX,06FH                 ;j
DL2:DEC CX
    JNZ DL2
    DEC BX 
    JNZ DL1
    POP CX
    POP BX
    RET
DELAY ENDP

CODE ENDS
END START
