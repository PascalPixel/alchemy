#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08009158();

void Func_08098184(void *object) {
    s32 next;
    s32 scale;

    if (object != NULL) {
        scale = M2C_FIELD(object, s32 *, 0x18);
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            M2C_FIELD(object, s32 *, 0x18) = next;
            M2C_FIELD(object, s32 *, 0x1C) = next;
        }
        Func_08009158();
    }
}
