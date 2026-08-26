#include "types.h"

#define ResourceTable_AllocateBlocks Func_08003e58

struct ResourceTableEntry {
    u16 size;
    u16 block;
};

extern u8 Data_03001810[512];
extern struct ResourceTableEntry Data_03001b10[96];

s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    struct ResourceTableEntry *tbl;
    u8 *scan_map;
    u8 *map;
    u32 blocks;
    s32 result;
    s32 pos;

    blocks = size >> 6;
    if (id > 95) return -1;
    map = Data_03001810;
    tbl = Data_03001b10;
    pos = 0;
    scan_map = map;
next_run:
    result = -1;
    if (pos >= 512) {
        goto done;
    }
    if (scan_map[pos] == 0xff) {
        u32 end;
        u32 i;
        u8 *scan;

        result = pos;
        end = result + blocks;
        scan = scan_map + result;
        while (pos < end) {
            if (*scan++ != 0xff) {
                goto occupied;
            }
            pos++;
        }
        for (i = 0; i < blocks; i++) {
            map[result + i] = id;
        }
        result <<= 6;
        goto done;
    }
occupied:
    pos += tbl[map[pos]].size >> 6;
    goto next_run;
done:
    return result;
}
