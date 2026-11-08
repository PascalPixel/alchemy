/// @file
/// Finds how far the object the leader pushes can slide.

#include "types.h"
#include "call.h"
#include "actor.h"

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern s8 gData_02002484[][4];
extern s32 gData_0200249c[];

struct Actor *Func_02000f34(s32 *direction, s32 *slot, s32 *kind);
s32 Func_02002338(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_02002338_(struct Actor *object, s32 *position) {
  return Func_02002338(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (Func_02002338 returns 2). Leaves the object's resting place in target and
/// returns 1, or 0 when nothing is pushed or it cannot move at all.
s32 Func_02000dcc(struct PushTarget *target) {
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

  object = Func_02000f34(&direction, &target->id, &target->footprint);
  if (object != 0) {
    footprint = target->footprint;
    width = (ABS(gData_02002484[footprint][0]) +
             ABS(gData_02002484[footprint][2])) >>
            4;
    height = (ABS(gData_02002484[footprint][1]) +
              ABS(gData_02002484[footprint][3])) >>
             4;
    dx = gData_0200249c[direction] & 0xffff0000;
    dz = gData_0200249c[direction] << 16;
    target->callback = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    target->x = object->x + dx;
    target->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      position[2] = target->z + (gData_02002484[footprint][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = target->x + (gData_02002484[footprint][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_02002338_; as
           * Func_02002338(object, position), 2 of 171 instructions differ in
           * tla-en. */
          if (Func_02002338_(object, position) == 2)
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
