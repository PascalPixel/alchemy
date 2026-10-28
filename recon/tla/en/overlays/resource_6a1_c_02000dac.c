// Draft, differ 151 of 132: overlay 6a1's copy of the function drafted in
// resource_69c_c_02000f34.c (overlay 69c's Func_02000f34, differ 151 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_sprite.h"

extern const u16 gData_020022d0[];
extern const s8 gData_020022dc[][4];
extern const s32 gData_020022f4[];

struct ObjectRuntime *Object_GetById(s32 id);

/// The whole-pixel half of a 16.16 coordinate, read as a halfword.
#define PIXELS(v) (((s16 *)&(v))[1])

struct ObjectRuntime *Func_02000dac(s32 *step, s32 *id, s32 *footprint) {
  struct ObjectRuntime **objects =
      (struct ObjectRuntime **)((u8 *)Ram_HeapSlots->eventWork + 52);
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *object;
  u32 index;
  u32 k;
  s32 ahead;
  s32 x;
  s32 z;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  *step = leader->angle >> 12;
  index = 8;
nextObject:
  object = *objects;
  k = 0;
  {
  nextShape:
    {
      if (*(s16 *)((struct FieldSprite *)object->animation)->record !=
          gData_020022d0[k])
        goto skip;
      ahead = gData_020022f4[*step];
      x = (PIXELS(leader->x) + (ahead >> 16)) >> 4;
      z = (PIXELS(leader->z) + (s16)ahead) >> 4;
      left = (PIXELS(object->x) + gData_020022dc[k][0]) >> 4;
      top = (PIXELS(object->z) + gData_020022dc[k][1]) >> 4;
      right = (PIXELS(object->x) + gData_020022dc[k][2]) >> 4;
      bottom = (PIXELS(object->z) + gData_020022dc[k][3]) >> 4;
      if (left > x || x >= right || top > z || z >= bottom)
        goto skip;
      *footprint = k;
      if (k & 1) {
        if (left == leader->x >> 20)
          goto skip;
      } else {
        if (top == leader->z >> 20)
          goto skip;
      }
      *id = index;
      return object;
    }
  skip:
    k++;
    if (k <= 5)
      goto nextShape;
  }
  index++;
  objects++;
  if (index <= 63)
    goto nextObject;
  return 0;
}
