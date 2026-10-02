#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"

extern u8 RomBytes_080308a0[];

/* ui/icon/build_ability_icon_tiles.c */
typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern FontTransfer *Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
extern s32 VramBlock_LoadCached(s32 index, s32 size, u8 *destination);
extern s32 RomBytes_08029a10[];
extern s32 UiIcon_PsynergyIconPointers[];

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gGlyphWork;
extern u32 UiIcon_MiscIconPointers[];

typedef struct {
    u8 input[0x400];
    u8 tiles[0x200];
    s16 width;
    s16 height;
    u8 *encoded;
} GlyphTransfer;

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

extern const u8 Menu_AnimatedCursorTiles[];
struct GlyphWork *Runtime_AllocateBlock(s32 kind, s32 size);
s32 Resource_FindFreeEntry(void);

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
    /* FAKEMATCH: keep the cursor-subrecord base for the paired row stores. */
    cursor->cursor = 0;
}

void UiGlyph_DecodeWithHeapRoutines(u8 *glyph, s32 outlined);

extern u8 Data_03001e98[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct State_0801a7c0 {
    u8 filler0[0x354];
    u16 first[16];
    u16 second[16];
    u16 cnt;
};

extern struct State_0801a7c0 *volatile gResQueueWork;

struct SelectionNode {
    struct SelectionNode *prev;
    struct SelectionNode *next;
    s16 base;
    u16 kind;
    u8 unknown_0c[4];
    u16 x;
    u16 y;
    u16 unknown_14;
    u16 unknown_16;
    u16 draw_x;
    u16 draw_y;
    u8 unknown_1c[0x34 - 0x1c];
};

struct SelectionScreen {
    u8 unknown_000[0x68];
    struct SelectionNode nodes[7];
    struct SelectionNode others[5];
    u8 unknown_2d8[0x348 - 0x2d8];
    struct SelectionNode *head;
    u8 unknown_34c[0x354 - 0x34c];
    u16 kinds[16];
    u16 bases[16];
    u16 count;
    u16 x;
    u16 y;
    u16 unknown_39a;
    u16 first;
    u8 unknown_39e[0x3b8 - 0x39e];
    u16 f3b8;
};

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
void Menu_LoadSelectedResource(void);

void UiGlyph_DecodeWithHeapRoutines(u8 *glyph, s32 outlined);

/* ui/icon/build_ability_icon_tiles.c */
/* ui/icon/icon_build_ability_icon_tiles.c */
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    slot = 0;

    if (glyph >= Ui_CountSecondTableEntries())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = RomBytes_08029a10[2];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->f604 = UiIcon_PsynergyIconPointers[glyph];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, &work->f400);
    Runtime_ReleaseHeapBlock(0x11);
}

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    struct State_0801a4c0 *state = gGlyphWork;

    state->value = UiIcon_MiscIconPointers[index];
    state->first = 2;
    state->second = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

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

/* Complete 268-byte glyph-work initializer, including its literal pool.
 * The cursor subrecord is shared by the paired row resets. */
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
    tbl = Menu_AnimatedCursorTiles;
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

void Resource_ClearOwnerListAndCounters(void)
{
    void *state;

    state = *(void **)((u32)&Data_03001e98);
    FIELD_AT_OFFSET(state, s32 *, 0x348) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x39A) = 0;
    if (0x80 & FIELD_AT_OFFSET(state, u16 *, 0x39E)) {
        FIELD_AT_OFFSET(state, s16 *, 0x39C) = 0;
        FIELD_AT_OFFSET(state, u16 *, 0x39E) = 0U;
    }
    FIELD_AT_OFFSET(state, s16 *, 0x3A0) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x394) = 0;
}

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct State_0801a7c0 *state = gResQueueWork;
    u16 cnt = state->cnt;

    if (cnt != 16) {
        state->first[cnt] = first;
        state->second[cnt] = second;
        state->cnt++;
    }
}

/* Link up to five of the selection screen's options into its node list,
   then place the nodes around the screen's centred x. */
void MenuSelection_BuildEntries(void)
{
    struct SelectionScreen *screen = gResQueueWork;
    u32 count = screen->count;
    u32 index = screen->first;
    struct SelectionNode *prev = 0;
    struct SelectionNode *node;
    s32 cnt = 0;

    while (index < count) {
        s32 base = screen->bases[index];
        u32 kind = screen->kinds[index];

        node = Resource_FindFreeTransferEntry(0);
        if (node == 0)
            break;
        MenuSelection_SetupEntry(kind, base, node, 0);
        if (screen->head == 0) {
            screen->head = node;
            node->prev = 0;
        } else {
            prev->next = node;
            node->prev = prev;
        }
        node->next = 0;
        cnt++;
        prev = node;
        if (cnt == 5)
            break;
        index++;
    }

    screen->x = 100 - cnt * 8;
    screen->y = 140;
    for (prev = screen->head, cnt = 0; prev != 0; prev = prev->next) {
        s32 x = screen->x + cnt;
        s32 y;

        prev->x = x;
        y = screen->y;
        prev->y = y;
        prev->draw_x = x;
        prev->draw_y = y;
        if (prev->kind == 6 && screen->f3b8 == 0) {
            prev->y = 6;
            prev->draw_y = 6;
        }
        prev->unknown_14 = 0;
        prev->unknown_16 = 0;
        cnt += 16;
    }
    Menu_LoadSelectedResource();
}

void UiWork_ReservedNoOp(void)
{
}
