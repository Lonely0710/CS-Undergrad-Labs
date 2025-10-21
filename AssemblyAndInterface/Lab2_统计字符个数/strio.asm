DATA SEGMENT
    NUM    DB 0
    LETTER DB 0
    OTHER  DB 0
    Res1   DB 'Letters: ', '$'
    Res2   DB 'Numbers: ', '$'
    Res3   DB 'Others: ', '$'
DATA ENDS

ASSUME CS:CSEG, DS:DATA, SS:SSEG

SSEG SEGMENT STACK
DW 64 DUP(?)
SSEG ENDS

CSEG SEGMENT
    ;子程序——处理数据
COPE_DATA proc near
    PUSH DX
    XOR  AH,AH
    MOV  AL,DL
    MOV  DH,10
    DIV  DH ; 百位数/10
    ;商是否为0
    TEST AL,0ffh
    ;若商AL为0，则为一位数 -> single
    JZ   single

    ;商AL不为0，至少为两位数
    PUSH AX ; 第一次商&余结果 AX 入栈
    XOR  AH,AH 
    DIV  DH ; 十位数/10
    TEST AL,0ffh
    ;若商AL为0，则为两位数 -> two
    JZ   two

    ;以上两种情况都不符合，说明为三位数，直接输出百位数
    PUSH AX ; 第二次商&余结果 AX 入栈
    MOV  DL,AL
    ADD  DL,'0'
    MOV  AH,02H
    INT  21H
    POP  AX ; 第二次商&余结果 AX 出栈
    two:      
    MOV  DL,AH
    ADD  DL,'0'
    MOV  AH,02H
    INT  21H
    POP  AX ; 第一次商&余结果 AX 出栈

    single:   
    MOV  DL,AH
    ADD  DL,'0'
    MOV  AH,02H
    INT  21H
    POP  DX
    RET
COPE_DATA ENDP

    ;主程序——测试键入字符数量
    START:    
    MOV  AX,DATA
    MOV  DS,AX

    MAIN_LOOP:
    MOV  AH,1
    INT  21H
    MOV  BL,AL

    CMP  BL, 0DH         ; 回车直接显示
    JE   SHOW_RES

    CMP  BL, 'A'
    JB   IFNUM
    CMP  BL, 'Z'
    JA   IFLETTER

    ; if 'A' < char < 'Z'
    INC  LETTER
    JMP  MAIN_LOOP

    IFNUM:    
    CMP  BL, '0'
    JB   IFOTHER
    CMP  BL, '9'
    JA   IFOTHER

    INC  NUM
    JMP  MAIN_LOOP

    IFLETTER: 
    CMP  BL, 'a'
    JB   IFOTHER
    CMP  BL, 'z'
    JA   IFOTHER

    INC  LETTER
    JMP  MAIN_LOOP

    IFOTHER:  
    INC  OTHER
    JMP  MAIN_LOOP

    ; 结果展示
    SHOW_RES: 
    MOV  AH, 09H
    LEA  DX, Res1
    INT  21H
    MOV  DL, [LETTER]
    CALL COPE_DATA

    MOV  AH, 02H
    MOV  DX,' '
    INT  21H
    MOV  AH, 09H
    LEA  DX, Res2
    INT  21H
    MOV  DL, [NUM]
    CALL COPE_DATA

    MOV  AH, 02H
    MOV  DX,' '
    INT  21H
    MOV  AH, 09H
    LEA  DX, Res3
    INT  21H
    MOV  DL, [OTHER]
    CALL COPE_DATA

    MOV  AH,4CH
    INT  21H

CSEG ENDS
END START