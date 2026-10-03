/* Draft: Resource_InitializeTable, complete EN extent 68 bytes.
 * The 2026-10-03 canonical-record repair scored 520/6 rows, with the marker
 * declaration unresolved. Native load/reset writers establish size at +0 and
 * VRAM byte offset at +2, including the zero/0xffff initialization policy.
 * One ordinary pass now consumes the shared VRAM limits and marker owner.
 * Retained score 380/5: initial marker-load/count setup differs at +4..+a.
 * All six edition objects are identical, with 66 bytes of text; the native
 * 68-byte extent includes two trailing boundary zeros. No padding device was
 * added. The stale linked map leaves ResourceBlockOwners unresolved in scoring
 * (symbol-name comparison only). Still a draft, with no byte credit.
 */
#include "VRAM_TAB.H"

void Resource_InitializeTable(void)
{
    u32 limit = VRAM_BLOCK_COUNT - 1;
    u8 *occupancy_markers = ResourceBlockOwners;
    u32 count = 0;
    u32 empty_marker = VRAM_BLOCK_OWNER_FREE;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct VramBlockCacheEntry *resource_entry = ResourceTableEntries;

        count = 0;
        do {
            resource_entry->offset |= VRAM_CACHE_OFFSET_FREE;
            resource_entry->size = 0;
            resource_entry++;
            count++;
        } while (count < VRAM_CACHE_ENTRY_COUNT);
    }
}
