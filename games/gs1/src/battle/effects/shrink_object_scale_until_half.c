#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080090f0(void *, s32, s32, s32);

void BattleEffect_ShrinkObjectScaleUntilHalf(void *obj) {
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) - 0x80;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = scale;
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x8000) {
        Func_080090f0(obj, 0, 0, 0);
        FIELD_AT_OFFSET(obj, s32 *, 0x6C) = 0;
    }
}
