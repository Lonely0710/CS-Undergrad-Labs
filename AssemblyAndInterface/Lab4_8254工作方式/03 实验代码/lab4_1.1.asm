code SEGMENT
    ASSUME cs:code
start:
    mov dx,283h;命令口地址
    mov al,00110100B;计数器0，先低后高，方式2，二进制
    out dx,al

    mov dx,280H
    mov ax,1000;计数初值为1000
    out dx,al;先送入低字节数据
    mov al,ah
    out dx,al;再送入高字节数据

    code ends
    end start
