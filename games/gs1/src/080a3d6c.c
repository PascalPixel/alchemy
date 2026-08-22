#include "types.h"

void *Func_08077008(s32 kind);

s32 Func_080a3d6c(s32 kind) {
    s32 entry;
    s32 remaining;
    s32 count;
    u16 *slots;

    count = 0;
    slots = (u16 *)((u8 *)Func_08077008(kind) + 0xD8);
    remaining = 0xE;
    do {
        entry = 0x1FF & *slots;
        slots += 1;
        if (entry != 0) {
            count += 1;
        }
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}
