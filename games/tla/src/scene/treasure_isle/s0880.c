#include "types.h"
#include "event_inline.h"

struct TreasureIsleCell {
  s32 x;
  s32 z;
  s32 unk08;
  u16 flag;
  u16 unk0e;
};

extern struct TreasureIsleCell gData_0200117c[];
extern struct TreasureIsleCell gData_020011cc[];

void Func_02000880(s32 unused, s32 frame, s32 id) {
  struct TreasureIsleCell *cells;

  if (id < 50) {
    cells = gData_0200117c;
    id -= 40;
  } else {
    cells = gData_020011cc;
    id -= 50;
  }
  if (frame == 15) {
    MapGrid_CopyCellsLayered(3, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
  } else if (frame == 30) {
    MapGrid_CopyCellsLayered(6, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
  } else if (frame == 45) {
    MapGrid_CopyCellsLayered(9, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, cells[id].x, cells[id].z);
    Event_ClearFlag(cells[id].flag);
  }
}
