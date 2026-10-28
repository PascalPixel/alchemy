// Draft, differ 73 of 126 (was 77): overlay 6a1's copy of the function drafted
// in resource_69c_c_02000c78.c (overlay 69c's Func_02000c78), whose header says
// what it is and what was tried. Forwarding one call through an inline wrapper
// (a FAKEMATCH(wrapper) on adoption) gained 4.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"

extern const u16 gData_020022d0[];
extern const s8 gData_020022dc[][4];

struct Actor *Actor_GetById(s32 id);
void Func_020021b0(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static __inline__ void Func_020021b0_(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ) {
  Func_020021b0(x, z, width, height, toX, toZ);
}

s32 Func_02000af0(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  u16 shape = *(s16 *)((struct FieldSprite *)object->animation)->record;
  s16 i = 0;
  const s8 *rect;
  s32 width;
  s32 height;
  s32 x;
  s32 z;

  if (gData_020022d0[0] != shape) {
  next:
    i++;
    if ((u16)i <= 5 && gData_020022d0[(u16)i] != shape)
      goto next;
  }
  if ((u16)i == 6)
    return 0;
  rect = gData_020022dc[(u16)i];
  width = (ABS(rect[0]) + ABS(rect[2])) >> 4;
  height = (ABS(rect[1]) + ABS(rect[3])) >> 4;
  x = (object->x + (rect[0] << 16)) >> 20;
  z = (object->z + (rect[1] << 16)) >> 20;
  Func_020021b0_(x, z, width, height,
                x + (u16)(*(s32 *)(map + 328) >> 20),
                z + (u16)(*(s32 *)(map + 332) >> 20));
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
