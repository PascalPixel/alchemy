// Draft, differ 95 of 126: for a pushable object (its animation one of the six
// kinds in gData_02004ca4), copies the cells under its box (gData_02004cb0:
// four signed bytes a kind, in sixteenths of a cell) from the map's spare
// area (Func_02004a54) and fills both layers' byte 2 there with 255; returns
// 1, or 0 for an object of no known kind. The same listing as overlay 674's
// Func_02003684 and 662's Func_020001d4's family (69c, 6a1, 6ad). Left: the
// native's search keeps the kind as a shifted halfword in r4 and the index
// as a halfword pair, and it takes the absolute values through branches in
// another order; registers differ from there on.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

extern u16 gData_02004ca4[];
extern s8 gData_02004cb0[][4];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02004a54(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_020040ec(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = Object_GetById(id);
  u16 kind = *((struct BattleAnimationState *)object->animation)->value28;
  u16 i = 0;
  s8 *box;
  s32 width, height, x, z;

  if (gData_02004ca4[0] != kind) {
    do {
      i++;
      if (i > 5)
        break;
    } while (gData_02004ca4[i] != kind);
  }
  if (i == 6)
    return 0;
  box = gData_02004cb0[i];
  width = (Abs(box[0]) + Abs(box[2])) >> 4;
  height = (Abs(box[1]) + Abs(box[3])) >> 4;
  x = (object->x + (box[0] << 16)) >> 20;
  z = (object->z + (box[1] << 16)) >> 20;
  Func_02004a54(x, z, width, height,
                x + (u16)(*(s32 *)(map + 0x148) >> 20),
                z + (u16)(*(s32 *)(map + 0x14c) >> 20));
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
