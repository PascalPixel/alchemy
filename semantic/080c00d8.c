#include "types.h"

typedef void (*Routine)(void *destination, s32 size, s32 value);

void Func_080c00d8(u8 *buffer)
{
    Routine run = (Routine)0x03000168;
    u8 *cursor;
    u32 *entry;
    u32 value;
    s32 index;

    cursor = buffer;
    run(cursor, 256, -1);
    cursor += 256;
    run(cursor, 128, 0x03ff03ff);
    cursor += 128;
    entry = (u32 *)cursor;
    value = 0x02010200;
    for (index = 0; index < 240; index++) {
        *entry++ = value;
        value += 0x00020002;
    }
    run(entry, 640, 0x03ff03ff);
}
