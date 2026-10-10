#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "edition.h"
#include "types.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "btlwork.h"
#include "heap_state.h"
#include "global_cells.h"
#include "render_input.h"
#include "game_state.h"
#include "iwram_call.h"
#include "text_font.h"
#include "glyph.h"

s32 UiWork_IsIdle(struct UiWindow *window)
{
#if !EDITION_INTERNATIONAL
    if (window == NULL)
        return 1;
#endif
    if (window->flags == 0 && window->duration == 0)
        return 1;
    return 0;
}

void UiWork_ResetCounters(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];

    state->colour = 15;
    state->line_spacing = 10;
    state->unknown_before_count = 9;
    state->outline = 0;
    state->unknown_after_line_spacing = 1;
}

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    s32 size;

    state->glyph_tiles = VramBlock_LoadCached(95, 128 << 6, 0);
    state->unknown_before_count = 9;
    state->line_spacing = 10;
    state->outline = 0;
    state->colour = 15;
    state->count = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback((void *)UiWork_ProcessAll, size);
}

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    s32 size;

    if (initialize != 0)
        state->glyph_tiles = VramBlock_LoadCached(95, 128 << 6, 0);
    state->unknown_before_count = 9;
    state->line_spacing = 10;
    state->outline = 0;
    state->colour = 15;
    state->count = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, size);
}

void UiWork_FinalizeSharedSlot(void)
{
    struct BattleDisplayWork *display = gBattleDisplayWork;
    struct UiWindow *window = display->window;

    if (window != NULL) {
        UiWork_Finalize(window, 1);
        display->window = NULL;
    }
}

enum { HEAP_SLOT_BATTLE_DISPLAY = 37 };

struct BattleMessageSlotRange {
    struct UiRenderWork *render;
    void *blocks[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW - 1];
    struct BattleDisplayWork *display;
};

LAYOUT_OFFSET_GUARD(BattleMessageSlotRange_Display, struct BattleMessageSlotRange,
    display, (HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW) * sizeof(void *));
LAYOUT_SIZE_GUARD(BattleMessageSlotRange_Size, struct BattleMessageSlotRange,
    (HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW + 1) * sizeof(void *));
LAYOUT_OFFSET_GUARD(BattleMessageSlotRange_HeapEndpoint, union HeapState,
    slots[HEAP_SLOT_BATTLE_DISPLAY], HEAP_SLOT_BATTLE_DISPLAY * sizeof(void *));
typedef char BattleMessageSlotRange_InBank[
    HEAP_SLOT_BATTLE_DISPLAY < sizeof(((union HeapState *)0)->slots) / sizeof(void *) ? 1 : -1];

void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    struct UiWindow *existing;
    struct UiWindow *work;
    struct UiRenderWork *state;
    struct BattleDisplayWork *control;

    state = ((struct BattleMessageSlotRange *)gWindowWork)->render;
    control = ((struct BattleMessageSlotRange *)gWindowWork)->display;
    /* FAKEMATCH: needed to match: result is cleared before the entry lookup and later carries the channel pointer as a scalar; without the early 0 the function differs 47. */
    result = 0;
    state->menu_state = 2;
    index = UiText_BuildRenderEntries(argument, 1);
    state->menu_state = 1;

    if (state->entries[index] != 0) {
        existing = control->window;
        if (existing == NULL) {
            work = UiWindow_Create(0, 15, 30, 6, 10);
            control->window = work;
            UiWindow_MapTextCanvasTiles(0, 15, 30, 6, 1);
            control->marked = 0;
        } else {
            work = existing;
        }
        if (work != NULL) {
            result = (s32)UiWork_ActivateChannel(work, index, control->marked);
            control->offset = (struct BattleDisplayOffset *)result;
            control->marked = 0;
            if (result == 0) {
                UiWork_Finalize(work, 1);
            }
        }
    }
}

#define UiWindow_MapTextCanvasTiles ((void (*)(s32,s32,s32,s32,s32))UiWindow_MapTextCanvasTiles)

s32 UiWork_IsComplete(void);
void UiText_ShowMessageAndWaitCore(s32 argument)
{
    UiText_PrepareMessageWork(argument);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *spacing, s32 flags);

