#include "types.h"
#include "scene.h"

/* runtime/remap_bytes_by_table.c */
extern u8 gRom[];

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = gRom;
    n = cnt - 1;
    if (n != -1) {
        do {
            n -= 1;
            *p = tbl[*p];
            p += 1;
        } while (n != -1);
    }
}

/* graphics/renumber_filler_entries.c */
extern u32 gUnk[];

void Graphics_RenumberFillerEntries(void)
{
    u32 *p = gUnk;
    u32 cnt = 128 << 7;
    u32 mask = 0xfff;
    s32 no = -1;

    do {
        u32 value = *p++;
        u32 idx = value & mask;

        if (idx == mask) {
            if (no != (s32)idx) {
                no++;
            }
            value = value + no - idx;
            p[-1] = value;
        }
        cnt--;
    } while (cnt != 0);
}
