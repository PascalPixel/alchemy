#include "effect_runtime.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080091a8(s32, s32, s32);

void ObjectMotion_SnapToTerrain(void *object) {
    s32 angle;

    angle = Func_080091a8(0, FIELD_AT_OFFSET(object, s32 *, 8), FIELD_AT_OFFSET(object, s32 *, 0x10));
    FIELD_AT_OFFSET(object, s32 *, 0xC) = angle;
    FIELD_AT_OFFSET(object, s32 *, 0x14) = angle;
}
