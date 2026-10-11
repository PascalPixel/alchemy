#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "window.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"

extern void UiWork_InitFreeList(void);

#if !EDITION_INTERNATIONAL
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: needed to match: in Japanese, a direct call loads the fill value before the routine and reverses pool order. */
    return fill(dst, size, value);
}
#endif

void UiWork_InitializeWithResourceCounters(void)
{
    struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
    volatile u32 fill;
#endif

    work = (struct UiRenderWork *)Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x850004bf, REG_DMA3);
#else
    Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
    work->dirty = 1;
    work->glyph_resource = 99;
    work->level = 15;
#if EDITION_INTERNATIONAL
    fill = 0xf000f000;
    Dma_Set((const void *)&fill, work, 0x85000140, REG_DMA3);
#else
    FillWords(Iwram_FillWords, work, 0x500, 0xf000f000);
#endif
    UiWork_InitFreeList();
    UiWork_SetTwoEntriesTo999();
    Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
    UiWork_InitCountersWithResourceAndScheduleRefresh();
}

void UiWork_Initialize(s32 kind)
{
    struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
    volatile u32 fill;
#endif
    s32 i;
    s32 UiWork_CopyTileTail(u32 src, u32 dst)
    {
        src &= 0x3ff;
        dst &= 0x3ff;
        Dma_Set((void *)(0x06000010 + src * 32),
                (void *)(0x06000000 + dst * 32),
                0x80000008, REG_DMA3);
        return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
    }

    work = (struct UiRenderWork *)Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
    fill = 0;
    Dma_Set(&fill, work, (0x85000000 | RENDER_WORK_SIZE / 4), REG_DMA3);
#else
    Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
    work->dirty = 1;
    work->glyph_resource = 99;
    work->menu_state = 1;
    work->level = 15;
#if EDITION_INTERNATIONAL
    fill = 0xf000f000;
    Dma_Set(&fill, work, 0x85000140, REG_DMA3);
#else
    FillWords(Iwram_FillWords, work, 0x500, 0xf000f000);
#endif
    UiWork_InitFreeList();
    Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
    UiWork_InitCountersAndScheduleRefresh(kind);
    UiWork_CopyTileTail(0xf013, 128);
    UiWork_CopyTileTail(0xf014, 129);
    UiWork_CopyTileTail(0xf015, 130);
    {
        u8 mode = 4;

        for (i = 2; i >= 0; i--)
            work->tile_attributes[i] = mode;
    }
}

void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
typedef s32 (*UiFillFn)(void *dst, s32 size, u32 value);

void UiWork_ResetCounters();

void RenderOutput_PrepareForRedraw(struct UiWindow *window);

void UiWork_UploadDirtyBlocks(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 flags;
    u8 *src;
    u8 *dst;
    if (!work->menu_busy) {
        flags = work->dirty;
        if (flags) {
            dst = (u8 *)0x06002000;
            src = (u8 *)work->tilemap;
            if (flags & 1) flags = 63;
            flags &= 63;
            flags >>= 1;
            do {
                if (flags & 1) Dma_Set(src, dst, 0x84000040, REG_DMA3);
                flags >>= 1;
                src += 256;
                dst += 256;
            } while (flags);
            work->dirty = flags;
        }
    }
}

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
    s32 tile;
    u32 bottom;
    u32 row;
    u8 *mode;

    tile = 240;
    bottom = y + height;
    tile <<= 8;
    if (bottom > 20)
        height = 20 - y;
    if (width <= 1)
        width = 2;
    if (width > 30)
        width = 30;
    if (height <= 1)
        height = 2;
    if (height > 30)
        height = 30;

    UiWindow_ClearTileAttributesInRect(x, y, width, height);

    row = 0;
    if (row < height) {
        mode = &canvas->menu_state;
        do {
            u32 column;

            if (*mode != 0) {
                if ((y + row) > 16)
                    tile = 0xF07F;
                else
                    tile = 0xF000;
            }
            column = 0;
            if (column < width) {
                do {
                    *cursor++ = tile;
                    column++;
                } while (column < width);
            }
            row++;
            cursor += 32 - width;
        } while (row < height);
    }
    canvas->dirty = 1;
}

