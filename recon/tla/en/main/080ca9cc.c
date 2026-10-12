#include "types.h"

extern u8 Data_0202e008[];

u32 Stream_ReadU16(u8 **cursor);

static inline s8 Stream_ReadS8(u8 **cursor)
{
    return *(*cursor)++;
}

s32 Func_080ca9cc(s32 id, s32 *position)
{
    u8 *cursor = Data_0202e008;
    s8 key;

    while ((key = Stream_ReadS8(&cursor)) != -1) {
        if (key == id) {
            position[0] = Stream_ReadU16(&cursor) << 16;
            position[2] = Stream_ReadU16(&cursor) << 16;
            return 0;
        }
        cursor += 4;
    }
    return -1;
}
