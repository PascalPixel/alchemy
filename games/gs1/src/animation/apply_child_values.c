#include "types.h"
#include "scene.h"
#include "abi/animation/apply_child_values.h"


void Animation_ApplyChildValues(void *obj)
{
    if ((obj != NULL) && (FIELD_AT_OFFSET(obj, u8 *, 0x54) == 1)) {
        Sys_Check(FIELD_AT_OFFSET(obj, s32 *, 0x50));
    }
}
