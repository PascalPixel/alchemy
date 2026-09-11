#include "scene.h"
#include "abi/object/motion/pos/snap_to_terrain.h"
#include "effect_runtime.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Motion_SnapToTerrain(void *object)
{
    s32 angle;

    angle = Obj_Place(0, FIELD_AT_OFFSET(object, s32 *, 8), FIELD_AT_OFFSET(object, s32 *, 0x10));
    FIELD_AT_OFFSET(object, s32 *, 0xC) = angle;
    FIELD_AT_OFFSET(object, s32 *, 0x14) = angle;
}
