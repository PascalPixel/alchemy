#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080d89ac(void *, s32);

void Func_080d896c(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x18) == 0) {
        Func_080d89ac(arg0, 3);
        return;
    }
    Func_080d89ac(arg0, 4);
}
