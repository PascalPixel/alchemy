#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "VRAM_BLOCK.H"
#include "GLYPH.H"
#include "AFFINE.H"
#include "UIWINDOW.H"
#include "RENDER_INPUT.H"
#include "RESOURCE_IDS.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"


/* Byte lanes used by the bobbing sprite pair. */
struct UiBobPairOutput {
    struct UiBobPairOutput *next;
    u8 unknown_04[4];
    u8 source;
    u8 unknown_09[11];
    u8 destination;
};

LAYOUT_OFFSET_GUARD(UiBobPairOutput_Source, struct UiBobPairOutput, source, 8);
LAYOUT_OFFSET_GUARD(UiBobPairOutput_Destination, struct UiBobPairOutput, destination, 0x14);

/* Frame counter advanced by the vertical-blank interrupt. */
extern volatile u32 gFrameTick;
extern const u8 Ui_PairBobOffsets[];

void Runtime_RemapBytesByTableFar(void *, s32);
u32 Resource_DecodeByteLz(const void *, void *);

/* graphics/resource/RenderResource_LoadFrame.c */

/* graphics/resource/RenderResource_CreateFrame.c */
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);

void UiWindow_SetTilemapEntry(struct UiWindow *, s32, s32, s32, u32);

/* ui/apply_table_scale_to_object.c */
struct UiScaleSprite {
    u8 filler0[6];
    u16 src_6;
    u8 src_8;
    u8 filler9[6];
    u8 out_15;
    u8 filler16[4];
    u8 out_20;
    u8 mode_21 : 2;
    u8 rest_21 : 6;
    u16 pos_22 : 9;
    u16 affine_22 : 5;
    u16 rest_22 : 2;
};

extern s32 Ui_ObjectPulseScales[];

/* menu/core/build_localized_pattern_tiles.c */
struct TileMask {
    u32 word0;
    u32 word1;
};

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask Data_08037250[];

extern u8 Data_03001e40[];

/* link/draw_shifted_tile_pair.c */
extern u8 gRomShiftedTilePair[];

/* graphics/tile/expand_vram_tiles_by_color_table.c */
extern u16 Graphics_ExpandNibbleTable[];

void Ui_PrepareTransferFromTableEntry(u32 index);
s32 ItemIcon_Compose(s32, s32);
void Ability_LoadGlyph(s32, s32, s32 *, s32 *, s32);
s32 UiGlyph_LoadEntryWithPalette(u32, s32, s32 *, s32 *, s32, s32);
s32 GameFlag_TestFar(s32);

void Ui_ApplyTableOffsetToPair(struct UiBobPairOutput *obj)
{
    s32 value;

    value = obj->source + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->destination = value;
    obj = obj->next;
    value = obj->source + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->destination = value;
}

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
    s32 size = 1024;
    void *buffer = Runtime_AllocateBlock(14, size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    if (value <= 95) {
        Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Runtime_RemapBytesByTableFar(buffer, 768);
        VramBlock_LoadCached(value, size, buffer);
        Runtime_ReleaseHeapBlock(14);
    }
}

struct RenderOutput *RenderResource_CreateFrame(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 index;
    struct RenderOutput *entity;

    index = Resource_FindFreeEntry();
    entity = NULL;
    if (index != 0x60) {
        RenderResource_LoadFrame(arg0, index, arg1);
        entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
        ((u8 *)&entity->packed)[1] |= 0x20;
        entity->sentinel = 0xfb;
    }
    return entity;
}

void Ui_ApplyTableScaleToObject(struct UiScaleSprite *obj)
{
    s32 v = Ui_ObjectPulseScales[(gFrameTick >> 1) & 7];
    struct AffineTransform efx;

    if (v < 0)
        v += 255;
    v >>= 8;

    if (obj != 0) {
        efx.scale_x = v;
        efx.scale_y = v;
        efx.angle = 0;
        obj->affine_22 = AffineMatrix_BuildForEffect(&efx);
        obj->mode_21 = 3;
        obj->pos_22 = obj->src_6 + 0xfff0;
        obj->out_20 = obj->src_8 + 0xf0;
        obj->out_15 = 0xfc;
    }
}

s32 Menu_BuildLocalizedPatternTiles(void)
{
    u32 *vram = (u32 *)0x06006280;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            Iwram_FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], Data_08037250[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], Data_08037250[mi].word1);
            }
        }
    }
}

/* ui/window/draw_three_tile_column.c */
void UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 tile_offset = arg3 * 2;
    s32 tile = tile_offset + 0xF315;

    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, 0x400 | tile, arg1, arg2, 0);
    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile_offset + 0xF314, arg1 + 1, arg2, 0);
    UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile, arg1 + 2, arg2, 0);
}

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

void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (*(u32 *)((u32)&Data_03001e40) >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Graphics_MergeShiftedTileRows((u32 *)0x06000220,
        (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
    Graphics_MergeShiftedTileRows((u32 *)0x06000240,
        (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}

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
            u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
            s32 col = 0;

            do {
                u32 packed = *src++;
                u32 dec = 0;
                s32 nibble = 0;

                do {
                    u32 color = Graphics_ExpandNibbleTable[
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

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    Ui_PrepareTransferFromTableEntry(resource);
    result = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
    s32 result;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    ItemIcon_Compose(resource, 0x1a);
    result = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
    s32 out;
    s32 cur;
    s32 ret;
    GlyphTransfer *work;

    work = Runtime_AllocateBlock(17, sizeof(*work));
    cur = index;
    Ability_LoadGlyph(resource, 0, &cur, &out, 1);
    ret = Resource_GetBuffer(index, (s32)work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

s32 Ui_LoadEntryForKind(u32 kind, s32 value)
{
    s32 out;
    s32 cur;
    u32 no;

    no = kind;
    cur = value;
    if (no > 7U) {
        no = 0;
    }
    if (GameFlag_TestFar(0x20) != 0) {
        switch (no) {
        case 0:
            no = 0x38;
            break;
        case 1:
            no = 0x39;
            break;
        }
    }
    UiGlyph_LoadEntryWithPalette(no, 0, &cur, &out, 0xE, 1);
    return out;
}
