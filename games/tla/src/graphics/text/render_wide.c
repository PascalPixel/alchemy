#include "types.h"
#include "ram_buffer.h"
#include "window.h"

s32 Func_0803c274(struct UiWindow *window, u32 c, s32 x, s32 y, s32 flags);

// Draws a string of wide (16-bit) codes into the window, one glyph per cell,
// skipping the control codes and their arguments; code 3 starts a new line.
// With no text, the render work's own entries are drawn, closed with a 0.
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
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) ||                      \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
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
