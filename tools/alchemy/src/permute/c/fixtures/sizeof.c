typedef int T;
_Atomic(int) atomic_int;
_Atomic(int *) atomic_ptr;
typedef _Atomic(_Atomic(_Atomic(int (*)(void)) *) *) AtomicCallback;
int target(int x, int y)
{
    x = sizeof(int) + x * 2;
    x = sizeof(int) - x + y;
    x = sizeof(T) * x * y;
    x = sizeof(int) & x | y;
    x = (sizeof(int)) + x;
    x = sizeof(int) + sizeof(int) + x;
    x = sizeof(int) + (x + y) * y;
    x = sizeof(int) + (int)(x + y) * y;
    x = sizeof(x) + y;
    return x;
}
