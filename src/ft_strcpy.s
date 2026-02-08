bits 64

global ft_strcpy

section .text
; char *ft_strcpy(char *dest, const char *src);
; rdi - pointer to the destination string
; rsi - pointer to the source string
; returns pointer to the destination string in rax
ft_strcpy:
    xor rax, rax ; Initialize rax using xor
    jmp .loop
.loop:
    cmp BYTE [rsi + rax], 0 
    je .finish ; If s[rax] == '\0', jump to finish
    mov r8b, [rsi + rax] ; Load byte from source
    mov [rdi + rax], r8b ; Store byte to destination
    inc rax ; rax++
    jmp .loop
.finish:
    mov BYTE [rdi + rax], 0 ; Null-terminate destination string
    mov rax, rdi ; Move destination pointer to rax for return
    ret ; return rax