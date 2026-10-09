#ident "synthetic directive marker"
#pragma outside
asm(".section .text" "\n");
int helper(void);
int target(int n, int *p)
{
#pragma before
    if (n)
#pragma single_body
    n++;
    switch (n) {
        n = 10;
        case 1: case 2: n++; *p = n; break;
        case 3 ... 5: { n--; break; }
        default: n = 0;
    }
    while (n) n--;
    do { n++; } while (n < 2);
    for (;;) { if (n) break; else continue; }
label:
    if (*p) goto label;
    asm volatile ("" : "=r"(n) : [value] "r"(*p) : "memory");
    return (n ? n + 1 : helper()) + (p[0], p[1]);
}
