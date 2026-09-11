#include "types.h"
#include "scene.h"
#include "abi/effects/position/apply_base_and_y_offset.h"


s32 EffectPosition_ApplyBaseAndYOffset(s32 arg0, void *arg1)
{
    Sys_Check();
    FIELD_AT_OFFSET(arg1, s32 *, 4) = (s32)(FIELD_AT_OFFSET(arg1, s32 *, 4) - 0x10);
}
