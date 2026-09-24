#include "TYPES.H"

/* Battle effect 14: its object update and its entry. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Object_SetPositionAndResetMotionFar(void *, s32, s32, s32);
s32 RunBattleEffect14();

void BattleFx_ShrinkObjectScaleUntilHalf(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) - 0x80;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = scale;
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x8000) {
        Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
        FIELD_AT_OFFSET(obj, s32 *, 0x6C) = 0;
    }
}

void BattleFx_CallEffect14(void)
{
    RunBattleEffect14();
}