s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 entry;
    s32 result = 0;
    /* FAKEMATCH: needed to match: four unread render entry slots, the native frame's eight extra bytes. */
    u16 entries[4];

    work->result[0] = 0;
    work->result[1] = 0;
    entry = UiText_BuildRenderEntries(argument, 1);
    if (work->entries[entry] == 0)
        return 0;
    if (window == 0)
        return 0;
    result = (s32)UiText_QueueRenderEntries((struct UiWindow *)window, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed);

void UiWork_SetBusyFlags(s32 flags)
{
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)gWindowWork[0];
    if (work != NULL) {
        if (flags & 1) {
            work->spacing_before = 1;
        }
        if (2 & flags) {
            work->spacing_after = 1;
        }
    }
}

struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed)
{
    s32 entry;
    u32 height;
    u32 width;
    u16 bounds[4];
    s32 flags;
    u16 *out;
    struct UiWindow *window;
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)gWindowWork[0];
    work->result[0] = (packed << 4) >> 20;
    /* FAKEMATCH: needed to match: the window starts as NULL, though it is assigned before it is read; that zero's register serves the plain zeros below. */
    window = NULL;
    work->result[1] = 0;
    packed &= 0xffff;
    flags = 0;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] == 0)
        return NULL;
    UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
    if (width == 0 && height == 0)
        return NULL;
    if (!(packed & 1))
        flags |= 2;
    if (packed & 8)
        flags |= 8;
    if (packed & 16)
        flags |= 128;
    if (packed & 32)
        flags |= 256;
    window = UiWindow_Create(x, y, width, height, flags);
    if (window == NULL)
        return NULL;
    if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
        UiWork_Finalize(window, 1);
        return NULL;
    }
    work->spacing_before = 0;
    work->spacing_after = 0;
    return window;
}

void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    s32 release;
    s32 pos[2] = {0, 0};
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    struct UiWindow *work = NULL;
    s32 zero;

    y = x = 0;
    release = flags & 1;

    if (flags & 2)
    {
        state->busy = 1;
    }

    UiText_GetResourceDimensions(no, &x, &y, (u32 *)&width, (u32 *)&height);

    x = (30 - width) >> 1;
    y = (12 - height) >> 1;

    if (flags & 8)
    {
        y = y + 4;
    }
    else if (flags & 0x40)
    {
        y = y + 12;
    }
    else
    {
        s32 dy;

        Object_GetScreenPositionFar(gGameState.selected_actor, pos);
        dy = pos[1] >> 3;
        if (dy > 9)
        {
            y = dy - 5;
        }
        else
        {
            y = dy + 4;
        }
    }

    work = UiText_OpenMessageWindow(no, x, y, release);

    if (work != NULL)
    {
        while (UiWork_IsComplete() == 0)
        {
            WaitFrames(1);
        }

        if (flags & 0x20)
        {
            ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
        }

        if (!(flags & 4))
        {
            UiWork_Finalize(work, release);
            while (UiWork_IsIdle(work) == 0)
            {
                WaitFrames(1);
            }
        }
    }

    zero = 0;
    state->busy = zero;
    state->result[0] = zero;
    state->result[1] = zero;
    WaitFrames(3);
}

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}

