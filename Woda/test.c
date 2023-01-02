
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

size_t icbrt(unsigned long x)
{
    int s;
    unsigned int y;
    unsigned long b;

    y = 0;
    for (s = 63; s >= 0; s -= 3)
    {
        y += y;
        b = 3 * y * ((unsigned long) y + 1) + 1;
        if ((x >> s) >= b)
        {
            x -= b << s;
            y++;
        }
    }
    return y;
}

int main(int argc, char **argv)
{
    long x = 10000000000000;

    printf("icbrt: %ld - cbrt: %ld", icbrt(x), (size_t)cbrt((double)x));
}