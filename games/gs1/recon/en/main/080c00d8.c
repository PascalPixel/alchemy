#include "types.h"

#define BattlePres_BuildTilemap Func_080c00d8

void BattlePres_BuildTilemap(s32 destination)
{
    s32 *cursor;
    s32 entry;
    u32 index;
    /* The ROM calls the IWRAM fill through call-via veneers, not callees
     * taking a fourth argument. Register lifetimes remain unmatched. */
    void (*fill)(s32 destination, s32 size, s32 value) =
        (void (*)(s32, s32, s32))0x03000168;

    fill(destination, 0x100, -1);
    destination += 0x100;
    fill(destination, 0x80, 0x03ff03ff);
    entry = 0x02010200;
    cursor = (s32 *)(destination + 0x80);

    index = 0;
    do {
        index++;
        *cursor++ = entry;
        entry += 0x00020002;
    } while (index <= 239);

    fill((s32)cursor, 0x280, 0x03ff03ff);
}
