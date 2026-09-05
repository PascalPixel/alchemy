#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Item_GetData();
s32 Func_080aa460(s32);

void Item_PlayUseAnimation(void)
{
    Ability_PlayUseAnimation(0x3fff & FIELD(Item_GetData(), u16 *, 0x28));
}
