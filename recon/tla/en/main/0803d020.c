// Draft, differ 8 of 112 (2026-10-24): a one-pass block around the centring of
// x takes one reordered pair off. Left: instruction order only.
// Draft, differ 10 of 112 (2026-10-18): only scheduling remains; the native
// loads the work pointer before it builds the count offset, and in the
// colour, outline and line-spacing cases and the loop start it loads the
// entry between the two instructions of the split offset. A barrier after the
// work pointer moves the frame setup (18).
// Draws message centred on a 192-pixel-wide text canvas with shadowed
// glyphs, running the colour, outline, line spacing and new line codes as
// ☀️'s UiText_RenderWideStringAtOffset does. A message starting "-" draws
// nothing and returns 0, 1, 2 or 3 by how many dashes open it.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiText_MeasureEntryDimensions(s32 entry, s32 *width, s32 *height,
                                   s32 flags);
void UiWork_ResetCounters(void);
s32 UiText_DrawShadowedGlyph(u8 *canvas, s32 ch, s32 x, s32 row);

s32 Func_0803d020(s32 message, u8 *canvas) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  s32 x = 0;
  s32 y = 0;
  s32 entry;
  s32 width;
  s32 height;
  u16 *text;
  u32 code;

  work->count = x;
  entry = UiText_BuildRenderEntries(message, 0);
  UiText_MeasureEntryDimensions(entry, &width, &height, 0);
  text = &work->entries[entry];
  if (text[0] == '-') {
    if (text[1] != '-')
      return 0;
    if (text[2] == '-')
      return text[3] == '-' ? 3 : 2;
    return 1;
  }
  do {
    x = (192 - width) / 2;
  } while (0);
  code = *text++;
  while (code != 0) {
    if (code <= 30) {
      switch (code) {
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
      case 11:
      case 12:
      case 17:
      skip:
        text++;
        break;
      case 7:
        UiWork_ResetCounters();
        break;
      case 3:
        x = 0;
        y += 15;
        break;
      case 14:
      case 15:
      case 28:
        text++;
        goto skip;
      }
    } else {
      x += UiText_DrawShadowedGlyph(canvas, code, x, y);
    }
    code = *text++;
  }
  return 0;
}
