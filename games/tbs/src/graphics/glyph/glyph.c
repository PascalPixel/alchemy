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
#include "types.h"
#include "glyph.h"
#include "battle_unit.h"
#include "resource.h"
#include "scene.h"
#include "btlsys.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "affine.h"
#include "node_chain.h"
#include "text/msg_ids.h"

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined);

s32 Ui_CountIconTableEntries(void)
{
    return UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers;
}

s32 Ui_CountSecondTableEntries(void)
{
    return UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers;
}

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    work->encoded = UiIcon_FramePointerTable[no1];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    work->encoded = UiIcon_OverlayPointerTable[no0];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (flag == 0) {
        *slot = Resource_FindFreeEntry();
    }
    *ret = VramBlock_LoadCached(*slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
}

void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
    UiIcon_BuildItemIconTiles(BtlActGet(action)->unknown_04[0],
        with_base, slot, tile, reuse);
}

struct ItemDefinition *Item_GetDirectFar(s32 item);

extern void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 slot);

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    GlyphTransfer *work;
    s32 slot;

    slot = 0;
    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));

    if (glyph >= Ui_CountIconTableEntries())
        glyph = 0;

    if (with_base != 0) {
        work->encoded = UiIcon_FramePointerTable[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->encoded = UiIcon_ItemIconPointers[glyph];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
}

extern GlyphTransfer *gGlyphWork;

#define ICON_FRAME 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

s32 ItemIcon_Compose(u32 code, u32 layers)
{
    s32 overlay;
    struct ItemDefinition *item;
    GlyphTransfer *work;
    s32 count;

    overlay = 0;
    count = 0;
    item = Item_GetDirectFar(code & 0x1ff);
    work = gGlyphWork;
    if (work == NULL)
        return -1;
    if (layers & ICON_FRAME) {
        work->encoded = UiIcon_FramePointerTable[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        overlay = 1;
    }
    work->encoded = UiIcon_ItemIconPointers[item->icon];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, overlay);
    if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
        work->encoded = UiIcon_MarkPointers[1];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if ((layers & ICON_BROKEN) && (code & 0x200)) {
        work->encoded = UiIcon_MarkPointers[0];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) && (item->flags & 2)) {
        work->encoded = UiIcon_MarkPointers[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if (layers & ICON_COUNT_ABOVE_ONE) {
        count = (code & 0xf800) >> 11;
        count++;
        if (count <= 1)
            count = 0;
    }
    if (layers & ICON_COUNT) {
        count = (code & 0xf800) >> 11;
        count++;
    }
    if (count != 0 && count <= 30) {
        s32 ones = count % 10;

        work->encoded = UiIcon_DigitPointers[ones];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
        if (count / 10 != 0) {
            work->encoded = UiIcon_DigitPointers[count / 10 + 9];
            work->width = 2;
            work->height = 2;
            UiGlyph_DecodeWithHeapRoutines(work, 1);
        }
    }
    return 256;
}

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    ItemIcon_Compose(arg0, arg1);
    VramBlock_LoadCached(arg2, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return 1;
}

s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    Ui_PrepareTransferFromTableEntry(icon);
    VramBlock_LoadCached(slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return 1;
}

/* FAKEMATCH: three more names of the one table keep each case's own literal slot; one name folds the switch to a single load. */
extern u8 RenderResource_PairSourceTable[], PairSource1[] __asm__("RenderResource_PairSourceTable"), PairSource2[] __asm__("RenderResource_PairSourceTable"), PairSource3[] __asm__("RenderResource_PairSourceTable");

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *, s32);

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot)
{
    void *source;
    switch (value) {
    case 1:
        source = PairSource1;
        break;
    case 2:
        source = PairSource2;
        break;
    case 3:
        source = PairSource3;
        break;
    case 0:
    default:
        source = RenderResource_PairSourceTable;
        break;
    }
    VramBlock_LoadCached(slot, 32, source);
    return 1;
}

void Ui_PrepareTransferForItem(s32 code)
{
    GlyphTransfer *state = gGlyphWork;
    struct ItemDefinition *item = Item_GetDirectFar(0x1ff & code);

    if (code != 0) {
        state->encoded = UiIcon_ItemIconPointers[item->icon];
    } else {
        state->encoded = UiIcon_ItemIconPointers[0];
    }
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
    UiIcon_BuildAbilityIconTiles(BtlActGet(action)->unknown_04[0],
        with_base, slot, tile, reuse);
}

