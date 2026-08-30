#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08005268();

s32 EffectPosition_ApplyBaseAndYOffset(s32 arg0, void *arg1) {
    Func_08005268();
    FIELD_AT_OFFSET(arg1, s32 *, 4) = (s32) (FIELD_AT_OFFSET(arg1, s32 *, 4) - 0x10);
}
