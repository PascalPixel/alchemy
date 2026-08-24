#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Object_Destroy();

void Func_08095f9c(void *arg0) {
    s32 position;

    position = M2C_FIELD(arg0, s32 *, 0x18) + 0xFFFFFC00;
    M2C_FIELD(arg0, s32 *, 0x1C) = (s32) (M2C_FIELD(arg0, s32 *, 0x1C) + 0xFFFFFC00);
    M2C_FIELD(arg0, u16 *, 6) = (u16) (M2C_FIELD(arg0, u16 *, 6) + 0x2000);
    M2C_FIELD(arg0, s32 *, 0x18) = position;
    if (position < 0x3000) {
        Object_Destroy();
    }
}
