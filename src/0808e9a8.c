#include "effect_runtime.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080091a8(s32, s32, s32);

void Func_0808e9a8(void *object) {
    s32 angle;

    angle = Func_080091a8(0, M2C_FIELD(object, s32 *, 8), M2C_FIELD(object, s32 *, 0x10));
    M2C_FIELD(object, s32 *, 0xC) = angle;
    M2C_FIELD(object, s32 *, 0x14) = angle;
}
