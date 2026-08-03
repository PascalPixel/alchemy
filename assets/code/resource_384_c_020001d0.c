#include "resource_384.h"

extern void Func_0200049e(s32);
extern void Func_0200049c(s32);

#define WaitForClosingBeat  Func_0200049e
#define FinishClosingBeat   Func_0200049c

void Func_020001d0(void)
{
    WaitForClosingBeat(123);
    FinishClosingBeat(1);
}
