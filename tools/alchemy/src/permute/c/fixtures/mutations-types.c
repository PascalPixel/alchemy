typedef unsigned short U;
struct S { int member; };
extern const U global;
extern const U global;
extern int table[4];
int helper(int h);
int helper(int other);
void side(void);
float floating(float f);
int variadic(int a, ...);
int (*callback)(int);
int target(int a, U b, struct S *p)
{
    const U x = 0;
    float f = 0.0f;
    long double ld;
    int y;
    int z;
    y = helper(a) + global;
    z = table[b] + callback(a);
    f = floating(f);
    side();
    ld = f;
    y += variadic(a, b);
    return y + z + p->member + x + (int)ld;
}
