#include "types.h"
#include "window.h"
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
#include "text_reader.h"
#include "heap_state.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"

s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity)
{
    struct UiRenderWork *work;
    u32 count;

    work = (struct UiRenderWork *)gWindowWork[0];
    work->count = 0;
    UiText_BuildRenderEntries(key, 1);
    for (count = 0; count < capacity - 1 &&
        (destination[count] = work->entries[count]) != 0; count++) {
    }
    destination[count] = 0;
    return count;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define DECODE_EUROPEAN
#endif

void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity)
{
    struct TextReader reader;
    void *original;
    s32 (*decode)(struct TextReader *);
    u32 value;
#if !defined(DECODE_EUROPEAN)
    u16 delta;
#endif

    original = ((union HeapState *)&gWorkSlot)->slots[50];
    if (original == NULL)
        RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
    decode = (s32 (*)(struct TextReader *))((union HeapState *)&gWorkSlot)->slots[50];
    UiText_LookupMessage(&reader, message);
#if !defined(DECODE_EUROPEAN)
    delta = 0xffff;
#endif
    while ((value = decode(&reader)) != 0) {
        switch (value) {
        case 14:
            capacity -= 3;
            if (capacity <= 0)
                goto done;
            *text++ = value;
#if defined(DECODE_EUROPEAN)
            value = decode(&reader) + 0xffff;
            *text++ = value;
            value = decode(&reader) + 0xffff;
#else
            *text++ = decode(&reader) + delta;
            value = decode(&reader) + delta;
#endif
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
#if defined(DECODE_EUROPEAN)
        case 29:
#endif
            capacity--;
            if (capacity <= 0)
                goto done;
            *text++ = value;
            value = decode(&reader) + 0xffff;
            break;
        default:
            capacity--;
            if (capacity <= 0)
                goto done;
            break;
        }
        *text++ = value;
    }
done:
    if (original == NULL)
        Runtime_ReleaseHeapBlock(50);
    *text = 0;
}
