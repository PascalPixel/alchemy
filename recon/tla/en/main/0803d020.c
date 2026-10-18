// Draft, differ 14 of 112: builds a message's render entries and draws them
// into a window centred in 192 pixels, running the colour, outline, line
// spacing and new line codes as ☀️ UiText_RenderWideStringAtOffset
// (games/tbs/src/window/window.c) does; a message that starts with dashes
// draws nothing and returns how many (0 to 3). Left: instruction order of
// constant builds against loads (the three 0xf3x offsets, 0x1342 and 0xf42),
// and the two-operand skip (codes 14, 15, 28): native steps once and jumps
// into the one-operand skip; text++ twice is combined into one add of 4.
// Tried: dash tests as a flat chain (differ 35 with the movs r0, #1 hoisted);
// x not initialized (differ 71: x and count's zero take different registers).
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiText_MeasureEntryDimensions(s32 entryIndex, u32 *outWidth,
                                   u32 *outHeight, s32 mode);
void UiWork_ResetCounters(void);
s32 Func_0803cfd0(struct UiWindow *window, u32 code, s32 x, s32 y);

s32 Func_0803d020(s32 message, struct UiWindow *window) {
  struct UiRenderWork *work = Ram_HeapSlots->windowTiles;
  u32 width;
  u32 height;
  s32 entry;
  u16 *text;
  u32 code;
  s32 x;
  s32 y;

  x = 0;
  y = 0;
  work->count = 0;
  entry = UiText_BuildRenderEntries(message, 0);
  UiText_MeasureEntryDimensions(entry, &width, &height, 0);
  text = &work->entries[entry];
  if (text[0] == '-') {
    if (text[1] == '-') {
      if (text[2] == '-')
        return text[3] == '-' ? 3 : 2;
      return 1;
    }
    return 0;
  }
  x = (s32)(192 - width) / 2;
  while ((code = *text++) != 0) {
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
        text++;
        break;
      }
    } else {
      x += Func_0803cfd0(window, code, x, y);
    }
  }
  return 0;
}
