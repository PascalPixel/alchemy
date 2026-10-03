/*
 * Draft: ResourceTable_AllocateBlocks; raw/08014174.s spans 128 bytes.
 * All six native spans are identical, including both storage literals.
 * Unsigned byte counts are floored to 64-byte blocks. Zero-block requests
 * find a free marker without reserving it; occupied entries skip their full
 * cached size/64 from the collision position, without repairing bad state.
 * TLA checks the candidate end against 512 before scanning, unlike the
 * maintained TBS body. This allocator changes markers, not cache records.
 * No prior addressed draft exists; no earlier baseline score is available.
 * Initial ordinary TBS-derived form plus TLA's bounds check: EN 1020/34.
 * ResourceBlockOwners is unresolved in the older linked symbol map, so that
 * scorer reference is compared by name only. All six compiled forms have
 * identical 124-byte text versus the native 128-byte extent. Register
 * allocation and reuse of the outer 512 constant differ; both ABS32 literal
 * targets agree, but their offsets are 0x74/0x78 versus native 0x78/0x7c.
 * One ordinary form measured; no retry, device, padding or match claimed.
 */
#include "RESOURCE.H"
#include "VRAM_TAB.H"

s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;
    s32 pos;
    u32 end;
    u32 i;

    blocks = size / VRAM_BLOCK_BYTES;
    if (id >= VRAM_CACHE_ENTRY_COUNT)
        return -1;
    pos = 0;
    for (;;) {
        result = -1;
        if (pos >= VRAM_BLOCK_COUNT)
            goto done;
        if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
            goto occupied;
        end = blocks + pos;
        if (end > VRAM_BLOCK_COUNT)
            goto done;
        result = pos;
        while (pos < end) {
            if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
                goto occupied;
            pos++;
        }
        for (i = 0; i < blocks; i++)
            ResourceBlockOwners[result + i] = id;
        goto found;
occupied:
        pos += (u32)ResourceTableEntries[ResourceBlockOwners[pos]].size
            / VRAM_BLOCK_BYTES;
    }
found:
    result *= VRAM_BLOCK_BYTES;
done:
    return result;
}
