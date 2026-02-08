bits 64

global ft_strcmp

section .text
; int ft_strcmp(const char *s1, const char *s2);
; rdi - pointer to the first string
; rsi - pointer to the second string
; returns difference between first differing characters in rax
; strcmp compares bytes as unsigned char, then returns (u8)s1 - (u8)s2
ft_strcmp:
	xor rax, rax ; i = 0 (64-bit index). Writing EAX/RAX clears upper bits.
	xor r8b, r8b ; r8b = 0 (only low 8 bits). Upper 56 bits of r8 are untouched.
	xor r9b, r9b ; r9b = 0 (only low 8 bits). Upper 56 bits of r9 are untouched.
	jmp .loop
.loop:
	; Load 1 byte from each string.
	; r8b/r9b are 8-bit low parts (1 byte = 8 bits) of r8/r9.
	mov r8b, [rdi + rax] ; c1 = *(u8*)(s1 + i)
	mov r9b, [rsi + rax] ; c2 = *(u8*)(s2 + i)
	
	; If bytes differ, we are done.
	; strcmp is defined on unsigned bytes, not signed chars.
	cmp r8b, r9b
	jne .finish
	
	; If c1 == 0 and c1 == c2, then both strings ended -> equal.
	cmp r8b, 0
	je .finish
	
	;cmp r9b, 0        ; Check for null terminator
	;je .finish      ; If null terminator, jump to finish
	
	inc rax           ; i++
	jmp .loop
.finish:
	; Zero-extend to 32-bit ints before subtracting.
	; movzx clears upper bits, so values are 0..255 (unsigned char).
	movzx eax, r8b   ; eax = (unsigned char)c1, zero-extends into rax
	movzx ecx, r9b   ; ecx = (unsigned char)c2
	sub eax, ecx     ; return (int)c1 - (int)c2 in eax/rax
	
	;sub r8b, r9b   ; if you didn't use 'movzx', this gives wrong result
	;movsx rax, r8b ; sign-extend the result to 64 bits
	
	ret

; if you didn't use 'movzx'...

; 0x00(0) - 0x01(1) = 0xFF(-1? 255?)
; ft_strcmp deals with unsigned char values.