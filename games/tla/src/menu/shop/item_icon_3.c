#include "item_icon.h"

void Func_0810bf98(s16 *window, s32 x, s32 y, s32 owner) {
  u8 *unit = UnitGet(owner);
  s32 left = window[6] * 8 + x + 8;
  s32 top = window[7] * 8 + y + 8;
  s32 i;

  Func_0810bebc((u16 *)(unit + 216), ICON_COUNT, 27);
  for (i = 0; i < ICON_COUNT; i++)
    Func_0810be70(i, left + i % 5 * 16, top + i / 5 * 16);
  Func_0810bea8(1);
}
