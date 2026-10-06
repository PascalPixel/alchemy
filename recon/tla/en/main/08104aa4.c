#include "TYPES.H"
#include "RESOURCE.H"
#include "RAM_BUFFER.H"

struct ItemMenuBlocks {
    u8 unknown_000[0x4ee];
    s16 resource_slots[2];
};

void Menu_ResetTwoResourceEntries(void)
{
    struct ItemMenuBlocks *state = Ram_HeapSlots->menu_runtime;

    Resource_ResetEntry((u16)state->resource_slots[0]);
    Resource_ResetEntry((u16)state->resource_slots[1]);
}
