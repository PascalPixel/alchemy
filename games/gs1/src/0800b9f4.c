#include "metadata_lookup.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_0800b9f4(void *work, s32 no)
{
    s32 high_bit;
    void *info;
    s32 value;

    high_bit = 0x80 & no;
    if (M2C_FIELD(work, s32, 0x0c) != 0) {
        info = Func_08185000((s32)M2C_FIELD(work, s16, 0));
        if (no < (s32)M2C_FIELD(info, u8, 5)) {
            value = *(s32 *)((u8 *)M2C_FIELD(work, s32, 0x0c) + (no * 4));
            M2C_FIELD(work, u8, 4) = (u8)M2C_FIELD(info, u8, 4);
            M2C_FIELD(work, s32, 0x10) = value;
            M2C_FIELD(work, s8, 0x15) = 0x10;
            if (high_bit == 0) {
                M2C_FIELD(work, s8, 0x14) = high_bit;
                M2C_FIELD(work, s16, 2) = (s16)high_bit;
            }
        }
    }
}
