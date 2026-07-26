#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080030f8();

void Func_0809163c(s32 arg0) {
    if ((M2C_FIELD(*(void **)0x03001EBC, s32 *, 0x1CC) == 0) && (arg0 != 0)) {
        Func_080030f8();
    }
}
