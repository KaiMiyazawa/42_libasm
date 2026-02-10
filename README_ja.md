# 42_libasm

[![42 School Project](https://img.shields.io/badge/42-Project-blue)](https://www.42.fr/)
[![Language](https://img.shields.io/badge/language-x86--64%20Assembly-orange)](https://www.nasm.us/)

> [English README](README.md)

x86-64アセンブリ言語（NASM構文）で実装された、C標準ライブラリの基本的な関数の包括的な実装です。このプロジェクトは、低レベルプログラミングスキル、システムアーキテクチャの深い理解、および呼び出し規約の遵守を実証しています。

---

## 📚 目次

- [概要](#概要)
- [学習目標](#学習目標)
- [実装された関数](#実装された関数)
- [技術仕様](#技術仕様)
- [はじめに](#はじめに)
- [使用方法](#使用方法)
- [テスト](#テスト)
- [技術リファレンス](#技術リファレンス)
- [プロジェクト構成](#プロジェクト構成)

---

## 🎯 概要

**libasm**は、純粋なx86-64アセンブリ言語を使用して基本的なCライブラリ関数を再実装することに焦点を当てた42 Schoolのプロジェクトです。この実装はSystem V ABI呼び出し規約に準拠し、現代のLinuxシステムと互換性があります。

### このプロジェクトの特別な点

- **低レベルマスタリー**: CPU命令の直接操作
- **システムプログラミング**: システムコール、レジスタ、メモリ管理の理解
- **エラー処理**: システムコールの適切なerrno管理
- **ABI準拠**: System V呼び出し規約への準拠
- **PIE互換性**: Position Independent Executableのサポート

---

## 🎓 学習目標

このプロジェクトを通じて、以下の専門知識を開発しました：

1. **x86-64アセンブリ言語**
   - レジスタの使用と管理（rax、rdi、rsi、rdxなど）
   - NASM構文とディレクティブ
   - メモリアドレッシングモード

2. **システムレベルプログラミング**
   - Linuxシステムコール（read、write）
   - errnoを使用したエラー処理
   - スタックのアライメントと管理

3. **呼び出し規約**
   - System V ABIパラメータの受け渡し
   - 戻り値の規約
   - スタックフレームの管理

4. **位置独立コード**
   - PLT（Procedure Linkage Table）の使用
   - PIE（Position Independent Executable）の互換性
   - 動的リンクに関する考慮事項

---

## 🛠 実装された関数

すべての関数は、C標準ライブラリの対応する関数の動作に一致するように実装されています：

| 関数 | 説明 | システムコール |
|----------|-------------|-------------|
| `ft_strlen` | 文字列の長さを計算 | - |
| `ft_strcpy` | ソースから宛先へ文字列をコピー | - |
| `ft_strcmp` | 2つの文字列を辞書順で比較 | - |
| `ft_write` | ファイルディスクリプタにデータを書き込む | あり |
| `ft_read` | ファイルディスクリプタからデータを読み取る | あり |
| `ft_strdup` | mallocで文字列を複製 | 間接的 |

### 関数シグネチャ

```c
size_t  ft_strlen(const char *s);
char   *ft_strcpy(char *dest, const char *src);
int     ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fildes, const void *buf, size_t nbyte);
ssize_t ft_read(int fildes, void *buf, size_t nbyte);
char   *ft_strdup(const char *s);
```

---

## 🔧 技術仕様

- **アセンブリ構文**: Intel（NASM）
- **アーキテクチャ**: x86-64（64ビット）
- **オブジェクト形式**: ELF64
- **呼び出し規約**: System V ABI
- **プラットフォーム**: Linux（Ubuntu/Debianでテスト済み）
- **PIEサポート**: あり（PLT経由）

---

## 🚀 はじめに

### 前提条件

- **Docker**（一貫した環境のために推奨）
- またはネイティブ環境：NASMアセンブラ、GCC、Make、Linux x86-64

### Dockerの使用（推奨）

```bash
# Dockerイメージをビルド
docker build --platform=linux/amd64 -t libasm .

# インタラクティブコンテナを実行
docker run --platform=linux/amd64 --rm -it -v "$PWD":/work libasm bash
```

### ネイティブビルド

必要なツールがインストールされていることを確認してください：
```bash
sudo apt-get install nasm gcc make
```

---

## 📖 使用方法

### ライブラリのビルド

```bash
make
```

これにより、実装されたすべての関数を含む静的ライブラリ`libasm.a`が作成されます。

### ビルド成果物のクリーンアップ

```bash
make clean   # オブジェクトファイルを削除
make fclean  # オブジェクトファイルとライブラリを削除
make re      # ゼロから再ビルド
```

### プログラムとのリンク

```bash
gcc your_program.c -L. -lasm -o your_program
./your_program
```

---

## 🧪 テスト

### クイックテスト

`main.c`に含まれる包括的なテストスイートを実行：

```bash
make test
```

これにより以下がテストされます：
- ✅ 標準ライブラリの動作との正確性
- ✅ エッジケース（空文字列、長い文字列、無効なファイルディスクリプタ）
- ✅ エラー処理とerrno値
- ✅ 戻り値の正確性

### libftとの比較

カスタムC実装と結果を比較するには：

```bash
# libftリポジトリをクローン（初回のみ）
git clone https://github.com/KaiMiyazawa/42_libft.git

# 比較テストを実行
make test_compare
```

これにより、以下との並列比較が実行されます：
- 標準Cライブラリ（libc）
- カスタムC実装（libft）
- アセンブリ実装（libasm）

### 手動テスト

個別の関数をテスト：

```bash
# 単一関数をコンパイル
nasm -f elf64 src/ft_strlen.s -o src/ft_strlen.o

# 単一関数でライブラリを作成
ar rcs libasm.a src/ft_strlen.o

# テスト
gcc -Wall -Wextra -Werror main.c -L. -lasm -o test
./test
```

### メモリリークテスト

```bash
make test
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test
```

---

## 📘 技術リファレンス

### System V ABIレジスタの使用

| レジスタ | 用途 | 呼び出し間で保持 |
|----------|---------|------------------------|
| **rdi** | 第1引数 | いいえ |
| **rsi** | 第2引数 | いいえ |
| **rdx** | 第3引数 | いいえ |
| **rcx** | 第4引数 | いいえ |
| **r8** | 第5引数 | いいえ |
| **r9** | 第6引数 | いいえ |
| **rax** | 戻り値 | いいえ |
| **rbx** | 汎用 | **はい** |
| **rsp** | スタックポインタ | **はい** |
| **rbp** | ベースポインタ | **はい** |

### 一般的なアセンブリ命令

#### データ移動
```nasm
mov dest, src     ; srcからdestへデータを移動
push src          ; スタックにsrcをプッシュ
pop dest          ; スタックからdestへポップ
```

#### 制御フロー
```nasm
cmp a, b         ; aとbを比較（a - bを計算し、フラグを設定）
je label         ; 等しい場合にジャンプ（ZF = 1）
jne label        ; 等しくない場合にジャンプ（ZF = 0）
jl label         ; 小さい場合にジャンプ（SF ≠ OF）
jg label         ; 大きい場合にジャンプ（ZF = 0かつSF = OF）
jle label        ; 以下の場合にジャンプ
jge label        ; 以上の場合にジャンプ
jmp label        ; 無条件ジャンプ
call function    ; 関数を呼び出し
ret              ; 関数から戻る
```

#### CPUフラグ
- **ZF**（ゼロフラグ）: 結果がゼロの場合にセット
- **SF**（符号フラグ）: 結果が負の場合にセット
- **CF**（キャリーフラグ）: 符号なしオーバーフロー時にセット
- **OF**（オーバーフローフラグ）: 符号付きオーバーフロー時にセット

### Linuxシステムコール

このプロジェクトで使用される一般的なシステムコール：

| システムコール番号（rax） | 関数 | 説明 |
|---------------------|----------|-------------|
| 0 | read | ファイルディスクリプタから読み取り |
| 1 | write | ファイルディスクリプタへ書き込み |
| 2 | open | ファイルを開く |
| 3 | close | ファイルディスクリプタを閉じる |
| 60 | exit | プロセスを終了 |

**使用パターン:**
```nasm
mov rax, syscall_number  ; システムコール番号を設定
mov rdi, arg1            ; 第1引数
mov rsi, arg2            ; 第2引数
mov rdx, arg3            ; 第3引数
syscall                  ; カーネルを呼び出し
```

### メモリ操作のルール

レジスタは高速なCPUストレージであり、RAMは低速なメインメモリです。

**有効な操作:**
```nasm
mov register, memory     ; ✅ メモリからロード
mov memory, register     ; ✅ メモリへストア
mov register, register   ; ✅ レジスタ間
```

**無効な操作:**
```nasm
mov memory, memory       ; ❌ メモリからメモリへは直接移動不可
```

**解決策:** 中間レジスタを使用：
```nasm
mov rax, [source]        ; レジスタへロード
mov [dest], rax          ; レジスタからストア
```

### PIEとPLTの説明

**PIE（Position Independent Executable）:**
- 実行可能ファイルを任意のメモリアドレスにロード可能
- セキュリティを強化（ASLR互換性）
- 現代のLinuxシステムで必須

**PLT（Procedure Linkage Table）:**
- 外部関数を呼び出すための間接参照メカニズム
- 共有ライブラリとの動的リンクを可能にする

**使用例:**
```nasm
call __errno_location wrt ..plt  ; PIE安全な外部関数呼び出し
```

`wrt ..plt`を使用しない場合、PIE実行可能ファイルのビルド時にリンカーが失敗する可能性があります。`wrt ..plt`ディレクティブは、呼び出しがPLTを経由することを保証し、位置独立性を実現します。

### エラー処理パターン

システムコールは失敗時に負のエラーコードを返します。errnoに変換：

```nasm
syscall               ; システムコールを実行
cmp rax, 0           ; エラーをチェック
jl .error            ; 負の場合（エラー）にジャンプ
ret                  ; 成功：戻り値はrax

.error:
    neg rax          ; エラーコードを正にする
    mov edi, eax     ; エラーコードを保存（32ビット）
    sub rsp, 8       ; スタックをアライメント（16バイトアライメントが必須）
    call __errno_location wrt ..plt
    add rsp, 8       ; スタックを復元
    mov [rax], edi   ; errnoを設定
    mov rax, -1      ; -1を返す
    ret
```

---

## 📁 プロジェクト構成

```
42_libasm/
├── src/               # アセンブリソースファイル
│   ├── ft_strlen.s
│   ├── ft_strcpy.s
│   ├── ft_strcmp.s
│   ├── ft_write.s
│   ├── ft_read.s
│   └── ft_strdup.s
├── main.c             # 包括的なテストスイート
├── Makefile           # ビルド自動化
├── Dockerfile         # 一貫した環境のためのコンテナセットアップ
├── .gitignore         # Git無視ルール
└── README.md          # このファイル
```

---

## 🎯 コードレビューのためのテストチェックリスト

### 文字列関数
- ✅ 空文字列（`""`）
- ✅ 非常に長い文字列（1000文字以上）
- ✅ 特殊文字を含む文字列
- ✅ NULLポインタの処理

### ft_write
- ✅ 標準出力への書き込み（fd 1）
- ✅ 有効なファイルディスクリプタへの書き込み
- ✅ 無効なファイルディスクリプタへの書き込み
- ✅ 戻り値の検証
- ✅ エラー時のerrnoの検証

### ft_read
- ✅ 標準入力からの読み取り（fd 0）
- ✅ 有効なファイルディスクリプタからの読み取り
- ✅ 無効なファイルディスクリプタからの読み取り
- ✅ 戻り値の検証
- ✅ エラー時のerrnoの検証

---

## 📝 ライセンス

このプロジェクトは42 Schoolのカリキュラムの一部です。学習目的での参照は自由に行ってください。

---

## 🙏 謝辞

- 包括的なカリキュラムを提供する42 School
- System V ABIドキュメント
- NASMドキュメントとコミュニティ

---

**⚙️ by KaiMiyazawaで作成**