/* FAKEMATCH: needed to match: this file reads UiIcon_FramePointerTable through its own declaration; the shop's changes its code. */
extern u8 *UiIcon_FramePointerTable[];

extern void *gWorkSlot[64];

#define ROUTINE_BLOCK 49

struct GlyphCursor { u16 unused; u16 cursor; };

extern const u8 Menu_AnimatedCursorTiles[];

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
    /* FAKEMATCH: needed to match: keep the cursor-subrecord base for the paired row stores. */
    cursor->cursor = 0;
}

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

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind)
{
    /* FAKEMATCH: the separate scalar scans retain their running addresses and 88 bytes; a shared typed loop produces 56. */
    s32 state;
    s32 off;
    s32 i;
    s32 j;
    u32 ret;
    u16 *q;
    u16 *p;
    u32 v;

    state = (s32)gResQueueWork;
    if (kind != 0) {
        i = 0;
        p = &((struct SelectionScreen *)state)->records[9].kind;
        off = 0;
loop_2:
        if (*p == 0) {
            return (struct SelectionNode *)(state + off + 0x1d4);
        }
        i = i + 1;
        p += 0x1A;
        off = off + 0x34;
        if (i == 5) {
            goto block_10;
        }
        goto loop_2;
    }
    j = 0;
    ret = state + 0x68;
    q = &((struct SelectionScreen *)state)->records[2].kind;
loop_7:
    v = *q;
    q += 0x1A;
    if (v == 0) {
        return (struct SelectionNode *)ret;
    }
    ret += 0x34;
    j += 1;
    if (j == 7) {
block_10:
        return 0;
    }
    goto loop_7;
}

void Resource_ScheduleOwnerResetDelayed(void)
{
    Scheduler_AddOrUpdateCallback(MenuSelection_DrawFrame, 0xC80);
}

void Resource_ScheduleOwnerReset(void)
{
    Scheduler_RemoveCallback(MenuSelection_DrawFrame);
}

s32 GameFlag_TestFar(s32 flag);

#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct UiWindow *, s32, s32))UiText_DrawCharacterAtOffset)
#define Menu_AnimatedCursorTiles ((u8 *)&Menu_AnimatedCursorTiles)

#define Menu_CursorObjectTiles ((u8 (*))&Menu_CursorObjectTiles)

extern u32 gFrameTick;

void RenderOutput_PrepareForRedraw(struct UiWindow *);

struct Work;
void WaitFrames(s32);

struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

