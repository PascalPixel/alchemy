#include "types.h"
/* FAKEMATCH: needed to match: this file reads UiIcon_FramePointerTable through its own declaration; the shop's changes its code. */
extern u8 *UiIcon_FramePointerTable[];
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "select.h"
#include "glyph.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "resource.h"
#include "dma.h"
#include "resource_ids.h"
#include "io_reg.h"




extern GlyphTransfer *gGlyphWork;


extern void *gWorkSlot[64];

#define ROUTINE_BLOCK 49

struct GlyphCursor { u16 unused; u16 cursor; };

extern const u8 Menu_AnimatedCursorTiles[];

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
    /* FAKEMATCH: needed to match: keep the cursor-subrecord base for the paired row stores. */
    cursor->cursor = 0;
}

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined);



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
        work->encoded = UiIcon_FramePointerTable[2];
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
    Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(palette, 0), 0x80000010, REG_DMA3);
}

void UiGlyph_ReservedNoOp(void)
{
}

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined)
{
    void *code;

    {
        u32 size;

        /* FAKEMATCH: needed to match: retain the size load after saving both arguments. */
        do {
            size = (u32)Tile_Decompress4bppCodeSize;
        } while (0);
        code = (u8 *)Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Tile_Decompress4bpp, code,
            0x84000000 | size, REG_DMA3);
    }
    ((void (*)(const void *, u8 *))gWorkSlot[ROUTINE_BLOCK])(
        glyph->encoded, glyph->input);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
    if (outlined) {
        u32 size;

        size = (u32)Tile_ExpandMaskedCodeSize;
        code = (u8 *)Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Tile_ExpandMasked, code,
            0x84000000 | size, REG_DMA3);
    } else {
        u32 size;

        size = (u32)Tile_ExpandOpaqueCodeSize;
        code = (u8 *)Runtime_AllocateHeapBlock(ROUTINE_BLOCK, size);
        size >>= 2;
        Dma_Set((const void *)Tile_ExpandOpaque, code,
            0x84000000 | size, REG_DMA3);
    }
    ((void (*)(u8 *, u8 *, u32, u32))gWorkSlot[ROUTINE_BLOCK])(
        glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
    Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}

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
