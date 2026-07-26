#include "types.h"

void Func_08002f4c(u16 *arg0, s32 arg1)
{
    s32 count;
    s32 index;

    count = arg1 >> 1;
    index = 1;
    arg0++;
    if (index < count) {
        u32 next_mask;
        u32 current_mask;
        u32 value_mask = 0x7ff;
        next_mask = 0xf800;
        current_mask = 0xf000;
        arg0 -= 2;
        do {
            u16 current;
            u16 next;
            u32 next_tag;
            u32 current_tag;

            next = arg0[2];
            next_tag = next & next_mask;
            arg0++;
            if (next_tag == next_mask) {
                current = arg0[0];
                current_tag = current & next_tag;
                if (current_tag == current_mask) {
                    u32 packed;
                    u32 first;
                    u32 second;

                    packed = (((current & value_mask) << 12)
                        | ((next & value_mask) * 2)) - (index * 2);
                    first = ((packed >> 12) & value_mask) | current_tag;
                    second = ((packed >> 1) & value_mask) | next_tag;
                    arg0[0] = first;
                    arg0[1] = second;
                }
            }
            index++;
        } while (index < count);
    }
}
