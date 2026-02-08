bits 64

global ft_strlen

section .text
; size_t ft_strlen(const char *str);
; rdi - pointer to the input string
; returns length of the string in rax
ft_strlen:
    xor rax, rax ; Initialize rax using xor
    jmp .loop
.loop:
    cmp BYTE [rdi + rax], 0 
    je .finish ; If s[rax] == '\0', jump to finish
    inc rax ; rax++
    jmp .loop
.finish:
    ret ; return rax