#include "types.h"

void Func_080fb334(u8 *player, u32 mask_arg, u32 value_arg)
{
    u16 mask = mask_arg;
    u16 value = value_arg;
    u32 current = *(u32 *)(player + 52);

    if (current == 0x68736d53) {
        s32 count;
        u8 *entry;
        u32 bit;

        *(u32 *)(player + 52) = current + 1;
        count = player[8];
        entry = *(u8 **)(player + 44);
        bit = 1;
        if (count > 0) {
            do {
                if ((mask & bit) != 0 && (entry[0] & 0x80) != 0) {
                    entry[11] = (s8)((s16)value >> 8);
                    entry[13] = value;
                    entry[0] |= 12;
                }
                count--;
                entry += 80;
                bit <<= 1;
            } while (count > 0);
        }
        *(u32 *)(player + 52) = 0x68736d53;
    }
}
