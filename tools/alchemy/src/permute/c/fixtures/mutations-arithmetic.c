int target(int a, int b, int *p)
{
    int x;
    float f = 1.0f;
    x = a * 12 + b * 0x18u;
    x += a << 7;
    x = x + 3;
    x %= 2;
    p = p + a;
    x = *(p + b);
    if (f < 2.0f) { x = a; } else { x = b; }
    x = a ? x : b;
    return (x >> 4) + sizeof(a * 12);
}