#if EDITION_INTERNATIONAL
s32 UiText_RenderGlyphPair(s32 code, u32 *out)
{
    u8 buf[384];
    struct UiRenderWork *work;
    const u8 *input;
    s32 length;
    s32 colour;
    s32 transparent;
    s32 first_index;
    s32 second_index;
    u16 second_width;
    u8 *digits;
    s32 group;
    s32 row;
    s32 column;
    s32 nibble;
    u32 value;

    work = (struct UiRenderWork *)gWindowWork[0];
    second_index = (code << 8) >> 16;
    code &= 255;
    Iwram_ClearWords(buf, 384);
    if (work->mode != 0) {
        transparent = 0;
        colour = 8;
    } else {
        transparent = 1;
        colour = work->colour;
    }
    first_index = code - 32;
    input = first_index * sizeof(struct FontGlyph) + (const u8 *)UiText_Glyphs;
    length = ((const struct FontGlyph *)input)->width;
    input = (const u8 *)((const struct FontGlyph *)input)->rows;
    if (work->outline == 1) {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        Iwram_ExpandBitRuns(input, buf + 50, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        Iwram_ExpandBitRuns(input, buf + 33, colour);
        length++;
    } else {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
    }
    if ((u16)second_index != 0) {
        input = (const u8 *)&UiText_Glyphs[-32] + ((u16)second_index << 5);
        second_width = *(const u16 *)input;
        input += 2;
        if (work->outline == 1) {
            u8 *dst = buf + length;
            Iwram_ExpandBitRuns(input, dst + 49, transparent);
            Iwram_ExpandBitRuns(input, dst + 50, transparent);
            Iwram_ExpandBitRuns(input, dst + 32, colour);
            Iwram_ExpandBitRuns(input, dst + 33, colour);
            second_width += 1;
        } else {
            u8 *dst = buf + length;
            u8 *p;
            p = dst + 49;
            Iwram_ExpandBitRuns(input, p, transparent);
            p = dst + 32;
            Iwram_ExpandBitRuns(input, p, colour);
        }
        length += second_width;
    }

    digits = buf + 7;
    group = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                value = 0;
                nibble = 7;
                do {
                    value = (value << 4) + *digits--;
                    nibble--;
                } while (nibble >= 0);
                *out++ = value;
                column++;
                digits += 24;
            } while (column <= 7);
            row++;
            digits -= 120;
        } while (row <= 1);
        group++;
        digits += 112;
    } while (group <= 1);
    return length;
}
#else
s32 UiText_RenderGlyphPair(u32 code, u32 *out)
{
    u8 buf[256];
    struct UiRenderWork *work;
    const u8 *input;
    const u8 *mark;
    s32 length;
    s32 colour;
    s32 transparent;
    u32 mark_code;
    u8 *digits;
    s32 group;
    s32 row;
    s32 column;
    s32 nibble;
    u32 value;

    /* FAKEMATCH: needed to match: the empty loop ends a scheduling region, keeping the spill of out ahead of the work-address load. */
    do { } while (0);
    work = (struct UiRenderWork *)gWindowWork[0];
    mark = 0;
    mark_code = UiText_MarkGlyphCodes[code >> 14];
    if (mark_code != 0)
        mark = (const u8 *)((const struct FontGlyph *)
            ((const u8 *)UiText_Glyphs +
            (mark_code - 32) * sizeof(struct FontGlyph)))->rows;
    code &= 0x3fff;
    if (code <= 0xff) {
        input = (const u8 *)UiText_Glyphs +
            (code - 32) * sizeof(struct FontGlyph);
        length = ((const struct FontGlyph *)input)->width;
        input = (const u8 *)((const struct FontGlyph *)input)->rows;
    } else {
        input = UiText_KanjiGlyphs + (code - 256) * 24;
        length = 10;
    }
    Iwram_ClearWords(buf, 256);
    if (work->mode != 0) {
        transparent = 0;
        colour = 8;
    } else {
        transparent = 1;
        colour = work->colour;
    }
    if (work->outline == 1) {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        Iwram_ExpandBitRuns(input, buf + 50, transparent);
        if (mark != 0) {
            Iwram_ExpandBitRuns(mark, buf + 24, transparent);
            Iwram_ExpandBitRuns(mark, buf + 25, transparent);
        }
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        Iwram_ExpandBitRuns(input, buf + 33, colour);
        if (mark != 0) {
            /* FAKEMATCH: needed to match: assigning digits in the mark argument makes the argument register compute buf + 7 before its loop copy. */
            Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
            Iwram_ExpandBitRuns(mark, buf + 8, colour);
        }
        length++;
    } else {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        if (mark != 0)
            Iwram_ExpandBitRuns(mark, buf + 24, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        if (mark != 0) {
            /* FAKEMATCH: needed to match: this mark path needs the same argument-before-loop order as the outlined path. */
            Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
        }
    }

    digits = buf + 7;
    group = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                value = 0;
                nibble = 7;
                do {
                    value = (value << 4) + *digits--;
                    nibble--;
                } while (nibble >= 0);
                *out++ = value;
                column++;
                digits += 24;
            } while (column <= 7);
            row++;
            digits -= 120;
        } while (row <= 1);
        group++;
        digits += 112;
    } while (group <= 1);
    return length;
}
#endif
