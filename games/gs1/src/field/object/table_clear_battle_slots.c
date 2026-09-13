#include "types.h"
#include "event_runtime.h"

/* object/table/clear_battle_slots.c */
void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 offset;
    s32 zero;
    s32 count;
    offset = 0x8C;
    zero = 0;
    count = 0x41;
    current = (s32 *)((u8 *)gWork + offset * 2);
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}

/* object/table/get.c */
void *get(u32 arg0)
{
    u8 *base = (u8 *)gWork;
    u32 offset;
    if (arg0 > 0xbf)
        return 0;
    offset = (arg0 * 4) + 0x14;
    return *(void **)(base + offset);
}
