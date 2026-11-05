// Draft, differ 59: overlay 6a1's copy of the function drafted in
// resource_69c_c_02000f34.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field/event_work.h"

extern const u16 gData_020022d0[];
extern const s8 gData_020022dc[][4];
extern const s32 gData_020022f4[];

struct Actor *Actor_GetById(s32 id);

/// The whole-pixel half of a 16.16 coordinate, read as a halfword.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *Func_02000dac(s32 *step, s32 *id, s32 *footprint) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor **objects;
  struct Actor *object;
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
  objects = &work->objects[8];
nextObject:
  object = *objects;
    k = 0;
  nextShape:
    {
      if (*(s16 *)((struct FieldSprite *)object->animation)->record !=
          gData_020022d0[k])
        goto skip;
      x = (PIXELS(leader->x) + (ahead >> 16)) >> 4;
      ahead = gData_020022f4[*step];
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
        *id = index;
        return object;
      } else {
        if (top == leader->z >> 20)
          goto skip;
        *id = index;
        return object;
      }
    }
  skip:
    k++;
    if (k <= 5)
      goto nextShape;
  objects++;
  index++;
  if (index <= 63)
    goto nextObject;
  return 0;
}
