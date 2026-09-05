#include "types.h"

#define Animation_ApplyChildValues Func_0800c598

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800b684(s32);

void Animation_ApplyChildValues(void *obj) {
    if ((obj != NULL) && (FIELD_AT_OFFSET(obj, u8 *, 0x54) == 1)) {
        Func_0800b684(FIELD_AT_OFFSET(obj, s32 *, 0x50));
    }
}
