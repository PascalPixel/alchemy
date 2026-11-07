#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "glyph.h"
#include "edition.h"
#include "version.h"
#include "text_font.h"

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
    /* FAKEMATCH(temporary): the entries' address named before the entry's
     * store, so its offset is live while the count's address is built and
     * that takes r3; as text = work->entries after the store, 8 of 126
     * instructions differ in tla-en. */
    u16 *entries = work->entries;

    work->entries[work->count] = c;
    text = entries;
    /* FAKEMATCH(one-pass): the count update in a one-pass block, which keeps
     * the count's reread after text = entries; as a plain statement, 2 of 126
     * instructions differ in tla-en. */
    do {
      work->count = (work->count + 1) & (UI_TEXT_ENTRY_COUNT - 1);
    } while (0);
  }
  for (;;) {
    c = *text++;
#if (LANGUAGE != LANGUAGE_JAPANESE)
    if (c > 0xff)
      c = 0x40;
#endif
    if (c == 0)
      break;
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (c <= 30) {
#else
    if (c <= 30 || c == 176) {
#endif
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
      case 29:
#endif
        text++;
        break;
      }
    } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
      // A kana followed by a voicing mark carries the mark in its code.
      next = *text;
      if (next == TEXT_MARK_VOICED) {
        c |= 1 << TEXT_MARK_SHIFT;
        text++;
      } else if (next == TEXT_MARK_SEMIVOICED) {
        c |= 2 << TEXT_MARK_SHIFT;
        text++;
      }
#else
      if ((window->flags & 8) == 0) {
        next = *text;
#if TLA_PATCH_4
        if (c > 32 && next > 32) {
          s16 pairWidth =
              gFont_Glyphs[c - 32].width + gFont_Glyphs[next - 32].width;

          // Outlined text joins only pairs one pixel narrower.
          if (work->outline == 1) {
            if ((u16)pairWidth <= 14) {
              c |= next << 8;
              text++;
            }
          } else if ((u16)pairWidth <= 15) {
            c |= next << 8;
            text++;
          }
        }
#else
        if (c > 32 && next > 32 &&
            (u16)(gFont_Glyphs[c - 32].width + gFont_Glyphs[next - 32].width) <=
                15) {
          c |= next << 8;
          text++;
        }
#endif
      }
#endif
      x += Func_0803bde4(window, c, x, y, 0);
    }
  }
}
