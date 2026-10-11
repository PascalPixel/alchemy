#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "text_reader.h"
#include "types.h"
#include "tbs_edition.h"
#include "dma.h"
#include "ramcode.h"
#include "mem.h"
#include "window.h"
#include "io_reg.h"
#include "edition.h"
#include "global_cells.h"
#include "render_input.h"

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *colours, s32 flags);
s32 UiWork_IsComplete(void);

void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset)
{
    struct UiRenderWork *work;
    struct UiWindow *window;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 entry;

    work = (struct UiRenderWork *)gWindowWork[0];
    x = 8;
    y = 8;
    // the window starts as NULL, though every path that reads it assigns it first; the same zero is the style argument below
    window = NULL;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] != 0) {
        UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width, (u32 *)&height);
        x = (30 - width) >> 1;
        y = ((15 - height) >> 1) + y_offset;
        if (mode != 0)
            window = UiWindow_Create(x, y, width, height, 0);
        else {
            window = UiWindow_Create(x, y, 0, 0, 2);
            window->width = mode;
            window->height = mode;
        }
        if (UiText_QueueRenderEntries(window, entry, 0, 0, 0, 0) == 0)
            UiWork_Finalize(window, 1);
        else {
            while (UiWork_IsComplete() == 0)
                WaitFrames(1);
            if (mode != 0) {
                UiWork_Finalize(window, 0);
                while (UiWork_IsIdle(window) == 0)
                    WaitFrames(1);
            } else
                UiWork_Finalize(window, 1);
            work->result[0] = 0;
            work->result[1] = 0;
        }
    }
}

s32 UiText_BuildRenderEntriesMode1(s32 arg0)
{
    return UiText_BuildRenderEntries(arg0, 1);
}

void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);

    RamCode_Load(routine, UiText_LookupMessageArm, UiText_LookupMessageCodeSize);
    routine(reader, message);
    MemFree(routine);
}

extern const void *const Text_MessageContexts[][2];

s32 UiText_DecodeSymbol(struct TextReader *reader)
{
    u32 code;
    u32 hi;
    const u8 *base;
    const u8 *tree;
    const u8 *leaves;
    const u8 *text;
    s32 text_bits;
    s32 tree_bits;
    s32 rank;
    s32 depth;
    s32 bit;
    u32 index;
    u32 half;

    code = reader->previous_code;
    text = (const u8 *)reader->words;
    text_bits = reader->bits;
    tree_bits = 1;
    hi = code >> 8;
    code &= 0xff;
    base = Text_MessageContexts[hi][0];
    base = base + ((const u16 *)Text_MessageContexts[hi][1])[code];
    tree = base;
    leaves = base - 1;
    rank = 0;
    for (;;) {
        bit = tree_bits & 1;
        tree_bits >>= 1;
        if (bit != 0 && tree_bits == 0) {
            tree_bits = *tree++;
            bit = tree_bits & 1;
            tree_bits = (tree_bits >> 1) | 0x80;
        }
        if (bit != 0)
            break;
        bit = text_bits & 1;
        text_bits >>= 1;
        if (bit != 0 && text_bits == 0) {
            text_bits = *text++;
            bit = text_bits & 1;
            text_bits = (text_bits >> 1) | 0x80;
        }
        if (bit == 0)
            continue;
        depth = 0;
        do {
            bit = tree_bits & 1;
            tree_bits >>= 1;
            if (bit != 0 && tree_bits == 0) {
                tree_bits = *tree++;
                bit = tree_bits & 1;
                tree_bits = (tree_bits >> 1) | 0x80;
            }
            if (bit == 0)
                depth++;
            else {
                rank++;
                depth--;
            }
        } while (depth >= 0);
    }
    index = rank * 3;
    if (((index * 4) & 7) == 0) {
        bit = leaves[-(index >> 1)];
        code = bit << 4;
        bit = leaves[-(index >> 1) - 1];
        code |= (u32)bit >> 4;
    } else {
        half = leaves[-(index >> 1)];
        half &= 0xf;
        code = leaves[-(index >> 1) - 1] | (half << 8);
    }
    reader->bits = text_bits;
    reader->previous_code = code;
    reader->words = (const u32 *)text;
    return code;
}

extern s32 CreateSideObject();

void UiWork_SetTwoEntriesTo999(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 index = 0;

    do {
        work->resource_ids[index] = 999;
        index++;
    } while (index != 2);
}

s32 Localization_LookupEntryId(u32 value)
{
    s32 result = -1;
    s32 i = 0;

    if (value < 20) {
        for (;;) {
            s32 key = Portrait_CharacterMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = Portrait_CharacterMap[i];
                break;
            }
            i += 2;
        }
    } else {
        for (;;) {
            s32 key = Portrait_SpriteMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = Portrait_SpriteMap[i];
                result += 128;
                break;
            }
            i += 2;
        }
    }
    return result;
}

s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 win;
    s32 minus_four;
    s32 ofs;
    void *work;

    work = gWindowWork[0];
    if (Localization_LookupEntryId(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    ofs = minus_four;
    if (((struct UiRenderWork *)work)->mode != 0) {
        win = (s32)UiWindow_Create(x, y, 6, 5, 2);
        ofs = 0;
    } else {
        win = (s32)UiWindow_Create(x, y, 5, 5, 2);
    }
    if (win != 0) {
        CreateSideObject(arg0, arg1, -1, win, ofs, minus_four);
    }
    return win;
}

void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry)
{
    struct UiRenderWork *state;
    struct UiWindow *work;
    s32 id;
    s32 index;
    s32 i;
    struct RenderOutput *entity;

    state = (struct UiRenderWork *)gWindowWork[0];
    work = state->windows;
    id = Localization_LookupEntryId(entry);
    if (id == -1)
        return;

    if (state->resource_ids[1] == id) {
        index = 1;
    } else if (state->resource_ids[0] == id) {
        index = 0;
    } else {
        return;
    }

    id = state->output_ids[index];

    for (i = 0; i != WINDOW_COUNT; i++, work++) {
        entity = work->output.head;
        if ((u8)entity->kind == 2 && (u8)entity->index == id) {
            UiWork_Finalize(work, 2);
            return;
        }
    }
}
