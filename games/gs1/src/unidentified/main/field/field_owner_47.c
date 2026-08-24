#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Func_08009230();

void Func_0808c44c(void) {
    void *temp_r0;
    void *temp_r0_2;

    temp_r0 = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (M2C_FIELD(temp_r0, s16 *, 0x19E) == 3) {
        temp_r0_2 = Runtime_AllocateBlock(0x1F, 0x540);
        if ((temp_r0_2 != NULL) && (M2C_FIELD(temp_r0_2, s8 *, 0x53D) != 0)) {
            M2C_FIELD(temp_r0_2, s8 *, 0x53A) = 0;
            M2C_FIELD(temp_r0_2, s8 *, 0x53B) = 0;
            M2C_FIELD(temp_r0_2, s8 *, 0x53C) = 1;
            M2C_FIELD(temp_r0_2, s8 *, 0x53D) = 0;
        }
        M2C_FIELD(M2C_FIELD(temp_r0, void **, 0x1E0), s8 *, 0x5B) = 1;
        Func_08009230();
    }
}
