#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Runtime_PushSlotEntry(s32 *slot_entry, s32 slot)
{
    s32 *previous_head;
    s32 slot_offset;
    s32 clamped_slot;

    clamped_slot = slot;
    if (clamped_slot > 0xFF) {
        clamped_slot = 0xFF;
    }
    if (clamped_slot < 0) {
        clamped_slot = 0;
    }
    slot_offset = clamped_slot * 4;
    previous_head = FIELD_AT_OFFSET(slot_offset, s32 **, ADDR_03001400);
    FIELD_AT_OFFSET(slot_offset, s32 **, ADDR_03001400) = slot_entry;
    *slot_entry = previous_head;
}
