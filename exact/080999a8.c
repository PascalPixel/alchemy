#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Func_08004458(void);
s32 Func_08009098(void *, s32);

void Func_080999a8(void *object) {
    s32 temp_r5;

    M2C_FIELD(object, s32 *, 0xC) = (s32) (M2C_FIELD(object, s32 *, 0xC) + 0xFFFFB334);
    temp_r5 = Func_08004458();
    M2C_FIELD(object, s32 *, 8) = (s32) (M2C_FIELD(object, s32 *, 8) + (temp_r5 - Func_08004458()));
    if ((s32) M2C_FIELD(object, s32 *, 0xC) <= (s32) M2C_FIELD(object, s32 *, 0x14)) {
        Func_08009098(object, 0x0809F0B0);
    }
}
