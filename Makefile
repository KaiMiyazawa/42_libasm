# Static library output
NAME = libasm.a

# Mandatory sources (64-bit NASM, Intel syntax)
SRCS = src/ft_strlen.s src/ft_strcpy.s src/ft_strcmp.s src/ft_write.s src/ft_read.s src/ft_strdup.s
OBJS = $(SRCS:.s=.o)

#BONUS_SRCS = 
#BONUS_OBJS = $(BONUS_SRCS:.s=.o) 

# Assembler and output format
ASM = nasm
# for Linux
# elf : Extensible and Linkable Format
# A standard format for Linux/BSD OSs.
ASMFLAGS = -f elf64
# for macOS
#ASMFLAGS = -f macho64

RM = rm -rf
#MKDIR = mkdir -p

#ifdef WITH_BONUS
#	SRCS += BONUS_SRCS
#	OBJS += BONUS_OBJS
#endif

# Default build
all: $(NAME)

# Build libasm.a and run basic tests in main.c
test: all
	@gcc -Wall -Wextra -Werror -o test main.c $(NAME)
	@./test

# Build libft shared lib and run comparison tests (libft + libc)
test_compare: all
	@cd 42_libft && make CFLAGS="-Wall -Wextra -Werror -fPIC"
	@cc -shared -o 42_libft/libft.so 42_libft/*.o
	@gcc -Wall -Wextra -Werror -DUSE_LIBFT_COMPARE -o test_compare main.c $(NAME) -ldl
	@./test_compare

# Assemble .s -> .o
.s.o:
	$(ASM) $(ASMFLAGS) $< -o $@

# Archive objects into static library
$(NAME): $(OBJS)
	ar rcs $@ $^

# Remove objects
clean:
	$(RM) $(OBJS)

# Remove objects and library
fclean: clean
	$(RM) $(NAME)

# Rebuild from scratch
re: fclean all

#bonus:
#	@make WITH_BONUS=1 ${NAME}

#COLORS
#RED = \033[1;31m
#GREEN = \033[1;32m
#YELLOW = \033[1;33m
#SKY = \033[1;36m
#DEFAULT = \033[0m

.PHONY: all clean fclean re bonus test test_compare