void UiWork_DrawByAttributes(struct UiWindow *window)
{
    u32 flags;
    u32 value;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    void *dst;
    UiFillFn fill;
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)gWindowWork[0];
    value = window->height;
    flags = window->flags;
    height = value;
    value = 0;
    window->duration = value;
    x = window->x;
    y = window->y;
    width = window->width;
    if (8 & flags) {
        if (0x20 & flags) {
            UiWindow_DrawFrame(x, y, width, height);
            fill = Iwram_FillWords;
            dst = (void *)0x06002500;
            fill(dst, 0xF00, 0x44444444);
        } else {
            fill = Iwram_FillWords;
            dst = (void *)0x06002500;
            fill(dst, 0xF00, 0);
        }
        UiWindow_MapTextCanvasTiles(x, y, width, height, 0);
    } else {
        UiWindow_DrawFrame(x, y, width, height);
    }
    work->dirty = 1;
}

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
    struct UiWindow *slot;
    struct UiWindow *found;
    s32 i;

    slot = ((struct UiRenderWork *)gWindowWork[0])->windows;
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->duration != 0) {
        i++;
        slot++;
        if (i == WINDOW_COUNT) {
            goto done;
        }
    }
    found = slot;
done:
    if (found != 0) {
        found->y = y;
        found->width = width;
        found->height = height;
        found->x = x;
        found->output.head = NULL;
        found->state = 0;
        found->output.tail_link = &slot->output.head;
        found->unknown_10 = 1;
        found->flags = 1;
        UiWork_ResetCounters(x);
        if (attrs & 8) {
            found->flags |= 8;
        }
        if (attrs & 32) {
            found->flags |= 32;
        }
        if (attrs & 64) {
            found->flags |= 64;
        }
        if (attrs & 128) {
            found->flags |= 128;
        }
        if (attrs & 0x100) {
            found->flags |= 0x100;
        }
        if (attrs & 2) {
            found->flags |= 2;
            found->frame = 0;
            found->duration = 1;
            UiWork_DrawByAttributes(found);
        } else {
            found->duration = 8;
            found->frame = 7;
            UiWork_WaitWindowDuration(found);
            WaitFrames(1);
        }
    }
    return found;
}

void UiWork_WaitWindowDuration(struct UiWindow *window)
{
    if (!(2 & window->flags) && window->duration != 0) {
        do {
            WaitFrames(1);
        } while (window->duration != 0);
    }
}

void UiWork_Finalize(struct UiWindow *work, s32 release)
{
    u16 zero;

    if (work == 0)
        return;

    RenderOutput_PrepareForRedraw(work);
    work->previous_x = work->x;
    work->previous_y = work->y;
    work->previous_width = work->width;
    zero = 0;
    work->flags = zero;
    work->previous_height = work->height;

    if (release != 0) {
        UiWindow_EraseBorderRect(work->x, work->y, work->width, work->height);
        work->output.head = NULL;
        work->output.tail_link = NULL;
        work->width = zero;
        work->height = zero;
        work->x = zero;
        work->y = zero;
        work->unknown_10 = zero;
        work->effect = zero;
        work->state = zero;
        work->flags = zero;
        work->frame = zero;
        work->duration = zero;
        work->previous_x = zero;
        work->previous_y = zero;
        work->previous_width = zero;
        work->previous_height = zero;
    } else {
        work->frame = release;
        work->duration = 4;
    }
}

void RenderOutput_PrepareForRedraw(struct UiWindow *window)
{
    if (!(8 & window->flags)) {
        RenderOutput_RedrawSavedRect(window);
        RenderOutput_ClearList(window);
    }
}

void RenderOutput_RedrawSavedRect(struct UiWindow *window)
{
    UiWindow_DrawFrame(window->x, window->y, window->width, window->height);
}

