#include "types.h"

void Func_08019000(s32, s32, s32, s32, s32);
volatile int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  int one;
  s32 tile;
  s32 tmp;
  s32 y;
  int pos;
  tmp = 2;
  tmp = tmp;
  tmp = arg3 * tmp;
  tile = tmp + 0xF315;
  y = arg2;
  Func_08019000(arg0, 0x400 | tile, arg1, y, 0);
  if (1)
  {
    one = 1;
    Func_08019000(arg0, tmp + 0xF314, arg1 + one, arg2, 0);
    pos = arg1 + 2;
    Func_08019000(arg0, tile, pos, arg2, 0);
  }
}
