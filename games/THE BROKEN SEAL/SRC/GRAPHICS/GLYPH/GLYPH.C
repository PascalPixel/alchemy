#include "SELECT.H"
#include "GLYPH.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"

extern u8 RomBytes_080308a0[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
extern u8 *RomBytes_08029a10[];
extern u8 *UiIcon_PsynergyIconPointers[];

extern GlyphTransfer *gGlyphWork;
extern u8 *UiIcon_MiscIconPointers[];


void Runtime_ReleaseHeapBlock(s32 kind);
extern const u8 Tile_Decompress4bpp[];
extern const u8 Tile_ExpandMasked[];
extern const u8 Tile_ExpandOpaque[];

/* Heap block addresses, indexed by block number. */
extern void *Data_03001e50[];

/* ARM routines copied into heap block 49 before each call. */
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];
#define ROUTINE_BLOCK 49

struct GlyphCursor { u16 unused; u16 cursor; };

extern const u8 Menu_AnimatedCursorTiles[];
void *Runtime_AllocateBlock(s32 kind, s32 size);
s32 Resource_FindFreeEntry(void);

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
    /* FAKEMATCH: keep the cursor-subrecord base for the paired row stores. */
    cursor->cursor = 0;
}

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined);

extern u8 Data_03001e98[];

extern struct SelectionScreen *volatile gResQueueWork;

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
void Menu_LoadSelectedResource(void);

/* ui/icon/build_ability_icon_tiles.c */
/* ui/icon/icon_build_ability_icon_tiles.c */
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    GlyphTransfer *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    slot = 0;

    if (glyph >= Ui_CountSecondTableEntries())
        glyph = 0;

    if (with_base != 0) {
        work->encoded = RomBytes_08029a10[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->encoded = UiIcon_PsynergyIconPointers[glyph];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
}

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    GlyphTransfer *state = gGlyphWork;

    state->encoded = UiIcon_MiscIconPointers[index];
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse)
{
    GlyphTransfer *work;
    u16 *table;
    u8 *entry;
    u32 index;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
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

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined)
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
        glyph->encoded, glyph->input);
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
        glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}

/* Complete 268-byte glyph-work initializer, including its literal pool.
 * The cursor subrecord is shared by the paired row resets. */
void UiGlyph_ResetWorkState(void)
{
    struct SelectionScreen *work;
    struct SelectionSprite *visual;
    const u8 *tbl;
    s32 i;
    u16 clear = 0;

    work = Runtime_AllocateBlock(18, sizeof(struct SelectionScreen));
    work->head = NULL;
    work->path = NULL;
    work->window = NULL;
    work->unknown_39a = clear;
    work->top = clear;
    work->cursor_index = 128;
    work->vertical_scroll = 32;
    work->count = clear;
    work->locked = 999;
    i = 0;
    do {
        work->records[i + 2].kind = 0;
        work->records[i + 9].kind = 0;
        i++;
    } while (i != 5);
    ResetCursor((struct GlyphCursor *)&work->records[7].base);
    ResetCursor((struct GlyphCursor *)&work->records[8].base);
    work->records[0].kind = 0;
    work->records[1].kind = 0;
    work->records[0].y = 0;
    work->records[1].y = 0;
    tbl = Menu_AnimatedCursorTiles;
    work->records[14].slot = Resource_FindFreeEntry();
    work->records[14].tile = VramBlock_LoadCached(work->records[14].slot, 256, tbl);
    work->records[14].kind = 0;
    work->records[14].scale = 0;
    work->records[15].kind = 0;
    visual = &work->records[14].sprite;
    visual->mode = 0;
    visual->mosaic = 0;
    visual->colors = 1;
    visual->affine = 0;
    visual->param = 0;
    visual->size = 1;
    visual->shape = 0;
    visual->priority = 0;
}

void Resource_ClearOwnerListAndCounters(void)
{
    struct SelectionScreen *screen;

    screen = gResQueueWork;
    screen->head = NULL;
    screen->unknown_39a = 0;
    if (screen->cursor_index & 0x80) {
        screen->top = 0;
        screen->cursor_index = 0;
    }
    screen->vertical_scroll = 0;
    screen->count = 0;
}

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct SelectionScreen *state = gResQueueWork;
    u16 cnt = state->count;

    if (cnt != 16) {
        state->kinds[cnt] = first;
        state->bases[cnt] = second;
        state->count++;
    }
}

/* Link up to five of the selection screen's options into its node list,
   then place the nodes around the screen's centred x. */
void MenuSelection_BuildEntries(void)
{
    struct SelectionScreen *screen = gResQueueWork;
    u32 count = screen->count;
    u32 index = screen->top;
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

    screen->base_x = 100 - cnt * 8;
    screen->base_y = 140;
    for (prev = screen->head, cnt = 0; prev != 0; prev = prev->next) {
        s32 x = screen->base_x + cnt;
        s32 y;

        prev->x = x;
        y = screen->base_y;
        prev->y = y;
        prev->target_x = x;
        prev->target_y = y;
        if (prev->kind == 6 && screen->locked == 0) {
            prev->y = 6;
            prev->target_y = 6;
        }
        prev->dx = 0;
        prev->dy = 0;
        cnt += 16;
    }
    Menu_LoadSelectedResource();
}

void UiWork_ReservedNoOp(void)
{
}