void RenderOutput_ClearList(void *work)
{
    struct RenderOutputList *list = work;
    struct RenderOutput *node;
    struct RenderOutput *next;

    next = NULL;
    if (list != NULL) {
        node = list->head;
        list->tail_link = &list->head;
        list->head = next;
        while (node != NULL) {
            next = node->next;
            RenderOutput_Release(node);
            node = next;
        }
    }
}

void UiWindow_ClearInteriorTiles(const struct RenderInput *window,
    u32 left, u32 top, u32 right, u32 bottom)
{
    struct UiRenderWork *work;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u16 *tiles;
    u32 row;

    right += 7;
    bottom += 7;
    right >>= 3;
    bottom >>= 3;
    x = left >> 3;
    work = (struct UiRenderWork *)gWindowWork[0];
    y = top >> 3;
    x += window->x;
    y += window->y;
    right += window->x;
    bottom += window->y;
    left = x + 1;
    top = y + 1;
    width = right - x;
    height = bottom - y;
    UiWindow_ClearTileAttributesInRect(left, top, width, height);
    tiles = work->tilemap + top * 32 + left;
    for (row = 0; row < height; row++) {
        for (left = 0; left < width; left++)
            *tiles++ = 0xf020;
        tiles += 32 - width;
    }
    work->dirty = 1;
}

extern u8 *gWindowWork[];

void UiWork_UpdateListTail(struct RenderOutputList *list)
{
    struct RenderOutput *node = list->head;
    struct RenderOutput **tail = &list->head;

    while (node != NULL) {
        tail = &node->next;
        node = node->next;
    }
    list->tail_link = tail;
}

void RenderOutput_AppendToList(struct RenderOutputList *list, struct RenderOutput *node)
{
    /* FAKEMATCH: needed to match: the first-word tail store preserves the two stores in order; the typed tail-link field reverses them. */
    if (list != NULL) {
        *list->tail_link = node;
        *(struct RenderOutput **)&list->tail_link = node;
    }
}

void RenderOutput_Release(struct RenderOutput *entry)
{
    RenderOutput_ReleaseFree(entry);
    if ((u8)entry->kind != 0) {
        Resource_ResetEntry((u8)entry->index);
        if ((u8)entry->kind == 2) {
            u8 *dst = gWindowWork[0];
            s32 idx = entry->oam.palette * 2 + RENDER_PALETTE_TBL_OFS;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *spacing, s32 flags)
{
    struct UiChannelSlot *render = ((struct UiRenderWork *)gWindowWork[0])->channels;
    struct UiChannelSlot *found = NULL;
    u32 i;

    for (i = 0; i != 3; i++, render++) {
        if (render->work == NULL) {
            found = render;
            break;
        }
    }
    if (found != NULL) {
        found->work = window;
        found->start_x = x << 8;
        found->x = x << 8;
        found->y = y << 8;
        found->entry = entry;
        found->colour = 15;
        found->line_spacing = 10;
        found->countdown = 0;
        found->outline = 0;
        found->ended = 0;
        found->flags = flags;
        if (spacing != NULL) {
            for (i = 0; i < 4; i++)
                found->spacing[i] = *spacing++;
        } else {
            for (i = 0; i < 4; i++)
                found->spacing[i] = 0;
        }
        found->line = 0;
    }
    return found;
}

struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve)
{
    struct UiChannelSlot *slot;
    struct UiChannelSlot *selected;
    u16 *destination;
    u16 val04;
    u16 zero;
    u32 index;

    slot = ((struct UiRenderWork *)gWindowWork[0])->channels;
    selected = 0;
    for (index = 0; index != 3; slot++, index++) {
        if (slot->work == 0 || slot->work->state != 0) {
            selected = slot;
            break;
        }
    }

    if (selected != 0) {
        if (selected->work == 0) {
            selected->y = 0xA00;
            val04 = 0x300;
            selected->work = work;
            goto reset_x;
        }
        if (preserve == 0) {
            if (selected->y == 0) {
                selected->y = 0xA00;
            } else if (selected->y < 0xD00) {
                selected->y += 0xD00;
            } else {
                UiWork_ResetChannelTransition(selected);
            }
            val04 = 0x300;
reset_x:
            selected->x = val04;
        }

        zero = 0;
        selected->start_x = 0x300;
        selected->work->state = zero;
        selected->colour = 15;
        selected->line_spacing = 10;
        selected->entry = value;
        selected->countdown = zero;
        selected->outline = zero;
        selected->line = zero;
        selected->ended = zero;
        index = 0;
        destination = selected->spacing;
        do {
            index++;
            *destination++ = zero;
        } while (index <= 3);
    }
    return selected;
}

s32 Ui_ClearVramBlock(void)
{
    s32 (*fill)(void *, s32, u32) = Iwram_FillWords;

    return fill((void *)0x06002500, 0xF00, 0);
}

s32 Ui_FillVramBlockPattern(void)
{
    s32 (*fill)(void *, s32, u32) = Iwram_FillWords;

    return fill((void *)0x06002500, 0xF00, 0x44444444);
}

void UiWork_ResetFreeChannel(void)
{
    struct UiChannelSlot *slot =
        ((struct UiRenderWork *)gWindowWork[0])->channels;
    struct UiChannelSlot *sel = 0;
    s32 i;

    for (i = 0; i != 3; slot++, i++) {
        if (slot->work == 0 || slot->work->state != 0) {
            sel = slot;
            break;
        }
    }
    if (sel != 0) {
        if (sel->work != 0) {
            Ui_ClearVramBlock();
            sel->y = 0;
        }
        sel->x = 0;
        sel->countdown = 0;
        sel->colour = 0xF;
        sel->outline = 0;
        sel->line_spacing = 0xA;
    }
}

void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    work->colour = channel->colour;
    work->outline = channel->outline;
    work->line_spacing = channel->line_spacing;
}

