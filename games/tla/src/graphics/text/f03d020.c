#include "types.h"
#include "ram_buffer.h"
#include "window.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiText_MeasureEntryDimensions(s32 entry, s32 *width, s32 *height,
                                   s32 flags);
void UiWork_ResetCounters(void);
s32 Func_0803cfd0(u8 *canvas, s32 ch, s32 x, s32 row);

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
  x = (192 - width) / 2;
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
      x += Func_0803cfd0(canvas, code, x, y);
    }
    code = *text++;
  }
  return 0;
}
