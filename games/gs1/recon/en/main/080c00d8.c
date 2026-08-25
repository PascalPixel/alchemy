#include "types.h"

#define BattlePresentation_BuildTilemap Func_080c00d8

void BattlePresentation_BuildTilemap(s32 destination)
{
    s32 *cursor;
    s32 entry;
    u32 index;

    Func_080072f8(destination, 0x100, -1);
    destination += 0x100;
    Func_080072f8(destination, 0x80, 0x03ff03ff);
    entry = 0x02010200;
    cursor = (s32 *)(destination + 0x80);

    index = 0;
    do {
        index++;
        *cursor++ = entry;
        entry += 0x00020002;
    } while (index <= 239);

    Func_080072f0(cursor, 0x280, 0x03ff03ff, 0x03000168);
}
