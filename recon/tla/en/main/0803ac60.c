/* Draft, differ 2 of 50: The Broken Seal's UiText_RenderGlyphTileAtWorkOffset
 * with ⚓️'s changes: the window position is read signed and the string's
 * tiles take offsetX & 7 where ☀️ passes the VRAM base. The native loads the
 * entry mask 0x1ff (ldr r2) before it rereads the count after the entry
 * store (ldrh r3, [r1]); sched2 ranks the reread first (priority 5 to 4, the
 * store to load latency is 1). Measured: a counter pointer as in ☀️ (differ
 * 10), the mask in a variable behind an empty asm, an operand asm or a
 * one-pass block (12 to 26: the mask is then built from a split constant). */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "render_input.h"
#include "text_render_runtime.h"

void UiText_RenderGlyphTileAtWorkOffset(
	u16 *buffer, const struct RenderInput *work, s32 offsetX, s32 offsetY)
{
	struct UiRenderWork *canvas = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
	u32 cell;

	if (buffer == NULL)
	{
		buffer = canvas->entries;
		canvas->entries[canvas->count] = 0;
		canvas->count = (canvas->count + 1) & (UI_TEXT_ENTRY_COUNT - 1);
	}

	cell = (((s16)work->y + offsetY + 1) << 5) + ((s16)work->x + offsetX) + 1;
	if (cell < 0x280)
	{
		s32 src;
		s32 dst;

		cell *= 2;
		dst = 0x06002000 + cell;
		src = (s32)canvas->tilemap + cell;

		UiText_RenderStringTiles(buffer, (u16 *)src, (u16 *)dst, offsetX & 7);
	}
}
