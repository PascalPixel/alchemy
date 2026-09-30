#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "TYPES.H"

typedef struct {
    u8 input[0x400];
    u8 tiles[0x200];
    s16 width;
    s16 height;
    u8 *encoded;
} GlyphTransfer;

GlyphTransfer *Runtime_AllocateHeapBlock(s32 kind, s32 size);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32 kind);

extern const u8 Tile_Decompress4bpp[];
extern const u8 Tile_ExpandMasked[];
extern const u8 Tile_ExpandOpaque[];
void Runtime_ReleaseHeapBlock(s32 slot);

/* Heap block addresses, indexed by block number. */
extern void *Data_03001e50[];

/* ARM routines copied into heap block 49 before each call. */
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];
#define ROUTINE_BLOCK 49

struct GlyphCursor { u16 unused; u16 cursor; };

struct GlyphRow {
    u8 unknown_00[8];
    struct GlyphCursor index;
    u8 unknown_0c[6];
    u16 state;
    u8 unknown_14[32];
};

struct GlyphVisual {
    u8 unknown_00[5];
    u8 flags05_0 : 2;
    u8 flags05_2 : 2;
    u8 flags05_4 : 1;
    u8 flags05_5 : 1;
    u8 flags05_6 : 2;
    u8 unknown_06;
    u8 flags07_0 : 1;
    u8 flags07_1 : 5;
    u8 flags07_6 : 2;
    u8 unknown_08;
    u8 flags09_0 : 2;
    u8 flags09_2 : 2;
    u8 flags09_4 : 4;
    u8 unknown_0a[2];
};

struct GlyphWork {
    struct GlyphRow rows[14];
    u8 unknown_2d8[10];
    u16 field_2e2;
    u16 slot;
    u16 tile;
    u8 unknown_2e8[18];
    u16 field_2fa;
    u8 unknown_2fc[4];
    struct GlyphVisual visual;
    u8 unknown_30c[10];
    u16 field_316;
    u8 unknown_318[48];
    s32 field_348;
    s32 field_34c;
    s32 field_350;
    u8 unknown_354[64];
    u16 field_394;
    u8 unknown_396[4];
    u16 field_39a;
    u16 field_39c;
    u16 field_39e;
    u16 field_3a0;
    u8 unknown_3a2[22];
    u16 field_3b8;
    u8 unknown_3ba[42];
};

extern const u8 Data_080346f8[];
struct GlyphWork *Runtime_AllocateBlock(s32 kind, s32 size);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
    /* FAKEMATCH: keep the cursor-subrecord base for the paired row stores. */
    cursor->cursor = 0;
}

void UiGlyph_DecodeWithHeapRoutines(u8 *glyph, s32 outlined);

void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse)
{
    GlyphTransfer *work;
    u16 *table;
    u8 *entry;
    u32 index;

    work = Runtime_AllocateHeapBlock(17, 0x608);
    table = Resource_GetTableEntry((s32)&ResourceId_Icons);
    if (icon <= 127)
        index = icon;
    else
        index = icon - 112;
    entry = (u8 *)table + table[index];
    work->encoded = entry + 32;
    work->width = 4;
    work->height = 4;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    if (reuse == 0)
        *slot = Resource_FindFreeEntry();
    *tile = VramBlock_LoadCached(*slot, 0x200, work->tiles);
    Runtime_ReleaseHeapBlock(17);
    Dma_Set(entry, (void *)(0x05000200 + palette * 32), 0x80000010, (volatile u32 *)0x040000d4);
}

/* An empty routine between the glyph loader and decoder; nothing in the
   main image calls it by name. */
void UiGlyph_ReservedNoOp(void)
{
}

void UiGlyph_DecodeWithHeapRoutines(u8 *glyph, s32 outlined)
{
    void *code;

    {
        u32 size;

        /* FAKEMATCH: retain the size load after saving both arguments. */
        do {
            size = (u32)Tile_Decompress4bppCodeSize;
        } while (0);
        code = (u8 *)Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Tile_Decompress4bpp, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    }
    ((void (*)(const void *, u8 *))Data_03001e50[ROUTINE_BLOCK])(
        *(const void **)(glyph + 0x604), glyph);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
    if (outlined) {
        u32 size;

        size = (u32)Tile_ExpandMaskedCodeSize;
        code = (u8 *)Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Tile_ExpandMasked, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    } else {
        u32 size;

        size = (u32)Tile_ExpandOpaqueCodeSize;
        code = (u8 *)Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Tile_ExpandOpaque, code,
            0x84000000 | size, (volatile u32 *)0x040000d4);
    }
    ((void (*)(u8 *, u8 *, u32, u32))Data_03001e50[ROUTINE_BLOCK])(
        glyph, glyph + 0x400, *(u16 *)(glyph + 0x600), *(u16 *)(glyph + 0x602));
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}

void UiGlyph_ResetWorkState(void)
{
    struct GlyphWork *work;
    struct GlyphVisual *visual;
    const u8 *tbl;
    s32 i;
    u16 clear = 0;

    work = Runtime_AllocateBlock(18, 996);
    work->field_348 = clear;
    work->field_34c = clear;
    work->field_350 = clear;
    work->field_39a = clear;
    work->field_39c = clear;
    work->field_39e = 128;
    work->field_3a0 = 32;
    work->field_394 = clear;
    work->field_3b8 = 999;
    i = 0;
    do {
        work->rows[i + 2].index.cursor = 0;
        work->rows[i + 9].index.cursor = 0;
        i++;
    } while (i != 5);
    ResetCursor(&work->rows[7].index);
    ResetCursor(&work->rows[8].index);
    work->rows[0].index.cursor = 0;
    work->rows[1].index.cursor = 0;
    work->rows[0].state = 0;
    work->rows[1].state = 0;
    tbl = Data_080346f8;
    work->slot = Resource_FindFreeEntry();
    work->tile = VramBlock_LoadCached(work->slot, 256, tbl);
    work->field_2e2 = 0;
    work->field_2fa = 0;
    work->field_316 = 0;
    visual = &work->visual;
    visual->flags05_2 = 0;
    visual->flags05_4 = 0;
    visual->flags05_5 = 1;
    visual->flags05_0 = 0;
    visual->flags07_1 = 0;
    visual->flags07_6 = 1;
    visual->flags05_6 = 0;
    visual->flags09_2 = 0;
}
