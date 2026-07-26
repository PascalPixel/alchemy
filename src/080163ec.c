#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);

void Func_080163ec(void *arg0) {
    if (!(2 & M2C_FIELD(arg0, u16 *, 0x16)) && (M2C_FIELD(arg0, s16 *, 0x1A) != 0)) {
        do {
            Func_080030f8(1);
        } while (M2C_FIELD(arg0, s16 *, 0x1A) != 0);
    }
}
