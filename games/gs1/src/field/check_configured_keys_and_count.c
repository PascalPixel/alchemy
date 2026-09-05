#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800eaf8();

s32 Field_CheckConfiguredKeysAndCount(void *work) {
    Func_0800eaf8();
    FIELD_AT_OFFSET(work, u16 *, 4) = (u16) (FIELD_AT_OFFSET(work, u16 *, 4) + 1);
    return 1;
}
