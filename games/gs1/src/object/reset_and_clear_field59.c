#include "types.h"

#define Object_ResetAndClearField59 Func_08093964

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080091e0(void *, s32);

s32 Object_ResetAndClearField59(void *obj) {
    Func_080091e0(obj, 0);
    FIELD_AT_OFFSET(obj, s8 *, 0x59) = 0;
    return 0;
}
