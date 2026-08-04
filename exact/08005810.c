#include "types.h"

u32 Func_08004458(void);
s32 Func_08002304(s32, s32);
u32 Func_08005b24(s32);

u32 Func_08005810(s32 mode)
{
    u32 empty[16];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = *(u8 **)0x03001F1C;
    count = 0;
    index = 0;
    do {
        value = *slot++;
        if (value == 0) {
            empty[count++] = index;
        }
        index += 1;
    } while (index <= 0xFU);
    index = 0x10;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (Func_08005b24(mode) == 0x10) {
                index = 0x10;
            }
        } else {
            index = Func_08002304(Func_08004458(), count);
            index = empty[index];
        }
    }
    return index;
}
