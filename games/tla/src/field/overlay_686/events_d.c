#include "ov686.h"

void Func_02000850(s32 a, s32 id);
void Func_02000c24(s32 a, s32 id);

void Func_0200124c(s32 a, s32 id)
{
    if ((u32)(id - 9) <= 1)
        Func_02000c24(a, id);
    else
        Func_02000850(a, id);
}
