#include "types.h"
#include "scene.h"
#include "abi/battle/find_tagged_slot_by_value.h"

extern void *gBattleWork;

s32 Battle_FindTaggedSlotByValue(u32 value)
{
    s32 index;
    s32 tag;
    s32 offset;
    char *base;
    s16 item;

    base = gBattleWork;
    if (value <= 7) {
        tag = 0x80;
        index = 0;
        tag <<= 1;
        offset = 0x58;
first:
        item = *(s16 *)(offset + (u32)base);
        if (item == 0xff)
            return -1;
        if (item == 0xfe)
            goto next_first;
        if (item == value)
            return index | tag;
next_first:
        offset += 2;
        index++;
        goto first;
    }

    tag = 0xc0;
    index = 0;
    base += 2;
    tag <<= 1;
    offset = 0x64;
second:
    item = *(s16 *)(offset + (u32)base);
    if (item == 0xff)
        return -1;
    if (item == 0xfe)
        goto next_second;
    if (item == value)
        return index | tag;
next_second:
    offset += 2;
    index++;
    goto second;
}
