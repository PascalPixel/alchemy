#include "TYPES.H"
#include "VRAM_BLOCK.H"

/*
 * Nonmatching: 26 halfword edits. Block layout, branches and the pool match.
 * The reference keeps the block map in two registers: ip for the fill and the
 * occupied-run lookup, and a copy in r7 (made just before the loop) for the
 * free check and the scan, which leaves id in r6 and the block count in r5.
 * This C reads the map through the symbol for the check, scan and fill, so
 * the compiler holds one lo-register map pointer in r5 and a copy in ip, and
 * id and the block count move up to r7 and r6. Copying the map into a second
 * local is removed by CSE and does not reproduce the r7 copy.
 * 2026-09-26: a structured outer run loop with an inner free-block scan
 * emits 116 bytes / 40 aligned edits, versus this 120-byte / 26-edit
 * baseline. It rotates the range check to the loop tail and still keeps
 * the map in r5; this does not explain the reference's separate ip/r7
 * lifetimes. Preserve the original control flow until new evidence does.
 * 2026-09-29 slice 4: 40 against 605, only two preheader moves apart.
 * The run is a real for (;;) loop, so loop.c hoists the two table addresses
 * into ip and lr; the map pointer is a local set at the top of each pass,
 * so CSE knows it is the table's address and adds the scan offset first
 * (adds r2, r0, r7) as the reference does, and loop.c hoists it too, where
 * cse2 turns it into the r7 copy of ip. The check and scan read the map,
 * the fill and the occupied-run lookup the named arrays (ResourceBlockOwners,
 * gVramBlockCache); end is blocks + result. Left: the reference moves lr
 * before copying ip into r7, which needs the lookup table's address hoisted
 * before the map's; setting a table local first at the top of the loop does
 * that but turns the lookup into [index, base] and swaps r6/r7 (55).
 * alchemy permute (seeds 1 and 11 from the old goto loop, 20 minutes)
 * reached 335; the for (;;) form and its spellings came from targeted
 * variants, and a third search from this form (seed 61, 4 jobs, 10
 * minutes, 23,563 candidates) found nothing below 40.
 */

extern u8 ResourceBlockOwners[512];

s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;

    blocks = size >> 6;
    if (id > 95) {
        return -1;
    }
    {
        s32 pos = 0;
        u32 end;
        u32 i;
        u8 *scan;

        for (;;) {
            u8 *map = ResourceBlockOwners;

            result = -1;
            if (pos >= 512) {
                goto done;
            }
            if (map[pos] != 0xff) {
                goto occupied;
            }
            result = pos;
            end = blocks + result;
            if (pos < end) {
                scan = &map[result];
                do {
                    if (*scan++ != 0xff) {
                        goto occupied;
                    }
                    pos++;
                } while (pos < end);
            }
            for (i = 0; i < blocks; i++) {
                ResourceBlockOwners[result + i] = id;
            }
            goto found;
occupied:
            pos += gVramBlockCache[ResourceBlockOwners[pos]].size >> 6;
        }
found:
        result <<= 6;
done:
        return result;
    }
}