void MenuSelection_DrawFrame(void)
{
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionSprite *cursor_entry = &state->records[14].sprite;
    struct SelectionNode *cursor = &state->records[14];
    struct SelectionNode *transfer = &state->records[15];
    struct SelectionNode *node;
    struct SelectionSprite *e;
    struct SelectionSprite *te;
    s32 cnt = 0;
    s32 slot;
    s32 tmp;
    struct EffectPosition pos;

    node = state->head;
    while (node != 0) {
        e = &node->sprite;
        e->affine = 0;
        e->param = 0;
        e->x = node->x;
        e->y = node->y;
        slot = 240;
        if (state->vertical_scroll != 0) {
            e->y += state->vertical_scroll;
        } else if (node->x != node->target_x) {
            if (node->dx > 0) {
                if (node->x + node->dx > node->target_x) {
                    node->x = node->target_x;
                } else {
                    node->x = node->x + node->dx;
                }
            } else {
                if (node->x + node->dx < node->target_x) {
                    node->x = node->target_x;
                } else {
                    node->x = node->x + node->dx;
                }
            }
            e->x = node->x;
        } else if (cnt == state->cursor_index) {
            slot = 241;
            if (cursor->kind != 0) {
                if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1) {
                    cursor->target_x = pos.x;
                    cursor->target_y = pos.y;
                    if (cursor->scale == 0) {
                        cursor->x = pos.x;
                        cursor->y = pos.y;
                        cursor->scale = 1;
                    }
                }
            }
        }
        if (node->scale != 0) {
            if (GameFlag_TestFar(0x103) != 0) {
                if (state->records[14].kind == 1) {
                    e->mode = 1;
                } else {
                    e->mode = 0;
                }
                if (node->kind == 1) {
                    e->mode = 1;
                }
            }
            Runtime_PushSlotEntry((s32 *)e, slot);
        }
        node = node->next;
        cnt++;
    }
    if (transfer->kind != 0) {
        node = NodeChain_GetNodeAtCount(state);
        te = &transfer->sprite;
        te->mode = 0;
        te->affine = 0;
        te->param = 0;
        te->mosaic = 0;
        te->colors = 1;
        te->size = 2;
        te->shape = 0;
        te->priority = 0;
        te->tile = transfer->tile;
        te->x = node->x - 4;
        te->y = node->y + (Menu_CursorBobOffsets[(gFrameTick >> 1) & 15] >> 1) - 4;
        if (transfer->scale != transfer->scale_end) {
            state->matrix[0] = transfer->scale;
            state->matrix[1] = transfer->scale;
            state->matrix[2] = 0;
            te->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
            te->affine = 3;
            te->x = te->x + 0xfff0;
            te->y = te->y + 0xf0;
            transfer->scale = transfer->scale + transfer->scale_step;
        }
        if (GameFlag_TestFar(0x103) != 0) {
            te->mode = 1;
        }
        Runtime_PushSlotEntry((s32 *)te, 248);
    }
    MenuSelection_DrawSideMarker(state, 0);
    MenuSelection_DrawSideMarker(state, 1);
    node = state->path;
    while (node != 0) {
        e = &node->sprite;
        if (node->x != node->target_x) {
            node->x = node->x + node->dx;
        }
        if (node->y != node->target_y) {
            node->y = node->y + node->dy;
        }
        e->x = node->x;
        e->y = node->y;
        if (node->scale != node->scale_end) {
            node->scale = node->scale + node->scale_step;
            state->matrix[0] = node->scale;
            state->matrix[1] = node->scale;
            state->matrix[2] = 0;
            e->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
            e->affine = 3;
            e->x = e->x + 0xfff8;
            e->y = e->y + 0xf8;
        } else {
            e->affine = 0;
            e->param = 0;
        }
        if (GameFlag_TestFar(0x103) != 0) {
            if (state->records[14].kind == 1) {
                e->mode = 1;
            } else {
                e->mode = 0;
            }
            if (node->kind == 1) {
                e->mode = 1;
            }
        }
        Runtime_PushSlotEntry((s32 *)e, 240);
        node = node->next;
    }
    if (cursor->kind != 0) {
        cnt = (s32)Menu_AnimatedCursorTiles;
        {
            void *src = (void *)(cnt + (((gFrameTick >> 2) & 15) << 8));

            cursor_entry->tile = VramBlock_LoadCached(cursor->slot, 0x100, src);
        }
        if (cursor->target_x != cursor->x) {
            tmp = (cursor->target_x - cursor->x) >> 1;
            if (tmp != 0) {
                cursor->x = cursor->x + tmp;
            } else {
                cursor->x = cursor->target_x;
            }
        }
        if (cursor->target_y != cursor->y) {
            if ((tmp = (cursor->target_y - cursor->y) >> 1) != 0) {
                cursor->y = cursor->y + tmp;
            } else {
                cursor->y = cursor->target_y;
            }
        }
        cursor_entry->y = cursor->y + Menu_CursorBobOffsets[(gFrameTick >> 2) & 15] - 32;
        cursor_entry->x = cursor->x - 4;
        if (GameFlag_TestFar(0x103) != 0) {
            if (state->records[14].kind == 1) {
                cursor_entry->mode = 1;
            } else {
                cursor_entry->mode = 0;
            }
        }
        Runtime_PushSlotEntry((s32 *)cursor_entry, 248);
    }
    state->status += 1;
}

void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index)
{
    u32 frame = (gFrameTick >> 2) & 7;
    struct SelectionSprite *entry;
    u8 *frames;
    u8 *tmp;

    if (state->records[index].kind == 0)
        return;
    entry = &state->records[index].sprite;
    entry->x = state->records[index].x;
    entry->y = state->records[index].y;
    if (index != 0) {
        frames = Menu_CursorObjectTiles;
        if (state->records[1].base != 0)
            entry->x = entry->x + state->records[1].base;
    } else {
        frames = Menu_CursorLeftObjectTiles;
        if (state->records[0].base != 0)
            entry->x = entry->x - state->records[0].base;
    }
    tmp = frames + frame * 128;
    entry->tile = VramBlock_LoadCached(state->records[index].slot, 128, tmp);
    if (GameFlag_TestFar(0x103)) {
        if (state->records[14].kind == 1)
            entry->mode = 1;
        else
            entry->mode = 0;
    }
    Runtime_PushSlotEntry((s32 *)entry, 238);
    if (state->records[index].base != 0)
        state->records[index].base--;
}

