#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08091200(s32, s32);
s32 Func_08091254(s32);

void Func_0809748c(void) {
    s32 temp_r6;
    void *temp_r5;

    temp_r5 = *(void **)0x03001EBC;
    temp_r6 = temp_r5 + 0x236;
    Func_08091200(temp_r6, 2);
    if (M2C_FIELD(temp_r5, s16 *, 0xCB8) != 0) {
        Func_08091200(0x10001, 1);
    } else {
        Func_08091200(temp_r6, 1);
    }
    Func_08091254(8);
}