void UiWork_ResetChannelTransition(struct UiChannelSlot *channel)
{

    channel->transition = 2;
}

#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    s32 shift = step * 6;
    u32 *base = PANEL_TILE_ROWS;
    s32 n;

    for (n = 0; n < PANEL_ROW_COUNT; n++) {
        u32 *row = base + n * PANEL_ROW_WORDS;

        Dma_Set(row + PANEL_ROW_LEAD + shift, row + PANEL_ROW_LEAD,
                0x84000000 | (PANEL_ROW_WORDS - PANEL_ROW_LEAD - shift), REG_DMA3);
        Iwram_FillWords(row + (PANEL_ROW_WORDS - shift), shift * 4, 0);
    }
}

void UiWork_ProcessRenderChannels(void)
{
    struct UiChannelSlot *channel =
        ((struct UiRenderWork *)gWindowWork[0])->channels;
    s32 channel_no = 0;
    s32 one = 1;

    do {
        struct UiWindow *current = channel->work;

        if (current != 0 && *(s32 *)&current->frame == 0) {
            u16 flags = current->flags;

            if (flags == 0) {
                channel->work = NULL;
            } else {
                s32 pending = current->effect;
                s32 kind;

                if (pending != 0) {
                    UiWork_AdvanceChannelTransition(channel);
                } else {
                    kind = UiWork_StepChannelScript(channel);
                    switch (kind) {
                    case 8:
                        channel->work->state = one;
                        break;
                    case 9:
                    {
                        struct UiWindow *entity = channel->work;

                        UiWork_Finalize(
                            entity,
                            entity->flags & 2
                        );
                        channel->x = pending;
                        channel->y = pending;
                        channel->entry = pending;
                        channel->countdown = pending;
                        channel->colour = pending;
                        channel->outline = pending;
                        channel->line_spacing = pending;
                        channel->work->state = one;
                        break;
                    }
                    }
                }
            }
        }
        channel_no++;
        channel++;
    } while (channel_no != 3);
}

extern volatile u16 gLagFramesShown;

