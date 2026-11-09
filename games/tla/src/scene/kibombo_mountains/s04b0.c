#include "types.h"
#include "event_inline.h"

void Func_020004b0(s32 unused, s32 cell) {
  if (cell == 16 || cell == 17)
    MapGrid_CopyCellAttributes(120, 33, 8, 2, 20, 33);
  if (cell == 14 || cell == 15)
    MapGrid_CopyCellAttributes(112, 15, 16, 4, 8, 15);
}
