/// @file
/// How far a pushed object can slide.

#include "types.h"
#include "actor.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern s8 gData_02001974[];
extern s32 gData_0200198c[];

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind);
s32 Func_02001850(struct Actor *object, s32 *position);

static inline s32 Func_02001850_Call(struct Actor *object, s32 *position) {
  return Func_02001850(object, position);
}

/// Finds how far a pushed object can slide in the leader's direction, probing
/// its footprint cell by cell until Func_02001850 reports a wall. Returns 1
/// with the end position in position[0] to position[2], or 0 if it cannot
/// move.
s32 Func_020001d4(s32 *position) {
  s32 probe[3];
  s32 direction;
  struct Actor *object;
  s32 kind, w, h, dx, dz, count, i, j;
  u8 *state;
  s32 offset;

  object = Func_0200033c(&direction, &position[4], &position[3]);
  if (object != NULL) {
    kind = position[3];
    w = (ABS(gData_02001974[kind * 4]) + ABS(gData_02001974[kind * 4 + 2])) >>
        4;
    h = (ABS(gData_02001974[kind * 4 + 1]) +
         ABS(gData_02001974[kind * 4 + 3])) >>
        4;
    dx = gData_0200198c[direction] & 0xffff0000;
    dz = gData_0200198c[direction] << 16;
    position[5] = 0;
    count = 0;
    state = &object->terrainId;
    *state = 2;
    position[0] = object->x + dx;
    position[2] = object->z + dz;
    probe[1] = object->y;
    // The row loop is built from labels: as a loop it grows by 14 bytes.
  next:
    offset = kind;
    probe[2] = position[2] + (gData_02001974[offset * 4 + 1] << 16);
    offset *= 4;
    j = 0;
    if (j >= h)
      goto advance;
  row:
    probe[0] = position[0] + (gData_02001974[offset] << 16);
    for (i = 0; i < w; i++) {
      /* FAKEMATCH(wrapper): the call through Func_02001850_Call; as
         Func_02001850(object, probe), 2 of 171 instructions differ in tla-en.
       */
      if (Func_02001850_Call(object, probe) == 2)
        goto hit;
      probe[0] += 0x100000;
    }
    probe[2] += 0x100000;
    j++;
    if (j < h)
      goto row;
  advance:
    position[0] += dx;
    count++;
    position[2] += dz;
    goto next;
  hit:
    *state = 0;
    if (count == 0)
      return 0;
    position[0] = object->x + dx * count;
    position[1] = object->y;
    position[2] = object->z + dz * count;
    return 1;
  } else {
    return 0;
  }
}
