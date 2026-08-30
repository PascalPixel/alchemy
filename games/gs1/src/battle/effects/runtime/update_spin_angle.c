#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleEffect_UpdateSpinAngle(void *arg0) {
    u32 temp_r2;

    temp_r2 = FIELD_AT_OFFSET(arg0, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(arg0, u16 *, 6) = (u16) (FIELD_AT_OFFSET(arg0, u16 *, 6) + temp_r2 + 0x1000);
    if (temp_r2 < 0x1000U) {
        FIELD_AT_OFFSET(arg0, s16 *, 0x64) = (s16) ((u16) FIELD_AT_OFFSET(arg0, s16 *, 0x64) + 1);
    }
}
