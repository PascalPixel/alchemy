#include "types.h"

#define Graphics_RenumberFillerEntries Func_0800fa8c

extern u32 Data_02010000[];

void Graphics_RenumberFillerEntries(void)
{
    u32 *p = Data_02010000;
    u32 cnt = 128 << 7;
    u32 mask = 0xfff;
    s32 no = -1;

    do {
        u32 value = *p++;
        u32 idx = value & mask;

        if (idx == mask) {
            if (no != (s32)idx) {
                no++;
            }
            value = value + no - idx;
            p[-1] = value;
        }
        cnt--;
    } while (cnt != 0);
}
