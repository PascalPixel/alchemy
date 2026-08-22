#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08095bac(void *arg0) {
    u32 step;

    step = M2C_FIELD(arg0, s16 *, 0x64) * 0x50;
    M2C_FIELD(arg0, u16 *, 6) = (u16) (M2C_FIELD(arg0, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        M2C_FIELD(arg0, s16 *, 0x64) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 0x64) + 1);
    }
}
