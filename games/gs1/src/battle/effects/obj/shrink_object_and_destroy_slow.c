#include "types.h"
#include "scene.h"

void Object_Destroy();

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) + 0xFFFFFE40;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0x1C) + 0xFFFFFE40);
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + 0x2000);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x3000) {
        Object_Destroy();
    }
}
