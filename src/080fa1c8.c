#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080fa1c8(s32 arg0, void *arg1) {
    u8 *temp_r2;

    temp_r2 = M2C_FIELD(arg1, u8 **, 0x40);
    M2C_FIELD(arg1, u8 **, 0x40) = temp_r2 + 1;
    *(volatile u8 *)temp_r2;
}
