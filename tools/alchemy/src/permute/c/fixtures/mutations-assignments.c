int called(int a);
int target(int a, int b)
{
    int x = a + b;
    int y;
    int z;
    int u;
    int v;
    int w;
    x = a + b;
    y = a + b;
    z = a + b;
    u = a + b;
    v = a + b;
    w = a + b;
    x += a;
    y = a;
    z = a;
    x = called(a);
    y = x;
    if (a) {
        int q = b;
        y = q + x * b;
        z = y;
    }
    x = (int)(255 & 15);
    y = sizeof((int)(a & 255));
    return (z = a + b) + x + y + u + v + w;
}