enum {
    TEXT_END = 0,
    TEXT_PAGE = 1,
    TEXT_WAIT_KEY = 2,
    TEXT_NEWLINE = 3,
    TEXT_PAUSE_LONG = 4,
    TEXT_PAUSE_SHORT = 5,
    TEXT_PAUSE_LONGEST = 6,
    TEXT_COLOURS_RESET = 7,
    TEXT_COLOUR = 8,
    TEXT_OUTLINE = 9,
    TEXT_LINE_SPACING = 10,
    TEXT_WINDOW_STATE = 15,
    TEXT_END_ALT = 30,
    TEXT_COMMANDS = 31,
    TEXT_SPACE = 32,
    TEXT_DAKUTEN = 222,
    TEXT_HANDAKUTEN = 223
};

s32 UiWork_StepChannelScript(struct UiChannelSlot *ch)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 keys = gKeysHeld;
    s32 count = UiText_SpeedSteps[gGameState.message_speed];
    s32 lag;
    u32 code;
    u32 next;
    struct UiWindow *window;
    s32 x;
    s32 y;
    u32 w;
    u32 h;
    s32 old_w;
    s32 old_h;
    s32 entry;
    s32 pc;
    s32 gx;
    s32 gy;
    s32 cue;
    s32 advance;
    s32 width;

    if (work->menu_state != 0) {
        lag = gLagFramesShown;
        if (lag < 0)
            lag = 0;
        if (lag > 2)
            lag = 2;
        count = lag * 5 + 3;
    }

    if (ch->transition != 0) {
        UiWork_ShiftPanelRowsLeft(1);
        ch->transition = (s16)ch->transition - 1;
        return 0;
    }
    if (gKeysHeld == 0 && ch->glyph_delay != 0) {
        ch->glyph_delay--;
        return 0;
    }

    do {
        code = TEXT_END;
        if (ch->ended == 0)
            code = work->entries[ch->entry];

        if (code < TEXT_COMMANDS) {
            switch (code) {
            case TEXT_NEWLINE:
                ch->x = ch->start_x;
                if (ch->work->flags & 8) {
                    if (ch->y > 0xcff) {
                        UiWork_ResetChannelTransition(ch);
                        count = 1;
                    } else {
                        ch->y += 0xd00;
                    }
                } else {
                    ch->y += 0xf00;
                    if (ch->line <= 2)
                        ch->line++;
                }
                break;

            case TEXT_PAGE:
                if (work->mode != 0 && ch->countdown < 900)
                    gKeysPressedLatch = 0;
                ch->countdown = 919;
                if (UiWork_CheckCancelByModeInput(ch) == 0) {
                    struct UiWindow *pane = ch->work;

#if EDITION_INTERNATIONAL
                    if (pane->width == 0 || pane->height == 0
                        || work->page_arrow_shown != 0)
                        break;
#else
                    if (work->page_arrow_shown != 0)
                        break;
#endif
                    UiText_DrawGlyph(pane, 1, pane->width * 4 - 8,
                                     pane->height * 8 - 16, 1);
                    work->page_arrow_shown = 1;
                    break;
                }
                window = ch->work;
                x = window->x;
                y = window->y;
                old_w = window->width;
                old_h = window->height;
                entry = ch->entry;
                work->page_arrow_shown = 0;
                RenderOutput_PrepareForRedraw(window);
                if (ch->flags == 0 && (window->width | window->height) != 0)
                    UiWindow_EraseBorderRect(window->x, window->y,
                                             window->width, window->height);
                entry = (entry + 1) & RENDER_ENTRY_MASK;
                if (work->entries[entry] != 0
                    && (window->width | window->height) != 0) {
                    if (ch->flags != 0) {
                        UiWindow_EraseBorderRect(window->x, window->y,
                                                 window->width, window->height);
                    } else {
                        UiWindow_FitOnScreen(entry, &x, &y, &w, &h,
                                             ch->spacing, 0);
                        if (window->flags & 0x80) {
                            if (old_h != h)
                                y -= h - old_h;
                            if (y < 0)
                                y = 0;
                        }
                        if (!(window->flags & 0x100)) {
                            x += (s32)(old_w - w) / 4;
                            UiWindow_FitOnScreen(entry, &x, &y, &w, &h,
                                                 ch->spacing, 2);
                        }
                        window->x = x;
                        window->y = y;
                        window->width = w;
                        window->height = h;
                    }
                    UiWindow_DrawFrame(window->x, window->y,
                                       window->width, window->height);
                }
                ch->x = ch->start_x;
                ch->y = 0;
                ch->line = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                ch->colour = 15;
                ch->outline = 0;
                ch->line_spacing = 10;
                UiWork_CopyParamsToRenderWork(ch);
#endif
                Resource_ResetEntry(work->glyph_resource);
                work->glyph_resource = 99;
                break;

            case TEXT_WAIT_KEY:
                if (work->mode != 0 && ch->countdown < 900)
                    gKeysPressedLatch = 0;
                if (UiWork_CheckCancelByModeInput(ch) != 0)
                    return 9;
                ch->countdown = 919;
                break;

            case TEXT_PAUSE_SHORT:
                if (ch->countdown == 0)
                    ch->countdown = 20;
                work->result[1] = 0;
                UiWork_CheckCancelByInput(ch);
                break;

            case TEXT_PAUSE_LONGEST:
                if (ch->countdown == 0)
                    ch->countdown = 120;
                work->result[1] = 0;
                UiWork_CheckCancelByInput(ch);
                break;

            case TEXT_PAUSE_LONG:
                if (ch->countdown == 0)
                    ch->countdown = 60;
                work->result[1] = 0;
                break;

            case TEXT_COLOUR:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->colour = work->entries[ch->entry];
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_OUTLINE:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->outline = work->entries[ch->entry];
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_LINE_SPACING:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->line_spacing = work->entries[ch->entry];
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_COLOURS_RESET:
                ch->colour = 15;
                ch->outline = 0;
                ch->line_spacing = 10;
                UiWork_CopyParamsToRenderWork(ch);
                break;

            case TEXT_WINDOW_STATE:
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                ch->work->effect = work->entries[ch->entry];
                ch->countdown = 10;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                break;

            case TEXT_END:
            case TEXT_END_ALT:
                ch->ended = 1;
                return 8;
            }

            if (work->menu_state == 0)
                count = 1;
        } else {
            gx = (ch->x + 128) / 256;
            gy = (ch->y + 128) / 256;
            cue = UiText_SpeedCueGaps[gGameState.message_speed];
            pc = ch->entry;
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
            if (work->mode != 0)
                gx += 8;
#endif
            pc = (pc + 1) & RENDER_ENTRY_MASK;
            next = work->entries[pc];
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
            if (next == TEXT_DAKUTEN) {
                code |= 0x4000;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
            } else if (next == TEXT_HANDAKUTEN) {
                code |= 0x8000;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
            }
#endif
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_ES)
            if (!(ch->work->flags & 8) && code > TEXT_SPACE
                && next > TEXT_SPACE) {
                s16 pair = UiText_Glyphs[code - TEXT_SPACE].width
                    + UiText_Glyphs[next - TEXT_SPACE].width;

                if (work->outline == 1) {
                    if ((u16)pair <= 14) {
                        code |= next << 8;
                        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                    }
                } else if ((u16)pair <= 15) {
                    code |= next << 8;
                    ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
                }
            }
#else
            if (!(ch->work->flags & 8) && code > TEXT_SPACE
                && next > TEXT_SPACE
                && (u16)(UiText_Glyphs[code - TEXT_SPACE].width
                         + UiText_Glyphs[next - TEXT_SPACE].width) <= 15) {
                code |= next << 8;
                ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
            }
#endif
#endif
            advance = UiText_DrawGlyph(ch->work, code, gx, gy, 0);
            ch->glyph_delay = UiText_SpeedDelays[gGameState.message_speed];
            if (advance != 0) {
                if (work->result[0] != 0) {
                    if (work->result[1] == 0) {
                        if (code != TEXT_SPACE) {
                            AudioCommand_PlayFar(work->result[0] + (code & 3));
                            work->result[1] = cue;
                        }
                    } else {
                        work->result[1]--;
                    }
                }
                width = advance << 8;
#if EDITION_INTERNATIONAL
                if (code == TEXT_SPACE)
                    width += ch->spacing[ch->line];
#else
                width += ch->spacing[ch->line];
#endif
                ch->x += width;
            }
#if !EDITION_INTERNATIONAL
            else
                return 9;
#endif
            if (code == TEXT_SPACE && work->menu_state == 0)
                count = 1;
        }

        if (ch->countdown != 0) {
            ch->countdown--;
            if (ch->countdown != 0)
                continue;
        }
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
    } while (--count != 0);

    return 0;
}

