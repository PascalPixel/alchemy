/// @file
/// Measures a push: finds the pushable actor ahead of the leader
/// (Func_020043a8, recording its number and kind in the push state), then
/// steps the actor's box one cell at a time along the leader's direction
/// until a cell of it is blocked (Func_02004a64 returns 2); with at least one
/// free step it records the end position in the push state and returns 1,
/// else 0. The actor stays on layer 2 while it is measured. The same code as
/// overlay 674's Func_020037d8 and overlay 662's Func_020001d4.

#include "types.h"
#include "actor.h"
#include "system/vector3.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

extern s8 gData_02004cb0[][4];
extern s32 gData_02004cc8[];
struct Actor *Func_020043a8(s32 *direction, s32 *number, s32 *kind);
s32 Func_02004a64(struct Actor *actor, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

static __inline__ s32 CallFunc_02004a64(struct Actor *actor,
                                        struct Vector3 *cell) {
  return Func_02004a64(actor, cell);
}

s32 Func_02004240(struct PushState *push) {
  struct Actor *actor;
  struct Vector3 cell;
  s32 direction, kind, width, height, dx, dz, steps, i, j, offset;
  u8 *layer;

  actor = Func_020043a8(&direction, &push->number, &push->kind);
  if (actor != 0) {
    kind = push->kind;
    width = (Abs(gData_02004cb0[kind][0]) + Abs(gData_02004cb0[kind][2])) >> 4;
    height = (Abs(gData_02004cb0[kind][1]) + Abs(gData_02004cb0[kind][3])) >> 4;
    offset = gData_02004cc8[direction];
    dx = offset & 0xffff0000;
    dz = offset << 16;
    push->unk14 = 0;
    steps = 0;
    layer = &actor->terrainId;
    *layer = 2;
    push->x = actor->x + dx;
    push->z = actor->z + dz;
    cell.y = actor->y;
    for (;;) {
      cell.z = push->z + (gData_02004cb0[kind][1] << 16);
      for (j = 0; j < height; j++) {
        cell.x = push->x + (gData_02004cb0[kind][0] << 16);
        for (i = 0; i < width; i++) {
          /* FAKEMATCH(wrapper): the call through CallFunc_02004a64; as a direct
           * call, 2 of 171 instructions differ in tla-en. */
          if (CallFunc_02004a64(actor, &cell) == 2)
            goto blocked;
          cell.x += 0x100000;
        }
        cell.z += 0x100000;
      }
      push->x += dx;
      steps++;
      push->z += dz;
    }
  blocked:
    *layer = 0;
    if (steps == 0)
      return 0;
    {
      s32 move = dx * steps;

      push->x = actor->x + move;
    }
    push->y = actor->y;
    push->z = actor->z + dz * steps;
    return 1;
  }
  return 0;
}

#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02004ca4[];

struct Actor *Func_020043a8(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->angle >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->animation)->value28[0] ==
      gData_02004ca4[k]) {
    x = (PIXELS(leader->x) + (gData_02004cc8[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_02004cc8[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02004cb0[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_02004cb0[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_02004cb0[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02004cb0[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}

