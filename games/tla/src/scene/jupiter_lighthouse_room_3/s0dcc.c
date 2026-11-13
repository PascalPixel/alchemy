/// @file
/// Scene code of overlay 0x69c, in ROM order, from s0dcc's offset up to the
/// next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "map.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

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

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

/// Finds the object the leader is pushing: among objects 8 to 63 whose sprite
/// shape is in the six-entry footprint table, the one whose footprint holds the
/// cell one step ahead of the leader, when the leader is not in line with its
/// near edge. Returns the object, with its id and the footprint's index.
DEFINE_FUNC_02000DAC(Func_02000f34, gData_02002478, gData_0200249c,
    gData_02002484)

void Func_020022e0(struct Actor *object, s32 mode);
void Func_02001300(void);

DEFINE_FUNC_02000EC8(Func_02001050, gData_02002484, Func_02001300,
    Func_020022e0, gData_0200249c)
