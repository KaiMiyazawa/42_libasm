// Simple test runner for libasm mandatory functions
// Focus: correctness, errno behavior, and libc/libft comparison
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

size_t ft_strlen(const char *s);
char *ft_strcpy(char *dest, const char *src);
int ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fildes, const void *buf, size_t nbyte);
ssize_t ft_read(int fildes, void *buf, size_t nbyte);
char *ft_strdup(const char *s);
#ifdef USE_LIBFT_COMPARE
#include <dlfcn.h>
#endif

// Minimal test accounting
static int g_total = 0;
static int g_failed = 0;

static void record_result(const char *label, int ok)
{
    g_total++;
    if (!ok)
    {
        g_failed++;
        printf("[FAIL] %s\n", label);
    }
    else
    {
        printf("[OK]   %s\n", label);
    }
}

// ---- basic unit tests ----
static void test_strlen(void)
{
    record_result("ft_strlen(\"abc\") == 3", ft_strlen("abc") == 3);
    record_result("ft_strlen(\"\") == 0", ft_strlen("") == 0);
    record_result("ft_strlen(\"ab\\0cd\") == 2", ft_strlen("ab\0cd") == 2);
    record_result("ft_strlen vs strlen", ft_strlen("Hello, World!") == strlen("Hello, World!"));
}

static void test_strcpy(void)
{
    char dest[64];
    char dest2[64];

    memset(dest, 0xAA, sizeof(dest));
    memset(dest2, 0xAA, sizeof(dest2));
    record_result("ft_strcpy basic",
                  ft_strcpy(dest, "Hello") == dest && strcmp(dest, "Hello") == 0);

    ft_strcpy(dest2, "");
    record_result("ft_strcpy empty",
                  strcmp(dest2, "") == 0);
}

static void test_strcmp(void)
{
    record_result("ft_strcmp equal", ft_strcmp("abc", "abc") == 0);
    record_result("ft_strcmp less", ft_strcmp("abc", "abd") < 0);
    record_result("ft_strcmp greater", ft_strcmp("abd", "abc") > 0);
    record_result("ft_strcmp prefix short", ft_strcmp("abc", "abcd") < 0);
    record_result("ft_strcmp prefix long", ft_strcmp("abcd", "abc") > 0);

    {
        int a = ft_strcmp("Hello", "World");
        int b = strcmp("Hello", "World");
        int sa = (a > 0) - (a < 0);
        int sb = (b > 0) - (b < 0);
        record_result("ft_strcmp vs strcmp (sign)",
                      sa == sb);
    }

    {
        unsigned char u1[] = {0x80, 0x00};
        unsigned char u2[] = {0x00, 0x00};
        unsigned char u3[] = {0xFF, 0x00};
        unsigned char u4[] = {0x01, 0x00};
        record_result("ft_strcmp non-ASCII 0x80 vs 0x00",
                      ft_strcmp((char *)u1, (char *)u2) == strcmp((char *)u1, (char *)u2));
        record_result("ft_strcmp non-ASCII 0xFF vs 0x01",
                      ft_strcmp((char *)u3, (char *)u4) == strcmp((char *)u3, (char *)u4));
    }
}

static void test_write(void)
{
    int fds[2];
    char buf[16] = {0};

    if (pipe(fds) != 0)
    {
        record_result("pipe for write", 0);
        return;
    }

    record_result("ft_write returns correct length",
                  ft_write(fds[1], "abc", 3) == 3);
    record_result("read back after ft_write",
                  read(fds[0], buf, sizeof(buf)) == 3 && memcmp(buf, "abc", 3) == 0);

    close(fds[0]);
    close(fds[1]);

    errno = 0;
    record_result("ft_write invalid fd sets errno",
                  ft_write(-1, "x", 1) == -1 && errno == EBADF);
}

static void test_read(void)
{
    int fds[2];
    char buf[16];

    if (pipe(fds) != 0)
    {
        record_result("pipe for read", 0);
        return;
    }

    write(fds[1], "xyz", 3);
    memset(buf, 0, sizeof(buf));
    record_result("ft_read returns correct length",
                  ft_read(fds[0], buf, sizeof(buf)) == 3);
    record_result("ft_read contents",
                  memcmp(buf, "xyz", 3) == 0);

    close(fds[0]);
    close(fds[1]);

    errno = 0;
    record_result("ft_read invalid fd sets errno",
                  ft_read(-1, buf, 1) == -1 && errno == EBADF);

    memset(buf, 0xAA, sizeof(buf));
    record_result("ft_read zero length returns 0",
                  ft_read(0, buf, 0) == 0);
}

static void test_strdup(void)
{
    char *dup = ft_strdup("hello");
    record_result("ft_strdup basic",
                  dup != NULL && strcmp(dup, "hello") == 0);
    free(dup);

    dup = ft_strdup("");
    record_result("ft_strdup empty",
                  dup != NULL && strcmp(dup, "") == 0);
    free(dup);
}

