/*
 * Draft: Event_GetActorTable: the event work's table for the current owner,
 * or its slot 13 while the party state's owner override is set. Score 6: the
 * listing indexes with the 20-byte offset added to the scaled owner ([r0,
 * r3]) where this C folds it into the load.
 */
#include "TYPES.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "RUNTIME_MEM.H"

struct SaveSlotRecord {
    u8 unknown_00[0x1c];
    u8 used;
    u8 unknown_1d[0x23];
};

struct SaveWork {
    u8 unknown_0000[0x303c];
    struct SaveSlotRecord slots[3];
};


void *Event_GetActorTable(void)
{
    void **event = Ram_HeapSlots->event_work;

    if (gPartyState.owner_override == 0)
        event = *(void **)((u8 *)event + (gPartyState.current_owner * 4 + 20));
    else
        event = event[13];
    return event;
}
