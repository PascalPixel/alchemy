/* Draft: Resource_FindFreeEntry, complete EN extent 52 bytes.
 * The 2026-10-03 record-ownership baseline scored 120/2 rows and retained
 * load-order differences. Its private byte-offset/goto view was the same
 * first-entry shaping now explicitly tagged FAKEMATCH in the maintained TBS
 * implementation. That shape is not silently carried into this ordinary pass.
 * One indexed scan uses the canonical record and free-offset sentinel.
 * Retained ordinary score 1705/26: the single loop replaces the native
 * separate first-entry test and subsequent scan, changing control flow,
 * register allocation and extent. All six edition objects are identical:
 * 40 bytes here versus the complete 52-byte native listing. The TBS header's
 * 36-byte ordinary result is specific to that game's flags. No device was
 * added; this semantic draft has no byte credit.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"

s32 Resource_FindFreeEntry(void)
{
    s32 slot;

    for (slot = 0; slot < VRAM_CACHE_ENTRY_COUNT; slot++) {
        if (ResourceTableEntries[slot].offset == VRAM_CACHE_OFFSET_FREE)
            return slot;
    }
    return VRAM_CACHE_ENTRY_COUNT;
}
