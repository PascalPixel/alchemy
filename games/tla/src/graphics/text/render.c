#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "text_render_runtime.h"

u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);


void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
{
    u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
        /* FAKEMATCH: ⚓️ steps the output before it reads the next byte. */
        __asm__("");
    }
    x >>= 3;
    *output = 0;
    y >>= 3;
    UiText_RenderWideStringInWindow(buffer, arg1, x, y);
    MemFree(buffer);
}

#if !defined(TLA_EDITION_JA) // The Japanese scaffold has no label for UiText_FormatNumber yet.

// The number writers format the value in a 16-byte buffer and draw it with
// the matching string writer.
void UiText_DrawNumber(s32 value, s32 width, s32 output, s32 x, s32 y)
{
    u8 data[16];
    /* FAKEMATCH: the value is pinned to the r4 it takes: the native loads the
       fifth argument from the stack before it copies the first, and the plain
       parameter copies the first ahead of the load (one reordered pair,
       differ 2). */
    register s32 pinned asm("r4") = value;

    UiText_DrawString(UiText_FormatNumber(data, pinned, width), output, x, y);
}

void UiText_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *work, s32 x, s32 y)
{
    u8 data[16];
    /* FAKEMATCH: pinned to r4 as in UiText_DrawNumber (differ 2 without). */
    register s32 pinned asm("r4") = value;

    UiText_DrawStringAtOffset(UiText_FormatNumber(data, pinned, width), work, x, y);
}

void UiText_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y)
{
    u8 data[16];
    /* FAKEMATCH: pinned to r4 as in UiText_DrawNumber (differ 2 without). */
    register s32 pinned asm("r4") = value;

    UiText_DrawStringInWindow(UiText_FormatNumber(data, pinned, width), output, x, y);
}

#endif
