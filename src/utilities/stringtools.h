#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include "serial/serial.h"

void reverse(char str[], size_t length)
{
    size_t start = 0;
    size_t end = length - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        ++start;
        --end;
    }
}

size_t itoa(int num, char str[], size_t base)
{
    size_t i = 0;

    int isNegative = 0;
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    if (num == 0)
    {
        str[i++] = '0';
    }

    while (num != 0)
    {
        size_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
    {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str, i);

    return i;
}

size_t ultoa(uint64_t num, char str[], size_t base)
{
    size_t i = 0;

    if (num == 0)
    {
        str[i++] = '0';
    }

    while (num != 0)
    {
        size_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    str[i] = '\0';

    reverse(str, i);

    return i;
}
size_t __ultoua(uint64_t num, char str[], size_t base)
{
    size_t i = 0;

    if (num == 0)
    {
        str[i++] = '0';
    }

    while (num != 0)
    {
        size_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }

    str[i] = '\0';

    reverse(str, i);

    return i;
}


size_t ltoa(int64_t num, char str[], size_t base)
{
    size_t i = 0;

    int isNegative = 0;
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    if (num == 0)
    {
        str[i++] = '0';
    }

    while (num != 0)
    {
        size_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
    {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str, i);

    return i;
}
