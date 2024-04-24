#include <klib.h>
#include <rtthread.h>
char *strchr(const char *s, int c) {
    while (*s != (char)c) {
        if (!*s++) {
            return NULL;
        }
    }
    return (char *)s;
}


char *strrchr(const char *s, int c) {
    char *last = NULL;
    do {
        if (*s == (char)c)
            last = (char *)s;
    } while (*s++);
    return last;
}


char *strstr(const char *haystack, const char *needle) {
  return rt_strstr(haystack, needle);
}
int is_space(char c) {
    return c == ' ';
}
long strtol(const char *restrict nptr, char **restrict endptr, int base) {
    int neg = 0;
    long val = 0;

    // Skip whitespace
    while (is_space(*nptr)) nptr++;

    // Check for optional sign
    if (*nptr == '+' || *nptr == '-') {
        neg = (*nptr == '-');
        nptr++;
    }

    // Check for optional "0x" prefix if base is 16
    if ((base == 0 || base == 16) && (nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X'))) {
        nptr += 2;
        base = 16;
    }

    // If no base provided, assume 10
    if (base == 0) base = 10;

    // Convert characters to value
    while (1) {
        int digit;

        if (*nptr >= '0' && *nptr <= '9') digit = *nptr - '0';
        else if (*nptr >= 'a' && *nptr <= 'z') digit = *nptr - 'a' + 10;
        else if (*nptr >= 'A' && *nptr <= 'Z') digit = *nptr - 'A' + 10;
        else break;

        if (digit >= base) break;

        val = val * base + digit;
        nptr++;
    }

    // Store end pointer if requested
    if (endptr) *endptr = (char *)nptr;

    // Apply sign and return result
    return (neg ? -val : val);
}


// long strtol(const char *restrict nptr, char **restrict endptr, int base){
//     int neg = 0;
//     long val = 0;

//     // Skip whitespace
//     while (isspace(*nptr)) nptr++;

//     // Check for optional sign
//     if (*nptr == '+' || *nptr == '-') {
//         neg = (*nptr == '-');
//         nptr++;
//     }

//     // Check for optional "0x" prefix if base is 16
//     if ((base == 0 || base == 16) && (nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X'))) {
//         nptr += 2;
//         base = 16;
//     }

//     // If no base provided, assume 10
//     if (base == 0) base = 10;

//     // Convert characters to value
//     while (1) {
//         int digit;

//         if (*nptr >= '0' && *nptr <= '9') digit = *nptr - '0';
//         else if (*nptr >= 'a' && *nptr <= 'z') digit = *nptr - 'a' + 10;
//         else if (*nptr >= 'A' && *nptr <= 'Z') digit = *nptr - 'A' + 10;
//         else break;

//         if (digit >= base) break;

//         val = val * base + digit;
//         nptr++;
//     }

//     // Store end pointer if requested
//     if (endptr) *endptr = (char *)nptr;

//     // Apply sign and return result
//     return (neg ? -val : val);
// }

char *strncat(char *dst, const char *src, size_t sz) {
    char *ptr = dst;
    while (*ptr) ptr++;  // 找到 dst 的末尾
    while (sz--) {
        if (!(*ptr = *src++)) return dst;  // 如果 src 结束，就提前返回
        ptr++;
    }
    *ptr = '\0';  // 确保 dst 结尾有 '\0'
    return dst;
}

