// Draft, differ 95 of 126: overlay 674's copy of overlay 668's Func_020040ec
// (the same listing, calling Func_02003790 where 668 calls
// Map_FillCellRectByte2), whose draft holds its notes.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

extern u16 gData_02004070[];
extern s8 gData_0200407c[][4];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02003e24(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02003790(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

s32 Func_02003684(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = Object_GetById(id);
  u16 kind = *((struct ObjectAnimationState *)object->animation)->value28;
  u16 i = 0;
  s8 *box;
  s32 width, height, x, z;

  if (gData_02004070[0] != kind) {
    do {
      i++;
      if (i > 5)
        break;
    } while (gData_02004070[i] != kind);
  }
  if (i == 6)
    return 0;
  box = gData_0200407c[i];
  width = (Abs(box[0]) + Abs(box[2])) >> 4;
  height = (Abs(box[1]) + Abs(box[3])) >> 4;
  x = (object->x + (box[0] << 16)) >> 20;
  z = (object->z + (box[1] << 16)) >> 20;
  Func_02003e24(x, z, width, height,
                x + (u16)(*(s32 *)(map + 0x148) >> 20),
                z + (u16)(*(s32 *)(map + 0x14c) >> 20));
  Func_02003790(0, x, z, width, height, 255);
  Func_02003790(2, x, z, width, height, 255);
  return 1;
}
