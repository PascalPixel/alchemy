/* First draft, differ 29: the listing keeps 0x8000 and -0x1021 inside the bit loop and holds the length in r6; this draft moves both constants out and holds the length in r5. */
#include "types.h"

u16 Func_0200217c(u32 length, u8 *data)
{
    u32 crc = 0xffff;
    u32 i;
    u32 bit;

    for (i = 0; i != length; i++) {
        crc ^= data[i] << 8;
        for (bit = 0; bit != 8; bit++) {
            u32 high = crc & 0x8000;
            u32 next = crc << 1;

            crc = next - 0x1021;
            if (!high)
                crc = next;
        }
    }
    return ~crc;
}
