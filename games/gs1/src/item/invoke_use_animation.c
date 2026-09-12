#include "types.h"

void *Item_GetData();
s32 Func_080aa460(s32);

void Item_PlayUseAnimation(void)
{
    Func_080aa460(0x3fff & *(u16 *)((u8 *)Item_GetData() + 0x28));
}
