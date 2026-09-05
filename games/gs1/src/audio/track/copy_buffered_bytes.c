#include "types.h"

extern u8 Data_0000443c[];

void AudioTrack_CopyBufferedBytes(u8 *destination)
{
    u32 cnt_off = 0x4404;
    u8 **base_p = (u8 **)0x02004c00;
    u8 *base = *base_p;
    u32 index = 0;
    u32 *cnt_p = (u32 *)(base + cnt_off);
    u32 saved_off = cnt_off;
    u32 count = *cnt_p;

    if (count != 0) {
        u32 displacement = (u32)Data_0000443c;
        u32 *offset = (u32 *)(base + displacement);
        u32 *cnt_p2;
        u8 *source;
        displacement -= 52;
        cnt_p = (u32 *)displacement;
        cnt_p2 = (u32 *)(base + saved_off);
        source = base + (u32)cnt_p;
        do {
            destination[*offset] = *source;
            (*offset)++;
            index = index + 1;
            source = source + 1;
        } while (index != *cnt_p2);
    }
}
