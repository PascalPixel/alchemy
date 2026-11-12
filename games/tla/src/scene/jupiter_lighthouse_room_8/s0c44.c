/// @file
/// Scene code of overlay 0x6a1, in ROM order, from s0c44's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern s8 gData_020022dc[][4];
extern s32 gData_020022f4[];

struct Actor *Func_02000dac(s32 *direction, s32 *slot, s32 *kind);
s32 Func_020021b8(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_020021b8_(struct Actor *object, s32 *position) {
  return Func_020021b8(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (Func_020021b8 returns 2). Leaves the object's resting place in target and
/// returns 1, or 0 when nothing is pushed or it cannot move at all.
s32 Func_02000c44(struct PushTarget *target) {
  struct Actor *object;
  s32 direction;
  s32 position[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  object = Func_02000dac(&direction, &target->id, &target->footprint);
  if (object != 0) {
    footprint = target->footprint;
    width = (ABS(gData_020022dc[footprint][0]) +
             ABS(gData_020022dc[footprint][2])) >>
            4;
    height = (ABS(gData_020022dc[footprint][1]) +
              ABS(gData_020022dc[footprint][3])) >>
             4;
    dx = gData_020022f4[direction] & 0xffff0000;
    dz = gData_020022f4[direction] << 16;
    target->callback = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    target->x = object->x + dx;
    target->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      position[2] = target->z + (gData_020022dc[footprint][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = target->x + (gData_020022dc[footprint][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_020021b8_; as
           * Func_020021b8(object, position), 2 of 171 instructions differ in
           * tla-en. */
          if (Func_020021b8_(object, position) == 2)
            goto blocked;
          position[0] += 0x100000;
        }
        position[2] += 0x100000;
      }
      target->x += dx;
      count++;
      target->z += dz;
    }
  blocked:
    *layer = 0;
    if (count == 0)
      return 0;
    target->x = object->x + dx * count;
    target->y = object->y;
    target->z = object->z + dz * count;
    return 1;
  }
  return 0;
}

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_020022d0[];
extern s8 gData_020022dc[][4];
extern s32 gData_020022f4[];

struct Actor *Func_02000dac(s32 *direction, s32 *slot, s32 *kind) {
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
      gData_020022d0[k]) {
    x = (PIXELS(leader->x) + (gData_020022f4[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_020022f4[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_020022dc[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_020022dc[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_020022dc[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_020022dc[k][3]) >> 4;
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
