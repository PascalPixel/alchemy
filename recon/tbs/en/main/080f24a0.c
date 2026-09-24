/* Draft, not exact (2026-09-24): 588 of 588 bytes, 257 differing halfwords.
   Hand-written from the assembly: palettes and tiles for two backgrounds, four
   30-tile map strips as goto loops (the reference rematerialises 0x10000 in
   each inner loop, so nothing was hoisted), window and blend registers.
   Residual: allocation. The reference keeps the work pointer in r7, the
   halfword zero in r6, 0x200 in r8 and 0x02010000 then 0x1ff in r5; in the
   strips the map pointer is r1, the column r0, the row r4, and the tile is
   copied to r2 for each store. */
#include "TYPES.H"
#include "DMA.H"

struct BgScroll {
    u16 x;
    u16 y;
};

extern u8 Value_00000015;
extern u8 Value_00000017;

void *Resource_GetTableEntry(s32 index);
void Resource_DecodeByteLz(const void *source, void *destination);

void Func_080f24a0(void)
{
    s32 *work = *(s32 **)0x03001efc;
    u8 *resource;
    u16 *map;
    struct BgScroll *scroll;
    s32 row;
    s32 col;
    s16 tile;
    u16 blank = 0x1ff;

    *(u16 *)0x04000000 = 0;
    resource = Resource_GetTableEntry((s32)&Value_00000015);
    Dma_Set(resource, (void *)0x05000200, 0x84000080, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000200 = 0;
    Resource_DecodeByteLz(resource + 0x200, (void *)0x02010000);
    Dma_Set((void *)0x02010000, (void *)0x06010000, 0x80000f00, (volatile u32 *)0x040000d4);
    resource = Resource_GetTableEntry((s32)&Value_00000017);
    Dma_Set(resource, (void *)0x05000000, 0x84000080, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000000 = 0;
    Resource_DecodeByteLz(resource + 0x200, (void *)0x02010000);
    Dma_Set((void *)0x02012940, (void *)0x06000000, 0x80002760, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x0201a140, (void *)0x06004ec0, 0x80004ec0, (volatile u32 *)0x040000d4);

    map = (u16 *)0x0600f000;
    tile = 0x267;
    row = 0;
row0:
    col = 29;
col0:
    *map++ = tile;
    tile = (tile << 16) + 0x10000 >> 16; /* FAKEMATCH: the reference increments the s16 tile in the high halfword */
    if (--col >= 0)
        goto col0;
    *map++ = blank;
    *map++ = blank;
    if (++row <= 10)
        goto row0;
    tile = 0x13b;
    row = 11;
row1:
    col = 29;
col1:
    *map++ = tile;
    tile = (tile << 16) + 0x10000 >> 16; /* FAKEMATCH: the reference increments the s16 tile in the high halfword */
    if (--col >= 0)
        goto col1;
    *map++ = blank;
    *map++ = blank;
    if (++row <= 31)
        goto row1;
    map = (u16 *)0x0600f800;
    tile = 300;
    row = 0;
row2:
    col = 29;
col2:
    *map++ = tile;
    tile = (tile << 16) + 0x10000 >> 16; /* FAKEMATCH: the reference increments the s16 tile in the high halfword */
    if (--col >= 0)
        goto col2;
    *map++ = blank;
    *map++ = blank;
    if (++row <= 10)
        goto row2;
    tile = 0;
    row = 11;
row3:
    col = 29;
col3:
    *map++ = tile;
    tile = (tile << 16) + 0x10000 >> 16; /* FAKEMATCH: the reference increments the s16 tile in the high halfword */
    if (--col >= 0)
        goto col3;
    *map++ = blank;
    *map++ = blank;
    if (++row <= 31)
        goto row3;

    *(u16 *)0x0400000a = 0x1f43;
    *(u16 *)0x0400000c = 0x1e81;
    *(u16 *)0x04000040 = 0xf0;
    *(u16 *)0x04000044 = 0x9f;
    *(u16 *)0x04000042 = 0xf0;
    *(u16 *)0x04000046 = 0x9f;
    *(u16 *)0x04000048 = 0x1616;
    scroll = (struct BgScroll *)0x03001ad0;
    for (row = 3; row >= 0; row--) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    ((struct BgScroll *)0x03001ad0)[1].y = 96;
    ((struct BgScroll *)0x03001ad0)[2].y = 96;
    work[2] = 0;
    work[0] = 0;
    work[1] = 0;
    work[3] = 0;
    work[5] = 0;
    work[4] = 0;
    Dma_Set((void *)0x03001ad0, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    *(u16 *)0x04000050 = 0x3fbf;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
}
