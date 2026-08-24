#include "metadata_lookup.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s16 *Func_0808d394(s16);

s32 Func_0809376c(void *arg0) {
    void *temp_r6;

    temp_r6 = M2C_FIELD(arg0, void **, 0x68);
    if (temp_r6 != NULL) {
        M2C_FIELD(arg0, s8 *, 0x55) = 0;
        M2C_FIELD(arg0, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + (M2C_FIELD(Func_08185000(*Func_0808d394(M2C_FIELD(arg0, s16 *, 0x66))), s8 *, 8) << 0x10) + 0x80000);
        M2C_FIELD(arg0, s32 *, 0x14) = (s32) M2C_FIELD(temp_r6, s32 *, 0x14);
        M2C_FIELD(arg0, s32 *, 0x10) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
    }
    return 0;
}
