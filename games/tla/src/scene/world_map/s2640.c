#include "types.h"
#include "event_inline.h"

void Func_02002618(s32 id, s32 dx, s32 dz);

void Func_02002640(s32 a, s32 b, s32 dx, s32 dz) {
  WorldMap_SetWindowCellTileAndDraw(a, b, 48, 224);
  Func_02002618(5, dx, dz);
  Func_02002618(8, dx, dz);
  Func_02002618(9, dx, dz);
  Func_02002618(10, dx, dz);
  WorldMap_SetWindowCellTileAndDraw(a + 16, b, 64, 224);
}
