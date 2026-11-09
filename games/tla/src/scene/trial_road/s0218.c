#include "trial_road.h"
#include "event_inline.h"

void Func_02000218(s32 side, s32 phase) {
  if (side == 0) {
    if (phase == 3)
      Func_0802b450(0, 120, 1, 2, 23, phase);
    MapGrid_CopyCellsLayered(127, 3, 1, 2, 57, 3);
    MapGrid_CopyCellAttributes(0, 127, 1, 1, 23, 8);
  } else {
    if (phase == 3)
      Func_0802b450(0, 120, 1, 2, 9, phase);
    MapGrid_CopyCellsLayered(127, 3, 1, 2, 43, 3);
    MapGrid_CopyCellAttributes(1, 127, 1, 1, 9, 8);
  }
}

void Func_020002a4(s32 side, s32 phase) {
  if (side == 0) {
    if (phase == 3)
      Func_0802b450(0, 120, 1, 2, 28, phase);
    MapGrid_CopyCellsLayered(127, 3, 1, 2, 62, 3);
    MapGrid_CopyCellAttributes(1, 127, 1, 1, 28, 8);
  } else {
    if (phase == 3)
      Func_0802b450(0, 120, 1, 2, 4, phase);
    MapGrid_CopyCellsLayered(127, 3, 1, 2, 38, 3);
    MapGrid_CopyCellAttributes(0, 127, 1, 1, 4, 8);
  }
}
