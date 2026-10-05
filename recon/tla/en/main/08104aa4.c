/* Near miss: differ 2, one scheduling tie. ☀️'s Menu_ResetTwoResourceEntries
   (MENU6.C); it goes after Resource_LoadPairedBlocksIfAvailable (08104a58,
   which matches) at the head of GRAPHICS/TILE/CREATE_WITH_LOADED_RESOURCE.C.
   The listing loads the menu work (ldr r5) before building 0x4ee; this
   build builds the first byte first. u16 slots, two named fields, a slot
   pointer, a loop and the scheduling-barrier search do not move it. */
#include "TYPES.H"
#include "RESOURCE.H"
#include "RAM_BUFFER.H"

/* The item menu work's two tile blocks; ☀️ keeps them at 0x392. */
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
