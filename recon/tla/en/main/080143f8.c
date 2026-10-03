/* Draft: Resource_GetBuffer, complete EN extent 20 bytes.
 * No earlier addressed or combined draft exists in the current TLA tree.
 * 2026-10-03: first ordinary pass reuses the maintained TBS wrapper with the
 * actual TLA cache owner. The source word retains its NULL/-1/address contract.
 * Retained score 0/0. All six edition objects are identical; their complete
 * 20-byte text and relocation shapes/targets match the current raw listing.
 * The corresponding complete own-ROM spans are identical in all six editions.
 * This remains an unlinked draft candidate; no byte credit is claimed.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"

s32 Resource_GetBuffer(s32 index, s32 source)
{
    return VramBlock_LoadCached(index, ResourceTableEntries[index].size,
                               (const void *)source);
}
