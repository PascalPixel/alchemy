#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_08077018();
s32 Func_080aa460(s32);
#define Ability_PlayUseAnimation Func_080aa460

#define Item_PlayUseAnimation Func_080aa448

void Item_PlayUseAnimation(void)
{
    Ability_PlayUseAnimation(0x3fff & FIELD(Func_08077018(), u16 *, 0x28));
}
