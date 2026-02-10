# 42_libasm

[![42 School Project](https://img.shields.io/badge/42-Project-blue)](https://www.42.fr/)
[![Language](https://img.shields.io/badge/language-x86--64%20Assembly-orange)](https://www.nasm.us/)

> [日本語版のREADMEはこちら / Japanese README](README_ja.md)

A comprehensive implementation of essential C standard library functions in x86-64 assembly language (NASM syntax). This project demonstrates low-level programming skills, deep understanding of system architecture, and adherence to calling conventions.

---

## 📚 Table of Contents

- [Overview](#overview)
- [Learning Objectives](#learning-objectives)
- [Implemented Functions](#implemented-functions)
- [Technical Specifications](#technical-specifications)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Testing](#testing)
- [Technical Reference](#technical-reference)
- [Project Structure](#project-structure)

---

## 🎯 Overview

**libasm** is a 42 School project focused on recreating fundamental C library functions using pure x86-64 assembly language. The implementation follows the System V ABI calling convention and is compatible with modern Linux systems.

### What Makes This Project Special?

- **Low-level mastery**: Direct CPU instruction manipulation
- **System programming**: Understanding of syscalls, registers, and memory management
- **Error handling**: Proper errno management for system calls
- **ABI compliance**: Adherence to System V calling conventions
- **PIE compatibility**: Position Independent Executable support

---

## 🎓 Learning Objectives

Through this project, I developed expertise in:

1. **x86-64 Assembly Language**
   - Register usage and management (rax, rdi, rsi, rdx, etc.)
   - NASM syntax and directives
   - Memory addressing modes

2. **System-level Programming**
   - Linux syscalls (read, write)
   - Error handling with errno
   - Stack alignment and management

3. **Calling Conventions**
   - System V ABI parameter passing
   - Return value conventions
   - Stack frame management

4. **Position Independent Code**
   - PLT (Procedure Linkage Table) usage
   - PIE (Position Independent Executable) compatibility
   - Dynamic linking considerations

---

## 🛠 Implemented Functions

All functions are implemented to match the behavior of their C standard library counterparts:

| Function | Description | System Call |
|----------|-------------|-------------|
| `ft_strlen` | Calculate string length | - |
| `ft_strcpy` | Copy string from source to destination | - |
| `ft_strcmp` | Compare two strings lexicographically | - |
| `ft_write` | Write data to file descriptor | Yes |
| `ft_read` | Read data from file descriptor | Yes |
| `ft_strdup` | Duplicate string with malloc | Indirect |

### Function Signatures

```c
size_t  ft_strlen(const char *s);
char   *ft_strcpy(char *dest, const char *src);
int     ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fildes, const void *buf, size_t nbyte);
ssize_t ft_read(int fildes, void *buf, size_t nbyte);
char   *ft_strdup(const char *s);
```

---

## 🔧 Technical Specifications

- **Assembly Syntax**: Intel (NASM)
- **Architecture**: x86-64 (64-bit)
- **Object Format**: ELF64
- **Calling Convention**: System V ABI
- **Platform**: Linux (tested on Ubuntu/Debian)
- **PIE Support**: Yes (via PLT)

---

## 🚀 Getting Started

### Prerequisites

- **Docker** (recommended for consistent environment)
- Or native: NASM assembler, GCC, Make, Linux x86-64

### Using Docker (Recommended)

```bash
# Build the Docker image
docker build --platform=linux/amd64 -t libasm .

# Run interactive container
docker run --platform=linux/amd64 --rm -it -v "$PWD":/work libasm bash
```

### Native Build

Ensure you have the required tools:
```bash
sudo apt-get install nasm gcc make
```

---

## 📖 Usage

### Build the Library

```bash
make
```

This creates `libasm.a`, a static library containing all implemented functions.

### Clean Build Artifacts

```bash
make clean   # Remove object files
make fclean  # Remove object files and library
make re      # Rebuild from scratch
```

### Link with Your Program

```bash
gcc your_program.c -L. -lasm -o your_program
./your_program
```

---

## 🧪 Testing

### Quick Test

Run the comprehensive test suite included in `main.c`:

```bash
make test
```

This tests:
- ✅ Correctness against standard library behavior
- ✅ Edge cases (empty strings, long strings, invalid file descriptors)
- ✅ Error handling and errno values
- ✅ Return value accuracy

### Compare with libft

To compare results with a custom C implementation:

```bash
# Clone the libft repository (one-time setup)
git clone https://github.com/KaiMiyazawa/42_libft.git

# Run comparison tests
make test_compare
```

This performs side-by-side comparison with:
- Standard C library (libc)
- Custom C implementation (libft)
- Assembly implementation (libasm)

### Manual Testing

Test individual functions:

```bash
# Compile single function
nasm -f elf64 src/ft_strlen.s -o src/ft_strlen.o

# Create library with single function
ar rcs libasm.a src/ft_strlen.o

# Test it
gcc -Wall -Wextra -Werror main.c -L. -lasm -o test
./test
```

### Memory Leak Testing

```bash
make test
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test
```

---

## 📘 Technical Reference

### System V ABI Register Usage

| Register | Purpose | Preserved Across Calls |
|----------|---------|------------------------|
| **rdi** | 1st argument | No |
| **rsi** | 2nd argument | No |
| **rdx** | 3rd argument | No |
| **rcx** | 4th argument | No |
| **r8** | 5th argument | No |
| **r9** | 6th argument | No |
| **rax** | Return value | No |
| **rbx** | General purpose | **Yes** |
| **rsp** | Stack pointer | **Yes** |
| **rbp** | Base pointer | **Yes** |

### Common Assembly Instructions

#### Data Movement
```nasm
mov dest, src     ; Move data from src to dest
push src          ; Push src onto stack
pop dest          ; Pop from stack to dest
```

#### Control Flow
```nasm
cmp a, b         ; Compare a and b (compute a - b, set flags)
je label         ; Jump if equal (ZF = 1)
jne label        ; Jump if not equal (ZF = 0)
jl label         ; Jump if less (SF ≠ OF)
jg label         ; Jump if greater (ZF = 0 and SF = OF)
jle label        ; Jump if less or equal
jge label        ; Jump if greater or equal
jmp label        ; Unconditional jump
call function    ; Call function
ret              ; Return from function
```

#### CPU Flags
- **ZF** (Zero Flag): Set when result is zero
- **SF** (Sign Flag): Set when result is negative
- **CF** (Carry Flag): Set on unsigned overflow
- **OF** (Overflow Flag): Set on signed overflow

### Linux Syscalls

Common syscalls used in this project:

| Syscall Number (rax) | Function | Description |
|---------------------|----------|-------------|
| 0 | read | Read from file descriptor |
| 1 | write | Write to file descriptor |
| 2 | open | Open file |
| 3 | close | Close file descriptor |
| 60 | exit | Exit process |

**Usage Pattern:**
```nasm
mov rax, syscall_number  ; Set syscall number
mov rdi, arg1            ; First argument
mov rsi, arg2            ; Second argument
mov rdx, arg3            ; Third argument
syscall                  ; Invoke kernel
```

### Memory Operation Rules

Registers are high-speed CPU storage; RAM is slower main memory.

**Valid Operations:**
```nasm
mov register, memory     ; ✅ Load from memory
mov memory, register     ; ✅ Store to memory
mov register, register   ; ✅ Register to register
```

**Invalid Operations:**
```nasm
mov memory, memory       ; ❌ Cannot move memory to memory directly
```

**Solution:** Use an intermediate register:
```nasm
mov rax, [source]        ; Load to register
mov [dest], rax          ; Store from register
```

### PIE and PLT Explained

**PIE (Position Independent Executable):**
- Executable can be loaded at any memory address
- Enhances security (ASLR compatibility)
- Required for modern Linux systems

**PLT (Procedure Linkage Table):**
- Indirection mechanism for calling external functions
- Enables dynamic linking with shared libraries

**Usage Example:**
```nasm
call __errno_location wrt ..plt  ; PIE-safe external function call
```

Without `wrt ..plt`, the linker may fail when building PIE executables. The `wrt ..plt` directive ensures the call goes through the PLT, making it position-independent.

### Error Handling Pattern

System calls return negative error codes on failure. Convert to errno:

```nasm
syscall               ; Make system call
cmp rax, 0           ; Check for error
jl .error            ; Jump if negative (error)
ret                  ; Success: return value in rax

.error:
    neg rax          ; Make error code positive
    mov edi, eax     ; Save error code (32-bit)
    sub rsp, 8       ; Align stack (16-byte alignment required)
    call __errno_location wrt ..plt
    add rsp, 8       ; Restore stack
    mov [rax], edi   ; Set errno
    mov rax, -1      ; Return -1
    ret
```

---

## 📁 Project Structure

```
42_libasm/
├── src/               # Assembly source files
│   ├── ft_strlen.s
│   ├── ft_strcpy.s
│   ├── ft_strcmp.s
│   ├── ft_write.s
│   ├── ft_read.s
│   └── ft_strdup.s
├── main.c             # Comprehensive test suite
├── Makefile           # Build automation
├── Dockerfile         # Container setup for consistent environment
├── .gitignore         # Git ignore rules
└── README.md          # This file
```

---

## 🎯 Testing Checklist for Code Review

### String Functions
- ✅ Empty strings (`""`)
- ✅ Very long strings (1000+ characters)
- ✅ Strings with special characters
- ✅ NULL pointer handling

### ft_write
- ✅ Write to stdout (fd 1)
- ✅ Write to valid file descriptor
- ✅ Write to invalid file descriptor
- ✅ Verify return value
- ✅ Verify errno on error

### ft_read
- ✅ Read from stdin (fd 0)
- ✅ Read from valid file descriptor
- ✅ Read from invalid file descriptor
- ✅ Verify return value
- ✅ Verify errno on error

---

## 📝 License

This project is part of the 42 School curriculum. Feel free to reference for learning purposes.

---

## 🙏 Acknowledgments

- 42 School for the comprehensive curriculum
- System V ABI documentation
- NASM documentation and community

---

**Made with ⚙️ by KaiMiyazawa**

