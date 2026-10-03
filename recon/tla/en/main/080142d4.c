/* Draft: VramBlock_LoadCached, complete EN extent 148 bytes.
 * Prior canonical-record attempt: 1195/17 rows; Dma_Set, Iwram_ClearWords and
 * ResourceTable_AllocateBlocks remained unresolved.
 * 2026-10-03: one ordinary pass consumes the maintained DMA macro, two-argument
 * resident clear and proven allocator contract, retaining the TBS cache policy.
 * Retained score 300/5; complete text is 148 bytes, with identical objects across
 * all six edition defines. Native differences are table-load scheduling at
 * +6/+8 and size-store scheduling at +4a/+4c/+4e.
 * Qualification: the scorer reconstructs the resident Thumb symbol's pool
 * word as 0x03000259; every own ROM and this C contain 0x03000258. That reported
 * operand row is not a native mismatch. The stale linked map also leaves
 * ResourceTable_AllocateBlocks unresolved (symbol-name comparison only).
 * Still a draft, with no byte credit.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"
#include "IWRAM_CALL.H"
#include "DMA.H"

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source)
{
    struct VramBlockCacheEntry *entry;
    s32 offset;
    void *destination;

    entry = &ResourceTableEntries[slot];
    if (slot >= VRAM_CACHE_ENTRY_COUNT)
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