#define WINDOW_HALF(window, type, field) \
    (*(type *)((u8 *)(window) + (u32)&(((struct UiWindow *)0)->field)))

struct UiWindowInterpolationScratch {
    s32 scaled_part;
    s32 scaled_duration;
    s32 result;
};

void UiWork_ProcessDirectWork(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    struct UiWindow *work = state->windows;
    s32 index = 0;
    u8 dirty;

loop:
    if (work->flags != 0) {
        if ((s16)work->frame != 0) {
            UiWindow_UpdateInterpolatedGeometry(work, 0);
            work->frame = (s16)work->frame - 1;
        } else if (work->duration != 0) {
            UiWork_DrawByAttributes(work);
        }
    } else if (work->duration != 0) {
        if ((s16)work->frame != work->duration) {
            UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
                                     (s16)work->previous_width,
                                     (s16)work->previous_height);
            UiWindow_UpdateInterpolatedGeometry(work, 1);
            work->frame++;
            dirty = 1;
            state->dirty = dirty;
        } else {
            UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
                                     (s16)work->previous_width,
                                     (s16)work->previous_height);
            work->output.head = NULL;
            work->output.tail_link = NULL;
            work->width = 0;
            work->height = 0;
            work->x = 0;
            work->y = 0;
            work->unknown_10 = 0;
            work->effect = 0;
            work->state = 0;
            work->flags = 0;
            work->frame = 0;
            work->duration = 0;
            work->previous_x = 0;
            work->previous_y = 0;
            work->previous_width = 0;
            work->previous_height = 0;
            state->dirty = 1;
        }
    }
    index++;
    work++;
    if (index != WINDOW_COUNT)
        goto loop;
}

