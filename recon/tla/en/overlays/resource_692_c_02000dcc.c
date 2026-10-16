// Draft, differ 8 of 34: the cell watch's table lookup (id, cell, value rows
// ended by 0xffff). The native rebuilds 0xffff on every pass; loop.c hoists it
// here. Goto loops keep it inside but change the layout or the registers: an
// if/goto loop (13), the while layout spelled with gotos (25 with the result
// set before the cell call, 15 after). The result is set before the cell call
// so it takes r7.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"

struct CellWatch {
  u8 unk00[4];
  s32 x;
  u8 unk08[4];
  s32 z;
  u8 unk10[4];
  struct ObjectRuntime *object;
};

s32 Func_02000658(u32 layer, s32 x, s32 y);

s32 Func_02000dcc(u16 *table) {
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 cell = Func_02000658(0, watch->x >> 20, watch->z >> 20) >> 8;
  u32 id;

  while (*table != 0xffff) {
    id = *table++;
    if (cell == *table++) {
      watch->object = Object_GetById(id);
      result = *table;
      break;
    }
    result = *table++;
  }
  return result;
}
