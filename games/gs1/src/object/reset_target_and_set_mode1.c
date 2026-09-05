#include "object_lookup.h"
#include "types.h"

#define Object_ResetTargetAndSetMode1 Func_08092ab4

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(void *, s32);
s32 Object_ResetMotion();

void Object_ResetTargetAndSetMode1(void) {
    void *temp_r0;

    temp_r0 = ObjectTable_Get();
    if (temp_r0 != NULL) {
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x38) = 0x80000000;
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x3C) = 0x80000000;
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x40) = 0x80000000;
        Object_ResetMotion();
        Object_SetMode(temp_r0, 1);
    }
}
