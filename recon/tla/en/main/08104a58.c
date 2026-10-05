/* Matches (differ 0): ☀️'s Resource_LoadPairedBlocksIfAvailable (MENU6.C),
   loading resources 0x200 and 0x201 where ☀️ loads data. It and
   Menu_ResetTwoResourceEntries (08104aa4, one scheduling tie) go at the
   head of GRAPHICS/TILE/CREATE_WITH_LOADED_RESOURCE.C, which already holds
   MENU6.C's third function; it waits for its neighbour. Adopting it names
   rows 0x200 and 0x201 of SYSTEM/RESOURCE/DIRECTORY.S. */
#include "TYPES.H"
#include "RESOURCE.H"
#include "RAM_BUFFER.H"

/* The item menu work's two tile blocks; ☀️ keeps them at 0x392. */
struct ItemMenuBlocks {
    u8 unknown_000[0x4ee];
    s16 resource_slots[2];
};

extern char ResourceId_ItemMenuTilesA;
extern char ResourceId_ItemMenuTilesB;

void VramBlock_LoadResourceFar(s32 entry_no, s32 mode, s32 resource_id);

/* ☀️'s: loads the item menu's two tile blocks into free resource entries. */
void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32 first_slot;
    s32 second_slot;
    struct ItemMenuBlocks *active_state;

    active_state = Ram_HeapSlots->menu_runtime;
    first_slot = Resource_FindFreeEntry();
    active_state->resource_slots[0] = (s16)first_slot;
    if (first_slot != -1) {
        VramBlock_LoadResourceFar(first_slot, 0x80, (s32)&ResourceId_ItemMenuTilesA);
    }
    second_slot = Resource_FindFreeEntry();
    active_state->resource_slots[1] = (s16)second_slot;
    if (second_slot != -1) {
        VramBlock_LoadResourceFar(second_slot, 0x80, (s32)&ResourceId_ItemMenuTilesB);
    }
}
