int global;
static inline int inline_fn(int arg0) { return arg0 + 1; }
int inline_fn2(int a);
int target(int a, int b)
{
    int x;
    int y;
    int arg0;
    x = a + b;
    y = a + b;
    arg0 = inline_fn2(a);
    x += a + b;
    y += inline_fn(a) + b;
    return x + y + arg0 + global + 7;
}
