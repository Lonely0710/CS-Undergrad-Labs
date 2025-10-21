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
    NUM DB 0FFH, 00H, 01H, 0FFH, 02H, 0FFH,0FFH, 0FFH, 03H

    ;定义LED用于定义段码数据部分
    LED DB 71H, 79H, 5EH, 39H, 7CH, 77H, 6FH, 7FH, 07H, 7DH, 6DH, 66H, 4FH, 5BH, 06H, 3FH

    ;定义乘的倍数
    MULT DB 4
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX

;---------------------------------初始化模块----------------------------
    ;初始化8255
    MOV DX, IO8255_K ;8255命令口
    MOV AL, 10001001B ;8255 A组使用方式0，A端口只输出，C端口只输入，B使用0方式，B选择输出
    OUT DX, AL            
    
    ;初始化8254
    MOV DX, IO8254_K
    MOV AL, 10010110B ;8254的方式命令：选用通道2、只读写低字节、使用方式3和二进制
    OUT DX, AL
    MOV DX, IO8254_2 ;8254计数器2端口
    MOV AL, 52 ;写入初值，产生合适的发送时钟频率
    OUT DX, AL
    
    ;初始化8251
    MOV DX, IO8251_A ;8251命令口
    XOR AL, AL ;空操作，向命令端口发送任意数

    MOV CX, 03
LOPD:    
    OUT DX, AL         
    PUSH CX
    MOV CX, 40H
LOOPM:    
    LOOP LOOPM
    POP CX
    LOOP LOPD

    MOV DX, IO8251_A 
    MOV AL, 40H              ;01000000B，内部复位
    OUT DX, AL
    MOV AL, 01001110B        ;方式命令字：停止位数1位，无校验，字符长度8位，波特率因子为16
    OUT DX, AL
    MOV AL, 00100111B        ;工作命令字：请求发送、允许接收、数据终端准备好、允许发送
    OUT DX, AL

;---------------------------读取键盘输入模块---------------------------------
GET:    
    CALL GETKEY            ;第一次获取键盘值
    MOV BL, AL              ;存列线状态
    MOV BH, AH              ;存行线状态

    CALL DELAY1             ;延时程序
    CALL GETKEY            ;第二次获取键盘值    
    CMP AL, BL
    JNZ GET                ;如果两次状态不相同，则认为存在干扰，转而重新获取键盘值，消除抖动
    CALL KEYUP            ;判断按键是否弹起
    CALL VALUE            ;得到键盘值对应的数值
    CALL DIP                ;显示数值
    CALL SEND            ;传输
    CALL DELAY1
    JMP GET

GETKEY PROC                ;获取键盘状态，得到键盘值
    MOV AH, 11111110B
SCAN:    
    MOV AL, AH
    MOV DX, IO8255_A        ;输出一行线为低
    OUT DX, AL
    MOV DX, IO8255_C
    IN AL, DX               ;读入列线状态
    OR AL, 11110000B        ;高四位置为1，低四位为列线状态
    CMP AL, 11111111B       ;此行有件按下吗？     
    JNE GEXIT               ;有键按下，退出
    ROL AH, 1               ;没有，循环左移，检查下一行
    JMP SCAN
GEXIT:    
    RET
GETKEY ENDP

KEYUP PROC                ;查看当前按键是否弹起
NO:    
    MOV AL, AH
    MOV DX, IO8255_A
    OUT DX, AL
    MOV DX, IO8255_C
    IN AL, DX
    OR AL, 11110000B
    CMP AL, 11111111B       ;相同说明按键已弹起，此时没有键按下
    JE KEXIT
    JMP NO
KEXIT:   
    RET
KEYUP ENDP

;--------------------------根据按键得到当前按下的数值-----------------------

VALUE PROC                ;获取键盘值对应的数
    NOT BL                ;取反操作，将列线状态取反
    NOT BH                ;取反操作，将行线状态取反
    MOV AH, BH              
    MOV AL, BL              
    MOV BL, AH
    MOV BH, 0
    MOV AH, NUM[BX]       ;获取行数，取反后BX的值一般只有1、2、4、8这几种可能
    MOV BL, AL
    MOV AL, NUM[BX]       ;获取列数
    MOV BH, AH            
    MOV BL, AL            ;将行数和列数分别存入BH和BL
    MOV AL, AH
    MOV AH, 0
    MUL MULT              ;行数×4,乘数放在AX中，结果存入AX中
    ADD AL, BL            ;将结果加上列数，得到最终数值，最终结果存在AL中
    MOV BL, AL            ;最终结果存入BL中
    MOV BH, 0             ;将BX高位置为0，至此，BX中存的是16位按键数字对应的数值
    RET
VALUE ENDP

;------------------------------七段数码管显示数值-----------------------------
DIP PROC                    
    MOV DX, IO8255_B        
    MOV AL, LED[BX]
    OUT DX, AL           ;通过8255的B端口输出到数码管，实现数码管显示
    RET
DIP ENDP

;--------------------------传输数值子程序-------------------------
SEND PROC                ;实现串行通信
NEXT:    
    MOV DX, IO8251_A     ;8251状态口
    IN AL, DX            ;读入当前8251状态
    TEST AL, 01H         ;检查发送是否准备好？（TxRDY=1？）
    JZ NEXT              ;未准备好，则等待
    MOV AL, BL
    MOV DX, IO8251_B     ;8251数据口
    OUT DX, AL           ;把要发送的数送给8251的数据口
    RET
SEND ENDP

;----------------------------延时子程序1---------------------------
DELAY1 PROC
    PUSH CX
    MOV CX, 00FFH
D1:    LOOP D1
    POP CX
    RET
DELAY1 ENDP

;-------------------延时子程序2--------------------
DELAY2 PROC
    PUSH CX
    MOV CX, 0FFFH
D2:    LOOP D2
    POP CX
    RET
DELAY2 ENDP

CODE ENDS
    END START