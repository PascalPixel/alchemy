typedef unsigned long u32;
struct Forward __attribute__((packed));
struct Packed { int x; } __attribute__((packed));
typedef struct { int x; unsigned flags : 3; } Pair, *PairPtr;
struct Item { struct { int nested; }; int value; } first, *second;
enum Kind { ZERO, ONE = 1, TWO } kind, *kind_ptr;
int *array[2][3];
int (*matrix)[3];
int (*callbacks[3])(const char *, int);
int *(*factory(void))(int);
extern int __attribute__((noinline)) call(u32);
static int helper(int x) { return x; }
static inline int inlined(int x) { return x * 2; }
int target(const Pair *p, int n)
{
    int a = 0, b = 1;
    int values[3] = { [0] = 4, [1 ... 2] = 8 };
    Pair pair = { .x = 3, .flags = 1 };
    for (int i = 0, j = n; i < j; i++, j--) a += values[i];
    if (a) b = p->x; else if (n) b = (int)sizeof(Pair);
    return call((u32)(a + b)) + pair.flags;
}
