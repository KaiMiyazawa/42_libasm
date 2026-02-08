bits 64

extern __errno_location

global ft_read

section .text
; ssize_t read(int fildes, void *buf, size_t nbyte);
; rdi - file descriptor
; rsi - pointer to the buffer
; rdx - number of bytes to read
; returns number of bytes read in rax, or -1 on error and sets errno

ft_read:
	; syscall: sys_read (0)
	mov rax, 0          ; syscall number for sys_read
	; xor rax, rax        ; alternative way to set rax to 0
	syscall              ; invoke operating system to do the read
	; 'syscall' uses rdi, rsi, rdx as arguments for read()
	; 'syscall' returns result in rax: number of bytes read or error code(negative)

	; #Using 'cmp'
	cmp rax, 0         ; compare return value with 0
	; how about 'rax - 0'?
	jl .error          ; if rax < 0, jump to error

	; # Using 'test'
	;; Check for error (return value in rax < 0) 
	;test rax, rax      ; set flags based on rax
	;; 
	;js .error          ; jump if rax < 0 (error)
	;; js: jump if sign flag is set (negative)
	
	ret                 ; return number of bytes read in rax

.error: ; rax <- -errno
	; On error, set errno to -rax
	neg rax             ; rax = -rax (make positive error code)
	mov edi, eax       ; escape error code in edi (int errno). int is 4 bytes. so use 'e'ax
	; Align stack to 16 bytes before call
	
	sub rsp, 8 ; align (System V ABI requires 16-byte alignment before calls)
	call __errno_location wrt ..plt ; get pointer to errno (PIE-safe)
	add rsp, 8 ; restore stack
	
	mov DWORD [rax], edi ; set errno to error code
	; errno is 'int', so store 4 bytes (DWORD)
	mov rax, -1        ; return -1 to indicate error
	ret
