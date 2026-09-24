#include "TYPES.H"

void *Item_Get();
s32 Ability_PlayUseAnimation(s32);

void Item_PlayUseAnimation(void)
{
    Ability_PlayUseAnimation(0x3fff & *(u16 *)((u8 *)Item_Get() + 0x28));
}
