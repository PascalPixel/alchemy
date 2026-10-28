// Draft, differ 235 of 171: finds how far a pushed object can slide in the
// leader's direction, probing its footprint cell by cell until
// Func_02001850 reports a wall. Left: register choice and the frame (the
// native keeps the kind on the stack).
#include "types.h"
#include "actor.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern s8 gData_02001974[][4];
extern s32 gData_0200198c[];

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind);
s32 Func_02001850(struct Actor *object, s32 *position);

s32 Func_020001d4(s32 *position) {
  s32 probe[3];
  s32 direction;
  struct Actor *object;
  s32 kind, w, h, dx, dz, count, i, j;
  u8 *state;

  object = Func_0200033c(&direction, &position[4], &position[3]);
  if (object == NULL)
    return 0;
  kind = position[3];
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
  dx = gData_0200198c[direction] & 0xffff0000;
  dz = gData_0200198c[direction] << 16;
  position[5] = 0;
  count = 0;
  state = &object->terrainId;
  *state = 2;
  position[0] = object->x + dx;
  position[2] = object->z + dz;
  probe[1] = object->y;
next:
  {
    probe[2] = position[2] + (gData_02001974[kind][1] << 16);
    for (j = 0; j < h; j++) {
      probe[0] = position[0] + (gData_02001974[kind][0] << 16);
      for (i = 0; i < w; i++) {
        if (Func_02001850(object, probe) == 2)
          goto hit;
        probe[0] += 0x100000;
      }
      probe[2] += 0x100000;
    }
    position[0] += dx;
    count++;
    position[2] += dz;
  }
  goto next;
hit:
  *state = 0;
  if (count == 0)
    return 0;
  position[0] = object->x + dx * count;
  position[1] = object->y;
  position[2] = object->z + dz * count;
  return 1;
}
