#include "TYPES.H"

void *Item_GetData();
s32 Func_080aa460(s32);
#define Ability_PlayUseAnimation Func_080aa460

void Item_PlayUseAnimation(void)
{
    Ability_PlayUseAnimation(0x3fff & *(u16 *)((u8 *)Item_GetData() + 0x28));
}
