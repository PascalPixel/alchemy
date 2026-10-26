// Draft, differ 79 of 126: finds an object's footprint by its sprite's shape in a
// six-entry table and copies the cells under it (Func_020054e8), then fills
// byte 2 of those cells with 255 on layers 0 and 2; returns 0 for a shape not
// in the table. Shared with overlay 6a1 and others.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u16 gData_02005f58[];
extern const s8 gData_02005f64[][4];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020054e8(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_02004a44(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02004938(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = Object_GetById(id);
  u16 shape = *(s16 *)((struct FieldSprite *)object->animation)->record;
  s16 i = 0;
  const s8 *rect;
  s32 width;
  s32 height;
  s32 x;
  s32 z;

  if (gData_02005f58[0] != shape) {
  next:
    i++;
    if ((u16)i <= 5 && gData_02005f58[(u16)i] != shape)
      goto next;
  }
  if ((u16)i == 6)
    return 0;
  rect = gData_02005f64[(u16)i];
  width = (ABS(rect[0]) + ABS(rect[2])) >> 4;
  height = (ABS(rect[1]) + ABS(rect[3])) >> 4;
  x = (object->x + (rect[0] << 16)) >> 20;
  z = (object->z + (rect[1] << 16)) >> 20;
  Func_020054e8(x, z, width, height,
                x + (u16)(*(s32 *)(map + 328) >> 20),
                z + (u16)(*(s32 *)(map + 332) >> 20));
  Func_02004a44(0, x, z, width, height, 255);
  Func_02004a44(2, x, z, width, height, 255);
  return 1;
}
