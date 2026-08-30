#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleEffect_UpdateSpinAngle(void *object) {
    u32 angle_step;

    angle_step = FIELD_AT_OFFSET(object, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(object, u16 *, 6) = (u16) (FIELD_AT_OFFSET(object, u16 *, 6) + angle_step + 0x1000);
    if (angle_step < 0x1000U) {
        FIELD_AT_OFFSET(object, s16 *, 0x64) = (s16) ((u16) FIELD_AT_OFFSET(object, s16 *, 0x64) + 1);
    }
}
