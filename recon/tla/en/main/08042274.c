/* Near miss, differ 2 of 74 (one reordered pair), 2026-10-15: The Broken
 * Seal's UiText_DrawPrefixedNumberAtOffset in the form of its English
 * edition (its Spanish, French and Italian ones set both halfwords in the
 * branches; this game's other editions are not yet measured), reading the
 * window's x and y as signed halfwords. The second halfword goes through a
 * u16 temporary, which loads 0xf01e from the pool as the native does, with a
 * barrier after its store. Remaining: the native loads the fifth argument
 * from the stack (ldr r5, [sp, #56]) and the window tiles before it copies
 * the value (adds r4, r0, #0); this copies first. Pinning the value to r4
 * (which fixes UiText_DrawNumber) or any other parameter to its register,
 * and an asm use of the variant or the work, did not move the copy. The
 * other editions' scaffold holds this function in one piece with
 * UiText_DrawNumberInWindow. */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "render_input.h"
#include "text_render_runtime.h"

u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);

void UiText_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work, s32 offset_x, s32 offset_y, s32 variant)
{
    u8 formatted[16];
    u16 output[8];
    u8 *text;
    s32 index;
    u32 cell;
    struct UiRenderWork *canvas;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;
    u16 second;

    canvas = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    text = UiText_FormatNumber(formatted, value, 4);
    if (variant == 0) {
        output[0] = 0xf01d;
    } else {
        output[0] = 0xf01f;
    }
    second = 0xf01e;
    output[1] = second;
    asm("");
    for (index = 0; index <= 4; index++) {
        output[index + 2] = text[index];
    }
    output[6] = 0;
    cell = (((s16)work->y + ((u32)offset_y >> 3) + 1) << 5) + ((s16)work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)canvas->tilemap + cell;
        phase = offset_x & 7;
        UiText_RenderStringTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
    }
}
