#include "types.h"

extern void Func_020054de(void *, s32);

s32 Func_02000594(void *object)
{
    Func_020054de(object, *(u16 *)((u8 *)object + 100) & 15);
    return 0;
}