void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position)
{
    /* FAKEMATCH: needed to match: the halfword window view keeps geometry reads before scratch stores; direct fields reorder them. */
    struct UiWindowInterpolationScratch scratch;
    s32 frame;
    s32 duration;
    s32 remaining;
    s32 x;
    s32 y;
    s32 width;
    s32 height;

    frame = WINDOW_HALF(window, s16, frame);
    duration = WINDOW_HALF(window, s16, duration);
    remaining = duration - frame;
    scratch.scaled_part =
        (frame *WINDOW_HALF(window, u16, width)) << 16;
    scratch.scaled_duration = duration << 17;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    x = (scratch.result >> 16) + WINDOW_HALF(window, u16, x);

    scratch.scaled_part =
        (remaining *WINDOW_HALF(window, u16, width)) << 16;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    width = scratch.result >> 15;

    scratch.scaled_part =
        (frame *WINDOW_HALF(window, u16, height)) << 16;
    scratch.scaled_duration = WINDOW_HALF(window, s16, duration) << 17;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    y = (scratch.result >> 16) + WINDOW_HALF(window, u16, y);

    scratch.scaled_part =
        (remaining *WINDOW_HALF(window, u16, height)) << 16;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    height = scratch.result >> 15;

    UiWindow_DrawFrame(x, y, width, height);
    if (save_position != 0) {
        WINDOW_HALF(window, u16, previous_x) = x;
        WINDOW_HALF(window, u16, previous_y) = y;
        WINDOW_HALF(window, u16, previous_width) = width;
        WINDOW_HALF(window, u16, previous_height) = height;
    }
}
