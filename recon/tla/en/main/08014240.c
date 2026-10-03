/* Draft: Resource_ClearSlotReferences, complete EN extent 52 bytes.
 * The earlier TBS reuse note reported four differing halfwords; the fresh
 * approved-compiler baseline is 200/2 rows with ResourceBlockOwners unresolved.
 * The actual listing is recon/tla/raw/08014240.s (the old header named 08014220).
 * 2026-10-03: one ordinary pass uses the maintained shared VRAM limits and
 * marker declaration.
 * Retained score 60/1: the marker load and remaining-count setup exchange order
 * at +8/+a. The complete text extent remains 52 bytes and is identical across all
 * six edition defines. The stale linked map leaves ResourceBlockOwners
 * unresolved in scoring (symbol-name comparison only). Still a draft.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    marker = ResourceBlockOwners;
    empty_marker = VRAM_BLOCK_OWNER_FREE;
    remaining = VRAM_BLOCK_COUNT;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0)
        return -1;
    return 0;
}
