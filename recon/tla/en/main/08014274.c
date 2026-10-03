/* Draft: Resource_ResetEntry, complete EN extent 56 bytes.
 * The 2026-10-03 record-ownership repair uses COMMON VRAM_TAB.H's two-halfword
 * size/state and byte-offset record and scored 60/1 row before this pass.
 * One ordinary pass now uses the shared limits and removes the unused private
 * marker declaration.
 * Retained score 60/1: the offset halfword load and sentinel shift exchange
 * order at +14/+16. The complete text extent remains 56 bytes and is identical
 * across all six edition defines. Still a draft, with no byte credit.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"

s32 Resource_ResetEntry(u32 resource_index)
{
    struct VramBlockCacheEntry *entry = &ResourceTableEntries[resource_index];

    if (resource_index >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
        Resource_ClearSlotReferences(resource_index);
        entry->offset |= VRAM_CACHE_OFFSET_FREE;
        entry->size = 0;
    }
    return 0;
}
