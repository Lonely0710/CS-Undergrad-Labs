code SEGMENT
    assume cs:code
start:
    mov dx,283h;命令口地址
    mov al,00110100B;计数器0，先低后高，方式2,二进制
    out dx,al

    mov dx,280H;设置定时器0
    mov ax,1000;设计初值为1000
    out dx,al;先送低字节，后送高字节
    mov al,ah
    out dx,al

    mov dx,283h;命令口地址
    mov al,01110110B;计数器1，先低后高，方式3，二进制
    out dx,al

    mov dx,281H;设置定时器1
    mov ax,1000;设计初值为1000显示led 0010h显示波形
    out dx,al;先送低字节，后送高字节
    mov al,ah
    out dx,al

    code ENDS
    end start