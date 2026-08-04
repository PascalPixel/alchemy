#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0801a778(void) {
    void *temp_r1;

    temp_r1 = *(void **)0x03001E98;
    M2C_FIELD(temp_r1, s32 *, 0x348) = 0;
    M2C_FIELD(temp_r1, s16 *, 0x39A) = 0;
    if (0x80 & M2C_FIELD(temp_r1, u16 *, 0x39E)) {
        M2C_FIELD(temp_r1, s16 *, 0x39C) = 0;
        M2C_FIELD(temp_r1, u16 *, 0x39E) = 0U;
    }
    M2C_FIELD(temp_r1, s16 *, 0x3A0) = 0;
    M2C_FIELD(temp_r1, s16 *, 0x394) = 0;
}
