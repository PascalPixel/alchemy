#include "jupiter_lighthouse_room_7.h"
#include "call.h"
#include "event_inline.h"

void Func_02002b54(s32 x, s32 z, s32 layer, s32 value);

/// Copies the map cells and sets the cell values that flags 0xa10 to 0xa15 call
/// for.
void Func_020002f0(void) {
  /* FAKEMATCH(wrapper): every call forwarded through the call layer; as direct
   * calls, the coordinates and flags are shared in saved registers and 117
   * instructions differ. */
  MapGrid_CopyCellsLayered(0x5e, 0x3c, 9, 8, 0x5e, 0xa);
  Call4((void (*)())Func_02002b54, 0x2300000, 0xf00000, 0, -2);
  Call4((void (*)())Func_02002b54, 0x1f00000, 0x1000000, 0, -2);
  Call4((void (*)())Func_02002b54, 0x2100000, 0xa00000, 0, -2);
  MapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0);
  MapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0);
  MapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0);
  if (Event_GetFlag(0xa10) ||
      Event_GetFlag(0xa11))
    Func_02002b54(0x2300000, 0xf00000, 0, 0);
  if (Event_GetFlag(0xa12) ||
      Event_GetFlag(0xa13))
    Func_02002b54(0x1f00000, 0x1000000, 0, 0);
  if (Event_GetFlag(0xa14) ||
      Event_GetFlag(0xa15))
    Func_02002b54(0x2100000, 0xa00000, 0, 0);
  if (Event_GetFlag(0xa10)) {
    MapGrid_CopyCellsLayered(0x60, 0x38, 7, 2, 0x60, 0xf);
    MapGrid_CopyCellsLayered(0x6f, 0x33, 1, 1, 0x63, 0xf);
    if (Event_GetFlag(0xa13)) {
      MapGrid_CopyCellsLayered(0x69, 0x33, 1, 3, 0x5f, 0xe);
      MapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0xec);
    }
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  } else if (Event_GetFlag(0xa11)) {
    MapGrid_CopyCellsLayered(0x62, 0x33, 5, 1, 0x62, 0xa);
    MapGrid_CopyCellsLayered(0x63, 0x34, 4, 5, 0x63, 0xb);
    MapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0xec);
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14)) {
      MapGrid_CopyCellsLayered(0x6d, 0x33, 3, 1, 0x5f, 0xa);
      MapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0xec);
    }
  } else {
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa13))
      MapGrid_CopyCellsLayered(0x6b, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14))
      MapGrid_CopyCellsLayered(0x6f, 0x34, 1, 1, 0x61, 0xa);
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  }
}
