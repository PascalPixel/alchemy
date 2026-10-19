#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "edition.h"
#include "text_render_runtime.h"

s32 Func_0803c274(struct UiWindow *window, u32 c, s32 x, s32 y, s32 flags);

/// Draws a string of 16-bit character codes straight into the UI tilemap, as
/// tiles, inside a window at tile (offsetX, offsetY) of its interior, writing
/// both the work block's copy and VRAM. A NULL buffer draws the decoded text
/// in UiRenderWork.entries. The window's position is read signed. Not in
/// tla-ja, which keeps an older layout.
// As ☀️'s function of the same name, which passes the VRAM base where this
// one passes offsetX & 7.
void UiText_RenderGlyphTileAtWorkOffset(u16 *buffer,
                                        const struct UiWindow *work,
                                        s32 offsetX, s32 offsetY) {
  struct UiRenderWork *canvas =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u32 cell;

  if (buffer == NULL) {
    buffer = canvas->entries;
    canvas->entries[canvas->count] = 0;
    {
      // ⚓️ loads the mask 0x1ff from the pool before it rereads the count.
      /* FAKEMATCH(pin): the mask pinned to the r2 it takes, as in
       * UiText_RenderWideStringInWindow, so it loads from the pool; as a
       * plain u16 local, 14 of 50 instructions differ in tla-en (a split
       * constant). */
      register u16 mask asm("r2") = UI_TEXT_ENTRY_COUNT - 1;

      /* FAKEMATCH(one-pass): the count update in a one-pass block, which
       * keeps the reread after the mask's load; as a plain statement, 2 of 50
       * instructions differ in tla-en (an empty asm barrier instead moves the
       * literal pool). */
      do {
        canvas->count = (canvas->count + 1) & mask;
      } while (0);
    }
  }

  cell = (((s16)work->y + offsetY + 1) << 5) + ((s16)work->x + offsetX) + 1;
  if (cell < 32 * 20) { // inside the tilemap
    s32 src;
    s32 dst;

    cell *= 2;
    dst = 0x06002000 + cell;
    src = (s32)canvas->tilemap + cell;

    UiText_RenderStringTiles(buffer, (u16 *)src, (u16 *)dst, offsetX & 7);
  }
}

/// Draws a string of wide (16-bit) codes into the window, one glyph per cell,
/// skipping the control codes and their arguments; code 3 starts a new line.
/// With no text, the render work's own entries are drawn, closed with a 0.
void UiText_RenderWideStringInWindow(s16 *text, struct UiWindow *window, s32 x,
                                     s32 y) {
  struct UiRenderWork *work;
  u16 c;
  s16 start;

  work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  start = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = 0;
    {
      /* FAKEMATCH: ⚓️ loads the mask 0x1ff from the pool before it rereads the
       * count; plainly, as a u32 or u16 temporary, or with a barrier between
       * the two statements, the count is reread first (differ 2) or the mask
       * becomes a split constant (differ 18). Only a halfword register loads
       * from the pool and a u16 local is widened, so the mask is pinned to the
       * r2 it takes, as in mode6.c. */
      register u16 mask asm("r2") = UI_TEXT_ENTRY_COUNT - 1;

      /* FAKEMATCH: the barrier of the pin above: it holds r3 until the mask is
       * loaded. */
      asm("" : "+l"(mask) : : "r3");
      work->count = (work->count + 1) & mask;
    }
  }
  c = *text++;
  if (c != 0)
    do {
      if (c <= 30 || c == 176) {
        switch (c) {
        case 3:
          x = start;
          y++;
          break;
        case 14:
        case 15:
        case 28:
          text++;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 17:
        case 29:
          text++;
          break;
        }
      } else {
        Func_0803c274(window, c, x, y, 0);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
        x++;
#else
        if ((u16)(c - 0xde) > 1)
          x++;
#endif
      }
      c = *text++;
    } while (c != 0);
  work->dirty = 1;
}
