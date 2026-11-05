// Draft, differ 151 of 132, skeleton gap 12/21 (tla-en, 2026-11-01)
// Finds the object the leader is pushing: among objects 8 to 63 whose sprite
// shape is in the six-entry footprint table, the one whose footprint holds the
// cell one step ahead of the leader, when the leader is not in line with its
// near edge. Returns the object, with its id and the footprint's index; the
// leader's step direction is left in step. Shared with overlay 6a1 and others.
// Left: only allocation. The listing has k, object and leader in r5, r6, r7
// with x, z and left in high registers; here x, z, left and top outrank them
// (lreg priority about 1.0 to 1.35 against 0.8 and 0.64) and take the low
// registers, and every other skeleton line is a move that follows from that.
// The listing also needs objects (lr) to outrank x, which weighted refs do not
// explain. Both loops are goto: a for loop on either level reduces k * 4 and
// hoists the tables, which the listing does not (gap 20 to 22). Swapping the
// comparison operands costs 4; s32 counters and a later work load change
// nothing.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "field_sprite.h"

extern const u16 gData_02005f58[];
extern const s8 gData_02005f64[][4];
extern const s32 gData_02005f7c[];

struct Actor *Actor_GetById(s32 id);

/// The whole-pixel half of a 16.16 coordinate, read as a halfword.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *Func_02004bf4(s32 *step, s32 *id, s32 *footprint) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor **objects;
  struct Actor *object;
  u32 index;
  u32 k;

  *step = leader->angle >> 12;
  index = 8;
  objects = &work->objects[8];
nextObject:
  object = *objects;
  k = 0;
nextShape:
  if (*(s16 *)((struct FieldSprite *)object->animation)->record ==
      gData_02005f58[k]) {
    s32 ahead = gData_02005f7c[*step];
    s32 x = (PIXELS(leader->x) + (ahead >> 16)) >> 4;
    s32 z = (PIXELS(leader->z) + (s16)ahead) >> 4;
    s32 left = (PIXELS(object->x) + gData_02005f64[k][0]) >> 4;
    s32 top = (PIXELS(object->z) + gData_02005f64[k][1]) >> 4;
    s32 right = (PIXELS(object->x) + gData_02005f64[k][2]) >> 4;
    s32 bottom = (PIXELS(object->z) + gData_02005f64[k][3]) >> 4;

    if (left <= x && x < right && top <= z && z < bottom) {
      *footprint = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *id = index;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *id = index;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto nextShape;
  index++;
  objects++;
  if (index <= 63)
    goto nextObject;
  return 0;
}
