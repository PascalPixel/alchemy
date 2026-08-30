#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleEffect_RenderMode(void *, s32);

void BattleEffect_RenderMode0Or1(void *effect) {
    if (FIELD_AT_OFFSET(effect, s32 *, 0x18) == 0) {
        BattleEffect_RenderMode(effect, 0);
        return;
    }
    BattleEffect_RenderMode(effect, 1);
}
