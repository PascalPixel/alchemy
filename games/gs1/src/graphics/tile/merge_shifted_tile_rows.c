#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* graphics/tile/merge_shifted_tile_rows.c */
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift)
{
    s32 neg;
    s32 rshift;
    s32 lshift;
    s32 i;

    neg = -shift;
    i = 0;
    rshift = neg * 4;
    lshift = shift * 4;

    do {
        u32 result;
        u32 shifted;
        u32 fallback;
        s32 j;

        shifted = *second++;
        result = 0;
        fallback = *first++;
        if (shift < 0)
            shifted >>= rshift;
        else
            shifted <<= lshift;

        j = 7;
        do {
            result <<= 4;
            if (shifted > 0x0FFFFFFF)
                result += shifted >> 28;
            else
                result += fallback >> 28;
            j--;
            shifted <<= 4;
            fallback <<= 4;
        } while (j >= 0);

        *output++ = result;
        i++;
    } while (i <= 7);
}

/* link/draw_shifted_tile_pair.c */
extern u8 gRomShiftedTilePair[];

void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (*(u32 *)ADDR_03001E40 >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Sys_SetMode((void *)0x06000220, gRomShiftedTilePair, offset, -phase);
    Sys_SetMode((void *)0x06000240, gRomShiftedTilePair + 32, offset + 32, phase);
}

/* graphics/tile/expand_vram_tiles_by_color_table.c */
extern u16 gUnk[];
extern u16 gRomByColorTable[];

void Graphics_ExpandVramTilesByColorTable(u16 *dst)
{
    s32 bank = 0;
    s32 pal_ofs = 0;
    s32 dst_bank = 0;

    do {
        s32 row = 0;
        s32 dst_ofs = (dst_bank + bank) << 6;

        do {
            u16 *out = (u16 *)((u8 *)dst + dst_ofs);
            u16 *src = (u16 *)((u8 *)gUnk + (row << 5));
            s32 col = 0;

            do {
                u32 packed = *src++;
                u32 dec = 0;
                s32 nibble = 0;

                do {
                    u32 color = gRomByColorTable[
                        (packed & 15) + pal_ofs
                    ] << (nibble * 4);
                    nibble++;
                    packed >>= 4;
                    dec |= color;
                } while (nibble <= 3);

                col++;
                *out++ = dec;
            } while (col <= 15);

            row++;
            dst_ofs += 32;
        } while (row <= 9);

        pal_ofs += 16;
        dst_bank += 4;
        bank++;
    } while (bank <= 1);
}

/* resource/load_table_entry_to_buffer.c */
/* resource/load_table_entry_to_buffer.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Sys_Do(resource);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Sys_Check(0x11);
    return result;
}

/* resource/load_kind26_entry_to_buffer.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    Sys_Apply(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)(work + 0x400));
    Sys_Check(0x11);
    return result;
}

/* resource/load_indexed_entry_to_buffer.c */
s32 Resource_GetBuffer(s32 index, s32 value);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    u8 *work;

    work = Runtime_AllocateBlock(0x11, 0x608);
    cur = index;
    Sys_SetRange(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)(work + 0x400));
    Sys_Check(0x11);
    return ret;
}
