DATA SEGMENT
    ;----8251端口地址------
    IO8251_B EQU 290H ;8251数据口
    IO8251_A EQU 291H ;8251命令口/状态口

    ;----8254端口地址------
    IO8254_0 EQU 280H ;8254计数器0
    IO8254_1 EQU 281H ;8254计数器1
    IO8254_2 EQU 282H ;8254计数器2
    IO8254_K EQU 283H ;8254命令口地址

    ;----8255端口地址-------    
    IO8255_A EQU 288H ;8255 A端口
    IO8255_B EQU 289H ;8255 B端口
    IO8255_C EQU 28AH ;8255 C端口
    IO8255_K EQU 28BH ;8255命令口

    ;定义键盘对应的不同数据段，用于字符转换
    NUM DB 0FFH, 00H, 01H, 0FFH, 02H, 0FFH, 0FFH, 0FFH, 03H

    ;定义LED用于定义段码数据部分
    LED DB 71H,79H,5EH,39H,7CH,77H,6FH,7FH,07H,7DH,6DH,66H,4FH,5BH,06H,3FH

    ;喇叭固定频率发送的高低频频率表
    FRE DW 1048,988,880,784,698,660,588,524,524,494,440,392,347,330,294,262 

DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX

;---------------------------------初始化模块----------------------------
    ;初始化8255
    MOV DX, IO8255_K            ;8255命令口
    MOV AL, 10000000B           ;8255方式命令： A组0方式，A输出，C输出，B组0方式，B输出
    OUT DX, AL

    ;初始化8254
    MOV DX, IO8254_K            ;8254命令口
    MOV AL, 10010110B           ;8254的方式命令：选用通道2，只读写低字节，方式3，二进制
    OUT DX, AL
    MOV DX, IO8254_2            ;8254计数器2端口
    MOV AL, 52                  ;写入初值，设产生合适的接收时钟频率
    OUT DX, AL

    ;初始化8251
    MOV DX, IO8251_A            ;8251命令口/状态口
    XOR AL, AL                  ;空操作，向命令端口发送任意数

    MOV CX, 03
LOPD:    
    OUT DX, AL             
    PUSH CX
    MOV CX, 40H
LOPM:    
    LOOP LOPM
    POP CX
    LOOP LOPD

    MOV DX, IO8251_A        ;8251命令口/状态口
    MOV AL, 40H             ;01000000B，内部复位
    OUT DX, AL
    MOV AL, 01001110B       ;方式命令字：停止位数1位，无校验，字符长度8位，波特率因子为16
    OUT DX, AL
    MOV AL, 00100111B       ;工作命令字：请求发送、允许接收、数据终端准备好、允许发送
    OUT DX, AL
    
;------------------------------主函数接收数据处理模块------------------------------

RECEIVE:    
    MOV DX, IO8251_A        ;8251命令口/状态口
    IN AL, DX               ;8251状态字
    AND AL, 02H             ;是否准备好接收？（RxRDY=1?）
    JZ RECEIVE              ;未准备好，则等待
    MOV DX, IO8251_B        ;8251数据口
    IN AL, DX               ;从数据口读取数据（接收）
    MOV BL, AL              ;将读来的数据存入BL
    MOV BH, 0               ;BX中存的就是按下键的数值
    CALL DIP                ;显示
    CALL PIANO              ;发声
    CALL DELAY1             ;延时程序
    JMP RECEIVE             ;循环接收

;------------------------------------喇叭发声部分---------------------------------
PIANO PROC
    SHL BL, 1               ;逻辑左移，将原数值乘2
    MOV AX, 4240H
    MOV DX, 0FH             ;计算初值，被除数高16位存在DX中，低16位存在AX中，为1MHz
    DIV WORD PTR[FRE+BX]
    MOV BX, AX              ;商存入AX中，结果为应设置的初值大小

    ;初始化8254
    MOV DX, IO8254_K        ;8254命令口
    MOV AL, 00110110B       ;8254方式命令：选通道0、先低后高读写、方式3、二进制
    OUT DX, AL

    MOV DX, IO8254_0        ;8254计数器0端口
    MOV AX, BX              ;写计数初值，产生合适的输出频率
    OUT DX, AL              ;先低后高送数
    MOV AL, AH
    OUT DX, AL
    MOV DX, IO8255_K        ;8255 C端口

    MOV AL, 03H             ;00000011B，置PC1=1
    OUT DX, AL

    MOV AL,01H             ;PC0置1
    OUT DX,AL
    PUSH CX
    MOV CX, 15              ;延时
DEL:    
    LOOP DEL
    MOV AL, 02H              ;00000000B
    OUT DX, AL

    MOV AL,00H             ;PC0复位为0
    OUT DX,AL
    POP CX
    RET
PIANO ENDP

;------------------------------七段数码管显示数值----------------------------

DIP PROC                    
    MOV DX, IO8255_K            ;8255命令口
    MOV AL, 10000000B           ;8255方式命令： A组0方式，A输出，C输出，B组0方式，B输出
    OUT DX, AL                  ;写方式命令
    MOV DX, IO8255_A            ;8255 A端口
    MOV AL, LED[BX]             ;取出LED对应数码
    OUT DX, AL                  ;A端口输出，实现七段数码管显示数值
    RET
DIP ENDP

;-----------------------------------延时子程序1------------------------------
DELAY1 PROC
    PUSH CX
    MOV CX, 00FFH
D1:    LOOP D1
    POP CX
    RET
DELAY1 ENDP

;-----------------------------------延时子程序2--------------------------------
DELAY2 PROC
    PUSH CX
    MOV CX, 0FFFFH
D2:    LOOP D2
    POP CX
    RET
DELAY2 ENDP
CODE ENDS
    END START