void Menu_OpenSelectionWindow(s32 mode, u32 count)
{
    struct SelectionScreen *screen;
    struct SelectionNode *node;
    struct UiWindow **slot;
    struct UiWindow *window;

    screen = gResQueueWork;
    node = NodeChain_GetNodeAtCount(screen);
    slot = &screen->window;
    window = *slot;
    if (window == 0) {
        if (mode == 6) {
            if (screen->locked != 0) {
                *slot = UiWindow_Create(17, 17, 5, 3, mode);
            } else {
                *slot = UiWindow_Create(17, 0, 5, 3, mode);
            }
            screen->vertical_scroll = 0;
            screen->locked = 999;
        } else {
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    } else {
        if (count != 0 && window->width != count + 2) {
            UiWork_Finalize(window, 2);
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    }
    if (screen->count != 0) {
        UiText_DrawCharacterAtOffset(node->message, screen->window, 0, 0);
    } else {
        switch (mode) {
        case 4:
            UiText_DrawCharacterAtOffset((s32)MsgAbilityNotKnown, screen->window, 0, 0);
            break;
        case 2:
            UiText_DrawCharacterAtOffset((s32)MsgItemNotHeld, screen->window, 0, 0);
            break;
        }
    }
}

void Resource_ResetOwnerEntries(void)
{
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionNode *node;

    Resource_ScheduleOwnerReset();
    UiWork_Finalize(state->window, 2);
    WaitFrames(1);
    node = state->head;
    while (node != 0) {
        if (node->kind != 0) {
            Resource_ResetEntry(node->slot);
            node->kind = 0;
        }
        node = node->next;
    }
    node = state->path;
    while (node != 0) {
        if (node->kind != 0) {
            Resource_ResetEntry(node->slot);
            node->kind = 0;
        }
        node = node->next;
    }
    Resource_ResetPendingTransfer();
    if (state->records[0].y != 0) {
        Resource_ResetEntry(state->records[0].slot);
        if (state->records[0].y != 0) {
            Resource_ResetEntry(state->records[1].slot);
        }
    }
    Resource_ResetEntry(state->records[14].slot);
    Runtime_ReleaseHeapBlock(18);
}

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionNode *node;

    state->base_x = first;
    state->base_y = second;
    node = state->head;
    while (node != 0) {
        node->x = first;
        node->target_x = first;
        node->y = second;
        node->target_y = second;
        node = node->next;
        first += 16;
    }
}

void Menu_SetupSelectionBothSides(void)
{
    struct SelectionScreen *state;

    state = (struct SelectionScreen *)gResQueueWork;
    Menu_SetupSelectionSide(state, 0);
    Menu_SetupSelectionSide(state, 1);
}

void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index)
{
    struct SelectionSprite *entry = &state->records[index].sprite;
    u8 *frames = 0;
    u32 count;

    state->records[index].kind = 0;
    if (index != 0) {
        /* FAKEMATCH: the separate local preserves the early r5 load and late copy; direct assignment changes later node registers. */
        u8 *right = Menu_CursorObjectTiles;

        count = state->count;
        if (state->top != 0)
            count -= state->top;
        if (count > 5) {
            state->records[index].kind = 1;
            count = 5;
        }
        frames = right;
        state->records[1].x = state->base_x + 16 * (count - 1) + 17;
    } else {
        frames = Menu_CursorLeftObjectTiles;
        state->records[0].x = state->base_x - 9;
        if (state->top != 0)
            state->records[0].kind = 1;
    }
    if (state->records[index].y == 0) {
        state->records[index].slot = Resource_FindFreeEntry();
        state->records[index].tile = VramBlock_LoadCached(state->records[index].slot, 128, frames);
        state->records[index].y = state->base_y;
        state->records[index].base = 0;
        entry->mode = 0;
        entry->mosaic = 0;
        entry->colors = 1;
        entry->affine = 0;
        entry->param = 0;
        entry->size = 0;
        entry->shape = 2;
        entry->priority = 0;
    }
}
