/*
 * Draft: UiText_DrawCharacterAtOffset, native extent 152 bytes in all six
 * editions, including its literal pool. Uses the maintained TBS rendering
 * body and TLA work layout; coordinates are signed halfword views and
 * pixel offsets use unsigned shifts.
 *
 * Finite ordinary trials (2026-10-03, approved TLA flags):
 * - Canonical work fields, actual heap root, signed coordinate views:
 *   EN score 360, 6 reordered rows; retained.
 * - Normalize offset_y after the entry terminator and before incrementing
 *   count: score 360, the same 6 reordered rows; simpler first form retained.
 * All six edition forms compile to 152 bytes, with layout guards passing.
 * Remaining scheduling differs around the heap load, ring-mask load,
 * y-offset shift/add and the literal-pool branch.
 *
 * The old combined text_render.c copy had no recorded score and used the
 * unsupported TEXT_IWRAM/offset macros and UiText_RenderTileRow contract,
 * with unsigned coordinate views. Its body is superseded here; the other
 * combined-draft functions are retained.
 *
 * The current scorer cannot resolve the newly named tile renderer from
 * its linked map, so that call uses symbol-name comparison only. This is
 * not an exact result or an adoption. No FAKEMATCH device is used.
 */
#include "TEXT_RENDER_RUNTIME.H"
#include "WINDOW.H"
#include "RAM_BUFFER.H"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);

void UiText_DrawCharacterAtOffset(
    s32 character,
    struct RenderInput *position,
    u32 offset_x,
    u32 offset_y)
{
    s32 byte_offset;
    s32 vram_address;
    u16 *text;
    s32 zero;
    u32 cell;
    struct UiRenderWork *canvas;
    u16 *counter;

    canvas = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    counter = &canvas->count;
    zero = 0;
    *counter = zero;
    UiText_BuildRenderEntries(character, 1);

    canvas->entries[*counter] = zero;
    *counter = (u16)((*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1));

    cell = (((s16)position->y + (offset_y >> 3) + 1) << 5)
        + ((s16)position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = canvas->entries;
        UiText_RenderStringTiles(
            text,
            (s32)((u8 *)canvas->tilemap + byte_offset),
            vram_address,
            7 & offset_x);
    }
}
