#include "types.h"

#define Graphics_PrepareTransferAndRun Func_080051e8

typedef void (*WorkFunc)(void *);

void Func_08004fe4(void *, void *, void *);

void Graphics_PrepareTransferAndRun(void *src, void *dst)
{
    u8 work[48];

    Func_08004fe4(src, dst, work);
    ((WorkFunc)0x030002c0)(work);
}