// ---- optional comparisons ----
#ifdef USE_LIBFT_COMPARE
static void compare_with_libft(void)
{
    void *handle = dlopen("42_libft/libft.so", RTLD_LAZY);
    if (!handle)
    {
        printf("[SKIP] libft.so not found: %s\n", dlerror());
        return;
    }

    size_t (*libft_strlen)(const char *) = dlsym(handle, "ft_strlen");
    char *(*libft_strdup)(const char *) = dlsym(handle, "ft_strdup");

    if (!libft_strlen || !libft_strdup)
    {
        printf("[SKIP] libft symbols not found\n");
        dlclose(handle);
        return;
    }

    record_result("libft ft_strlen compare",
                  ft_strlen("compare") == libft_strlen("compare"));

    {
        char *a = ft_strdup("libft");
        char *b = libft_strdup("libft");
        record_result("libft ft_strdup compare",
                      a && b && strcmp(a, b) == 0);
        free(a);
        free(b);
    }

    dlclose(handle);
}
#endif

static void compare_with_libc(void)
{
    {
        const char *s = "libc_compare";
        record_result("libc strlen compare",
                      ft_strlen(s) == strlen(s));
    }

    {
        char *a = ft_strdup("libc");
        char *b = strdup("libc");
        record_result("libc strdup compare",
                      a && b && strcmp(a, b) == 0);
        free(a);
        free(b);
    }

    {
        char dst_a[32];
        char dst_b[32];
        const char *src = "copy";
        memset(dst_a, 0xAA, sizeof(dst_a));
        memset(dst_b, 0xBB, sizeof(dst_b));
        record_result("libc strcpy compare",
                      ft_strcpy(dst_a, src) == dst_a &&
                      strcpy(dst_b, src) == dst_b &&
                      strcmp(dst_a, dst_b) == 0);
    }

    {
        int a = ft_strcmp("Hello", "World");
        int b = strcmp("Hello", "World");
        int sa = (a > 0) - (a < 0);
        int sb = (b > 0) - (b < 0);
        record_result("libc strcmp compare (sign)",
                      sa == sb);
    }

    {
        int fds1[2];
        int fds2[2];
        char buf1[8] = {0};
        char buf2[8] = {0};
        int e1;
        int e2;

        if (pipe(fds1) != 0 || pipe(fds2) != 0)
        {
            record_result("libc write compare setup", 0);
        }
        else
        {
            errno = 0;
            ssize_t r1 = ft_write(fds1[1], "abc", 3);
            e1 = errno;

            errno = 0;
            ssize_t r2 = write(fds2[1], "abc", 3);
            e2 = errno;

            record_result("libc write compare (ret)",
                          r1 == r2 && e1 == e2);

            read(fds1[0], buf1, sizeof(buf1));
            read(fds2[0], buf2, sizeof(buf2));
            record_result("libc write compare (data)",
                          memcmp(buf1, buf2, 3) == 0);
        }

        if (fds1[0] >= 0) { close(fds1[0]); }
        if (fds1[1] >= 0) { close(fds1[1]); }
        if (fds2[0] >= 0) { close(fds2[0]); }
        if (fds2[1] >= 0) { close(fds2[1]); }
    }

    {
        int fds1[2];
        int fds2[2];
        char buf1[8] = {0};
        char buf2[8] = {0};
        int e1;
        int e2;

        if (pipe(fds1) != 0 || pipe(fds2) != 0)
        {
            record_result("libc read compare setup", 0);
        }
        else
        {
            write(fds1[1], "xyz", 3);
            write(fds2[1], "xyz", 3);

            errno = 0;
            ssize_t r1 = ft_read(fds1[0], buf1, sizeof(buf1));
            e1 = errno;

            errno = 0;
            ssize_t r2 = read(fds2[0], buf2, sizeof(buf2));
            e2 = errno;

            record_result("libc read compare (ret)",
                          r1 == r2 && e1 == e2);
            record_result("libc read compare (data)",
                          memcmp(buf1, buf2, 3) == 0);
        }

        if (fds1[0] >= 0) { close(fds1[0]); }
        if (fds1[1] >= 0) { close(fds1[1]); }
        if (fds2[0] >= 0) { close(fds2[0]); }
        if (fds2[1] >= 0) { close(fds2[1]); }
    }

    {
        errno = 0;
        ssize_t r1 = ft_write(-1, "x", 1);
        int e1 = errno;
        errno = 0;
        ssize_t r2 = write(-1, "x", 1);
        int e2 = errno;
        record_result("libc write EBADF compare",
                      r1 == r2 && e1 == e2 && e1 == EBADF);
    }

    {
        char buf[4];
        errno = 0;
        ssize_t r1 = ft_read(-1, buf, 1);
        int e1 = errno;
        errno = 0;
        ssize_t r2 = read(-1, buf, 1);
        int e2 = errno;
        record_result("libc read EBADF compare",
                      r1 == r2 && e1 == e2 && e1 == EBADF);
    }
}

int main(int argc, char **argv)
{
    (void)argv;
    (void)argc;

    // Basic correctness checks
    printf("== basic tests ==\n");
    test_strlen();
    test_strcpy();
    test_strcmp();
    test_write();
    test_read();
    test_strdup();

#ifdef USE_LIBFT_COMPARE
    // Compare libasm behavior with libft implementations
    printf("\n== compare with libft ==\n");
    printf("Build libft.so first:\n");
    printf("  cd 42_libft && make CFLAGS=\"-Wall -Wextra -Werror -fPIC\"\n");
    printf("  cc -shared -o libft.so *.o\n\n");
    compare_with_libft();
#endif

    // Compare libasm behavior with libc equivalents
    printf("\n== compare with libc ==\n");
    compare_with_libc();

    // Summary + exit status for CI style usage
    printf("\nSummary: %d / %d passed\n", g_total - g_failed, g_total);
    return (g_failed == 0) ? 0 : 1;
}
