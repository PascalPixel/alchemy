#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "mem.h"

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
