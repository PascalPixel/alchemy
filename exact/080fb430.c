#include "types.h"

void Func_080fb410(void *);

void Func_080fb430(u8 *player, s32 mask_arg, s32 value_arg)
{
    register s32 mask = (u16)mask_arg;
    register s32 value = (u8)value_arg;
    register s32 count;
    register u8 *entry;
    register u32 bit;

    if (*(u32 *)(player + 0x34) != 0x68736d53)
        return;

    *(u32 *)(player + 0x34) += 1;
    count = player[8];
    entry = *(u8 **)(player + 0x2c);
    bit = 1;
    if (count > 0) {
        register s32 check = value;
        do {
            if ((mask & bit) && (entry[0] & 0x80)) {
                entry[0x17] = value;
                if (check == 0)
                    Func_080fb410(entry);
            }
            count--;
            entry += 0x50;
            bit <<= 1;
        } while (count > 0);
    }
    *(u32 *)(player + 0x34) = 0x68736d53;
}
