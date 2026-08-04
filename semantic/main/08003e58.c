#include "types.h"

extern u8 Data_03001810[];
extern u16 Data_03001b10[];

s32 Func_08003e58(u32 id, u32 size)
{
    u32 count = size >> 6;
    s32 position;
    u16 *sizes;

    if (id > 95)
        return -1;

    sizes = Data_03001b10;
    position = 0;
    for (;;) {
        u32 start;
        u32 end;
        u32 fill;
        u8 *cursor;
        u8 value;
        s32 result = -1;

        if (position >= 512)
            return result;

        if (Data_03001810[position] != 0xff) {
occupied:
            position += sizes[Data_03001810[position] * 2] >> 6;
            continue;
        }

        start = position;
        end = start + count;
        if ((u32)start < end) {
            cursor = Data_03001810 + start;
            do {
                value = *cursor++;
                if (value != 0xff)
                    goto occupied;
                position++;
            } while ((u32)position < end);
        }

        for (fill = 0; fill < count; fill++)
            Data_03001810[start + fill] = id;
        return start << 6;
    }
}
