#include "TYPES.H"
#include "EVENT_RUNTIME.H"

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

/* The object pointer in slot index of the 192-entry table at gWork + 0x14,
   or NULL for an index past the table. */
void *ObjectTable_Get(u32 index)
{
    u8 *base = (u8 *)gWork;
    u32 offset;
    if (index > 0xbf)
        return 0;
    offset = (index * 4) + 0x14;
    return *(void **)(base + offset);
}
