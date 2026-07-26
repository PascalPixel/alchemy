#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08019d0c(void) {
    s16 *base = (s16 *)*(void **)0x03001E8C;
    s32 i = 0x976;
    do {
        base[i] = 0x3E7;
        i++;
    } while (i != 0x978);
}
