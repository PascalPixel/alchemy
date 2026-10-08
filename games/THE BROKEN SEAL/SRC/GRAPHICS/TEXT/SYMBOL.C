#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TEXT_READER.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "DMA.H"
#include "MEM.H"
#include "WINDOW.H"
#include "IO_REG.H"

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
    /* FAKEMATCH: needed to match: the null window also supplies the zero style argument, retaining one register across the message and dimension lookups. */
    window = NULL;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] != 0) {
        UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width, (u32 *)&height);
        x = (30 - width) >> 1;
        y = ((15 - height) >> 1) + y_offset;
        if (mode != 0)
            window = UiWindow_Create(x, y, width, height, (s32)window);
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
    u32 size;

    /* FAKEMATCH: needed to match: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)UiText_LookupMessageCodeSize;
    } while (0);
    routine = (void (*)(struct TextReader *, s32))MemAlloc(size);
    Dma_Set((const void *)UiText_LookupMessageArm, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(reader, message);
    MemFree(routine);
}

extern const void *const Text_MessageContexts[][2];

s32 UiText_DecodeSymbol(struct TextReader *reader)
{
    u32 code;
    u32 hi;
    /* FAKEMATCH: tree pinned to r1, as the native allocates it (62 instructions differ without it). */
    register const u8 *tree asm("r1");
    const u8 *leaves;
    const u8 *text;
    s32 text_bits;
    s32 tree_bits;
    s32 rank;
    /* FAKEMATCH: depth pinned to r3, as the native allocates it (92 differ without it). */
    register s32 depth asm("r3");
    s32 bit;
    u32 index;
    u32 half;

    code = reader->previous_code;
    text = (const u8 *)reader->words;
    text_bits = reader->bits;
    tree_bits = 1;
    hi = code >> 8;
    code &= 0xff;
    tree = Text_MessageContexts[hi][0];
    tree = tree + ((const u16 *)Text_MessageContexts[hi][1])[code];
    leaves = tree - 1;
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
        code = leaves[-(index >> 1)] << 4;
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
