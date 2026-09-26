#include "TYPES.H"

#define ResourceTable_AllocateBlocks Func_08003e58

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
 */

struct ResourceTableEntry {
    u16 size;
    u16 block;
};

extern u8 Data_03001810[512];
extern struct ResourceTableEntry Data_03001b10[96];

s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;

    blocks = size >> 6;
    if (id > 95) {
        return -1;
    }
    {
        u8 *map = Data_03001810;
        struct ResourceTableEntry *tbl = Data_03001b10;
        s32 pos = 0;
        u32 end;
        u32 i;
        u8 *scan;

next_run:
        result = -1;
        if (pos >= 512) {
            goto done;
        }
        if (Data_03001810[pos] != 0xff) {
            goto occupied;
        }
        result = pos;
        end = result + blocks;
        if (pos < end) {
            scan = Data_03001810 + result;
            do {
                if (*scan++ != 0xff) {
                    goto occupied;
                }
                pos++;
            } while (pos < end);
        }
        for (i = 0; i < blocks; i++) {
            Data_03001810[result + i] = id;
        }
        goto found;
occupied:
        pos += tbl[map[pos]].size >> 6;
        goto next_run;
found:
        result <<= 6;
done:
        return result;
    }
}
