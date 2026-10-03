/* Draft: 1195 score / 17 differing instructions against the EN listing.
 * The shared cache record and TLA object name restore compilation.
 * Dma_Set, Iwram_ClearWords and ResourceTable_AllocateBlocks are still
 * unresolved names; helper contracts and ordinary code shape remain.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"
#include "IWRAM_CALL.H"

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source)
{
    struct VramBlockCacheEntry *entry;
    s32 offset;
    void *destination;

    entry = &ResourceTableEntries[slot];
    if (slot > 95)
        return 0;
    if (size > 0x2000)
        return 0;
    if (entry->size > 16) {
        if (entry->size != size) {
            Resource_ResetEntry(slot);
            offset = ResourceTable_AllocateBlocks(slot, size);
        } else {
            offset = entry->offset;
        }
    } else {
        offset = ResourceTable_AllocateBlocks(slot, size);
    }

    if (offset != -1) {
        destination = (void *)(0x06010000 + offset);
        entry->size = size;
        entry->offset = offset;
        if (source != 0) {
            if (source == (const void *)-1) {
                Iwram_ClearWords(destination, size);
            } else {
                Dma_Set(source, destination, (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
            }
        }
        return (u32)offset >> 5;
    }
    return 0;
}
