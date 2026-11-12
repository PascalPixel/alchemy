/// @file
/// Scene code of overlay 0x674, in ROM order, from s37d8's offset up to the next object the overlay links.

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

extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind);
s32 Func_02003e2c(struct Actor *object, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

static __inline__ s32 RowByte(s8 *row, s32 index) { return row[index]; }

static __inline__ s32 BoxSpan(s32 kind, s32 start) {
  return (Abs(gData_0200407c[kind][start]) +
          Abs(gData_0200407c[kind][start + 2])) >>
         4;
}

static __inline__ s32 CallFunc_02003e2c(struct Actor *object,
                                        struct Vector3 *cell) {
  return Func_02003e2c(object, cell);
}

s32 Func_020037d8(struct PushState *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = Func_02003940(&direction, &push->number, &push->kind);
  if (object != 0) {
    kind = push->kind;
    /* FAKEMATCH(dead): the address of kind taken and dropped; as kind =
     * push->kind alone, 127 of 171 instructions differ in tla-en. */
    (void)&kind;
    {
      s32 width = BoxSpan(kind, 0);
      s32 height = BoxSpan(kind, 1);
      u8 *layer;
      s32 index;

      offset = gData_02004094[direction];
      dx = offset & 0xffff0000;
      dz = offset << 16;
      push->unk14 = 0;
      steps = 0;
      layer = &object->terrainId;
      *layer = 2;
      push->x = object->x + dx;
      push->z = object->z + dz;
      cell.y = object->y;
      for (;;) {
        s8 *table = gData_0200407c[0];

        index = kind * 4;
        /* FAKEMATCH(wrapper): the read through RowByte; as table[index + 1],
         * 27 of 171 instructions differ in tla-en. */
        cell.z = push->z + (RowByte(table, index + 1) << 16);
        for (j = 0; j < height; j++) {
          cell.x = push->x + (gData_0200407c[0][index] << 16);
          for (i = 0; i < width; i++) {
            /* FAKEMATCH(wrapper): the call through CallFunc_02003e2c; as a
             * direct call, 2 of 171 instructions differ in tla-en. */
            if (CallFunc_02003e2c(object, &cell) == 2)
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
    }
    result = 0;
    if (steps != 0) {
      push->x = object->x + dx * steps;
      result = 1;
      push->y = object->y;
      push->z = object->z + dz * steps;
    }
  } else
    result = 0;
  return result;
}

#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02004070[];

struct Actor *Func_02003940(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gData_02004070[k]) {
    x = (PIXELS(leader->x) + (gData_02004094[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_02004094[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_0200407c[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_0200407c[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_0200407c[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_0200407c[k][3]) >> 4;
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

