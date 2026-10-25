#include "types.h"

void Func_0803bb58(s32 no, u32 *pw, u32 *ph, u16 *spacing);
void UiText_MeasureEntryDimensions(s32 no, u32 *pw, u32 *ph, u16 *spacing);

void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                          u16 *spacing, u32 flags) {
  s32 x;
  s32 y;
  s32 limit;
  s32 right;
  s32 bottom;
  s32 over;
  s32 pos;

  x = *px;
  y = *py;
  limit = 30;

  if (!(flags & 2)) {
    if (flags & 1)
      Func_0803bb58(no, pw, ph, spacing);
    else
      UiText_MeasureEntryDimensions(no, pw, ph, spacing);
  }

  if (*pw == 0 && *ph == 0)
    return;

  if (!(flags & 2)) {
    *pw = (*pw + 19) >> 3;
    *ph = (*ph + 15) >> 3;
  }

  right = x + *pw;
  if (right > limit) {
    over = right - limit;
    pos = x - over;
    if (pos >= 0)
      x = pos;
    else
      x = 0;
  }
  bottom = y + *ph;
  if (bottom > 20) {
    over = bottom - 20;
    pos = y - over;
    if (pos >= 0)
      y = pos;
    else
      y = 0;
  }
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (x > limit - *pw)
    x = limit - *pw;
  if (y > 20 - *ph)
    y = 20 - *ph;
  *px = x;
  *py = y;
}
