#include "types.h"

typedef void (*WorkFunc)(void *);

void Func_08004fe4(void *, void *, void *);

void Func_080051e8(void *src, void *dst)
{
    u8 work[48];

    Func_08004fe4(src, dst, work);
    ((WorkFunc)0x030002c0)(work);
}
