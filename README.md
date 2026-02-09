# 42_libasm

## build
```
docker build --platform=linux/amd64 -t libasm .
```

## run
```
docker run --platform=linux/amd64 --rm -it -v "$PWD":/work libasm bash
```

## test
```
nasm -f elf64 src/ft_strlen.s -o src/ft_strlen.o
ar rcs libasm.a src/ft_strlen.o
gcc -Wall -Wextra -Werror main.c -L. -lasm -o test_strlen
./test_strlen
```

## test (all)
```
make test
```

## compare with libft
```
# build libft shared library (once)
cd 42_libft && make CFLAGS="-Wall -Wextra -Werror -fPIC"
cc -shared -o libft.so *.o

# back to project root and run compare test
cd ..
make test_compare
```

## compare with libft
```
git clone git@github.com:KaiMiyazawa/42_libft.git
```


## MEMO
rdi: 1st argument
rsi: 2nd argument
rdx: 3rd argument
rax: return value

move: mov dest, src
push: push src
pop: pop dest
call: call function
ret: return

cmp: cmp a, b  ; a - b
je: je label   ; jump if equal (zero flag set)
jne: jne label ; jump if not equal (zero flag not set)
jl: jl label   ; jump if less (sign flag != overflow flag)
jg: jg label   ; jump if greater (zero flag clear and sign flag == overflow flag)
jle: jle label ; jump if less or equal (zero flag set or sign flag != overflow flag)
jge: jge label ; jump if greater or equal (sign flag == overflow flag)
jmp: jmp label  ; unconditional jump

ZF: zero flag
SF: sign flag
CF: carry flag
OF: overflow flag


syscall: call kernel function reeding rax with syscall number
	rax = 0 : read
	rax = 1 : write
	rax = 2 : open
	rax = 3 : close
	rax = 60 : exit




### ルール
レジスタは、CPU内の超高速メモリ領域。
メモリは、RAMなどの比較的低速なメモリ領域。

mov レジスタ, メモリ
mov メモリ, レジスタ
mov レジスタ, レジスタ
mov メモリ, メモリ  --- NG ---

移動したい値をレジスタに一旦移動してから、目的の場所に移動する。


System V ABIについて
System V ABI (Application Binary Interface) は、Unix系オペレーティングシステムで使用される関数呼び出し規約の一つです。
'calling convention'とも呼ばれ、関数が引数をどのように受け取り、戻り値をどのように返すか、またスタックの管理方法などを定義しています。
x86-64アーキテクチャにおけるSystem V ABIでは、関数の引数は以下のようにレジスタに渡されます。

## about 'call __errno_location wrt ..plt'
'wrt ..plt'についての説明です。
make test が PIE(Position Independent Executable: プログラムがメモリのどこに置かれても動作する実行ファイル) でリンクしている
なのに __errno_location の呼び方が PIE に合わない relocation になった

wrt ..plt を付けると「外部関数呼び出し＝PLT(Procedure Linkage Table: 外部ライブラリの関数を呼ぶための入り口)経由」になり、PIEでもリンクできる



## レビューメモ
- str*
	- 空の文字列
	- 非常に長い文字列
- write
	- stdoutでft_writeを試す
	- 開いているファイル記述子でft_writeを試す
	- 間違ったファイル記述子でft_writeを試す
	- 戻り値を確認する
- read
	- stdinでft_readを試す
	- 開いているファイル記述子でft_readを試す
	- 間違ったファイル記述子でft_readを試す
	- 戻り値を確認する

