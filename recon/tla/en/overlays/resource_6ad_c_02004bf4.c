// Draft, differ 151 of 132: finds the object the leader is pushing: among objects 8
// to 63 whose sprite shape is in the six-entry footprint table, the one whose
// footprint holds the cell one step ahead of the leader, when the leader is
// not in line with its near edge. Returns the object, with its id and the
// footprint's index; the leader's step direction is left in step. Shared with
// overlay 6a1 and others.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"

extern const u16 gData_02005f58[];
extern const s8 gData_02005f64[][4];
extern const s32 gData_02005f7c[];

struct ObjectRuntime *Object_GetById(s32 id);

/// The whole-pixel half of a 16.16 coordinate, read as a halfword.
#define PIXELS(v) (((s16 *)&(v))[1])

struct ObjectRuntime *Func_02004bf4(s32 *step, s32 *id, s32 *footprint) {
  struct ObjectRuntime **objects =
      (struct ObjectRuntime **)((u8 *)Ram_HeapSlots->eventWork + 52);
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
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
          gData_02005f58[k])
        goto skip;
      ahead = gData_02005f7c[*step];
      x = (PIXELS(leader->x) + (ahead >> 16)) >> 4;
      z = (PIXELS(leader->z) + (s16)ahead) >> 4;
      left = (PIXELS(object->x) + gData_02005f64[k][0]) >> 4;
      top = (PIXELS(object->z) + gData_02005f64[k][1]) >> 4;
      right = (PIXELS(object->x) + gData_02005f64[k][2]) >> 4;
      bottom = (PIXELS(object->z) + gData_02005f64[k][3]) >> 4;
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
