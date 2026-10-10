int target(int a, int b)
{
    float f = 0.0f;
    int x = 0;
    int y;
    x = a + b;
    if (a != 0) { y = x; } else { y = b; }
    while (!b) { b++; }
    do { x += b; } while (a == 0);
    for (y = 0; y < a; y++) { x += y; }
    return x + a;
}
