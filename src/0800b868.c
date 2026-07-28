#include "metadata_lookup.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0800b798(s32 arg0);

void Func_0800b868(void *work)
{
    s32 value;
    s32 zero;
    void *info;

    if (work != NULL) {
        info = Func_08185000(M2C_FIELD(work, s16, 0));
        if (M2C_FIELD(info, u8, 0) != 0) {
            value = M2C_FIELD(info, s32, 0x0c);
            if (value == 0) {
                value = Func_0800b798(M2C_FIELD(work, s16, 0));
            }
            M2C_FIELD(work, u8, 4) = M2C_FIELD(info, u8, 4);
            M2C_FIELD(work, s32, 0x0c) = M2C_FIELD(info, s32, 0x10);
            M2C_FIELD(work, s32, 8) = value;
            M2C_FIELD(work, u8, 7) = M2C_FIELD(info, u8, 0x0a);
            zero = 0;
            M2C_FIELD(work, u8, 0x16) = 0xff;
            M2C_FIELD(work, s32, 0x10) = zero;
            M2C_FIELD(work, u8, 0x14) = zero;
        }
    }
}
