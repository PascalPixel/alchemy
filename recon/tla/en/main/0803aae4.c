// Draft, differ 2 of 126 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 10 of 126: The Broken Seal's international
// Text_DrawWideStringAtOffset with ⚓️'s additions: code 176 joins the
// control codes (with no case of its own) and glyphs are drawn by
// Func_0803bde4. All ten rows are one kind of swap, five times: the native
// schedules a halfword load one slot earlier than sched2 ranks it here (the
// text read between the halves of the split 0xf3e, 0xf3c and 0xf38 in cases
// 8 to 10, the count reread after the entry store), the same one-slot load
// order as this lane's other near misses.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "glyph.h"

void Text_ResetStyle(void);
s32 Func_0803bde4(struct Window *window, u32 c, s32 x, s32 y, s32 flags);

void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                     s32 y) {
  struct WindowWork *work;
  u32 c;
  u32 next;
  s16 start;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  c = 0;
  start = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = c;
    work->count = (work->count + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  }
  for (;;) {
    c = *text++;
    if (c > 0xff)
      c = 0x40;
    if (c == 0)
      break;
    if (c <= 30 || c == 176) {
      switch (c) {
      case 8:
        work->colour = *text;
        text++;
        break;
      case 9:
        work->outline = *text;
        text++;
        break;
      case 10:
        work->lineSpacing = *text;
        text++;
        break;
      case 7:
        Text_ResetStyle();
        break;
      case 3:
        x = start;
        y += 15;
        break;
      case 14:
      case 15:
      case 28:
        text++;
      case 11:
      case 12:
      case 17:
      case 29:
        text++;
        break;
      }
    } else {
      if ((window->flags & 8) == 0) {
        next = *text;
        if (c > 32 && next > 32 &&
            (u16)(gFont_Glyphs[c - 32].width +
                  gFont_Glyphs[next - 32].width) <= 15) {
          c |= next << 8;
          text++;
        }
      }
      x += Func_0803bde4(window, c, x, y, 0);
    }
  }
}
