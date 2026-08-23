#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
s32 Object_SetCallback(void *, s32);

void Func_080999a8(void *object) {
    s32 temp_r5;

    M2C_FIELD(object, s32 *, 0xC) = (s32) (M2C_FIELD(object, s32 *, 0xC) + 0xFFFFB334);
    temp_r5 = Rand();
    M2C_FIELD(object, s32 *, 8) = (s32) (M2C_FIELD(object, s32 *, 8) + (temp_r5 - Rand()));
    if ((s32) M2C_FIELD(object, s32 *, 0xC) <= (s32) M2C_FIELD(object, s32 *, 0x14)) {
        Object_SetCallback(object, Data_0809f0b0);
    }
}
