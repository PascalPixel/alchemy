#include "types.h"

#define BattleEffect_AdvanceObjectField6WithRamp Func_08095bac

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleEffect_AdvanceObjectField6WithRamp(void *obj) {
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16) (FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16) ((u16) FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}
