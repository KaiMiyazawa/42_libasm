bits 64

extern ft_strlen
extern ft_strcpy
extern malloc

global ft_strdup

section .text
; char *ft_strdup(const char *str);
; rdi - pointer to the input string
; returns pointer to duplicated string in rax, or NULL on error
ft_strdup:
	push rdi              ; Save original pointer to str
	; 'call' uses resisters. So you need use 'push' to save rdi.
	call ft_strlen        ; Call ft_strlen to get length of str
	; ft_strlen returns length in rax
	; rax = length of str
	mov rdi, rax        ; Move length to rdi for malloc argument
	inc rdi             ; rdi = length + 1 (for null terminator)
	; rdi - size_t size
	call malloc wrt ..plt ; Call malloc to allocate memory (PIE-safe)
	; malloc uses rdi as argument (size)
	; malloc returns pointer to allocated memory in rax
	
	cmp rax, 0          ; Check if malloc returned NULL
	je .error    ; If NULL, jump to error handling
	
	; test rax, rax      ; alternative way to check for NULL
	; jz .error          ; Jump if zero (NULL)
	
	mov rdi, rax		; Move pointer to allocated memory to rdi for ft_strcpy
	pop rsi             ; Restore original pointer to str into rsi
	; rsi - const char *str (source string)
	; rdi - char *dest (destination buffer)

	sub rsp, 8 ; align stack before call
	call ft_strcpy      ; Call ft_strcpy to copy str to allocated memory
	add rsp, 8 ; restore stack
	
	; ft_strcpy returns pointer to destination in rax
	ret                  ; Return pointer to duplicated string in rax

.error: ; rax <- 0 == NULL
	add rsp, 8        ; Clean up stack (pop rdi)
	ret	
