/* Draft, differ 4 of 62: after the terminator store the native shifts the row
 * offset (lsrs r2, r4, #3) before it loads the window row, and adds them
 * after copying the column offset; this loads the row first. Measured: the
 * shift written first in the sum is differ 10; the shift as a temporary
 * behind an empty asm barrier changes the high-register copies (differ 29);
 * four minutes of permuter found nothing better. Twin of The Broken Seal's
 * UiText_DrawStringAtOffset with this game's loop barrier. */
#include "mem.h"
#include "text_render_runtime.h"
#include "window.h"
#include "ram_buffer.h"

void UiText_DrawStringAtOffset(
    u8 *text,
    struct RenderInput *work,
    s32 offsetX,
    s32 offsetY)
{
    u16 *buffer;
    u16 *output;
    struct UiRenderWork *canvas;
    u32 cell;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    canvas = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    output = buffer;
    while (*text != 0) {
        *output = *text;
        text++;
        output++;
        /* FAKEMATCH: ⚓️ steps the output before it reads the next byte. */
        __asm__("");
    }
    *output = 0;

    cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5)
        + ((s16)work->x + ((u32)offsetX >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)canvas->tilemap + cell;
        phase = offsetX & 7;
        UiText_RenderStringTiles(buffer, (u16 *)src, (u16 *)dst, phase);
        MemFree(buffer);
    }
}
