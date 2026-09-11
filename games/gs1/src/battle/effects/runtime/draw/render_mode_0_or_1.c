#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleFx_RenderMode(void *, s32);

void BattleFx_RenderMode0Or1(void *effect)
{
    if (FIELD_AT_OFFSET(effect, s32 *, 0x18) == 0) {
        BattleFx_RenderMode(effect, 0);
        return;
    }
    BattleFx_RenderMode(effect, 1);
}
