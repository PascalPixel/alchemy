#include "types.h"
extern s32 Func_02001dba(s32);

void *Func_0200042c(void)
{
    if (Func_02001dba(0x855) != 0)
        return (void *)0x0200a27c;
    return (void *)0x0200a0cc;
}
