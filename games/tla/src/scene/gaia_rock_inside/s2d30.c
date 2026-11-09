/// @file
/// Opens the passages on each side of the hall by how many of its two pairs of flags
/// (0x9bc and 0x9bd, 0x9be and 0x9bf) are set: both open the wide way, one a side way, and
/// a single side opened alone also opens its connecting cells.
#include "types.h"
#include "event_inline.h"


void Func_02002d30(void) {
  u8 a = GameFlag_Test(0x9bc);
  u8 third = GameFlag_Test(0x9be);
  s32 b = GameFlag_Test(0x9bd);
  s8 first = a;
  s8 sideThird = third;
  u8 sumLeft = b + first;
  s32 d = GameFlag_Test(0x9bf);
  u8 sumRight = sideThird + d;
  s8 left = sumLeft;
  s8 right = sumRight;

  if (left != 0) {
    if (left == 2) {
      MapGrid_CopyCellsLayered(74, 115, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(71, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(67, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(79, 110, 1, 3, 30, 85);
    } else if (first != 0) {
      MapGrid_CopyCellsLayered(66, 95, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(81, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(75, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(65, 110, 1, 3, 30, 85);
    } else {
      MapGrid_CopyCellsLayered(69, 82, 5, 12, 26, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(69, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(73, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(77, 110, 1, 3, 30, 85);
    }
  }
  if (right == 0)
    return;
  if (right == 2) {
    MapGrid_CopyCellsLayered(83, 115, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(82, 124, 1, 3, 30, 85);
  } else if (sideThird != 0) {
    MapGrid_CopyCellsLayered(76, 82, 4, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(75, 91, 1, 3, 30, 85);
  } else {
    MapGrid_CopyCellsLayered(82, 82, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(81, 91, 1, 3, 30, 85);
  }
}
