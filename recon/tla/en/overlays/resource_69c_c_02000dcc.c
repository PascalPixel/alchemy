// Differ 2 of 171 (tla-en, 2026-11-08, -mtune=gba), under -Os. How far a pushed actor
// slides: from the actor Func_02000f34 finds, steps its footprint one step at a
// time in the push direction until a cell of it is blocked (Func_02002338
// returns 2), and leaves the stopping point in move; returns 0 when it cannot
// move at all. Shared with overlay 6a1 and others. Left: the load of actor->x
// lands ahead of the mov r1, r9 copy of dx instead of after it. History: 6 to 2
// (2026-11-07) with an inline call wrapper (fresh add r1, sp, #28) and a
// once-only block around the x load.
#include "types.h"
#include "call.h"
#include "actor.h"

struct PushMove {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 direction;
  s32 unk10;
  s32 unk14;
};

extern s8 gData_02002484[][4];
extern s32 gData_0200249c[];

struct Actor *Func_02000f34(s32 *step, s32 *a, s32 *direction);
s32 Func_02002338(struct Actor *actor, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_02002338_Inline(struct Actor *actor, s32 *position) {
  return Func_02002338(actor, position);
}

s32 Func_02000dcc(struct PushMove *move) {
  struct Actor *actor;
  s32 stepIndex;
  s32 position[3];
  s32 direction;
  s32 *p;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  actor = Func_02000f34(&stepIndex, &move->unk10, p = (s32 *)&move->direction);
  if (actor != 0) {
    direction = *p;
    width = (ABS(gData_02002484[direction][0]) +
             ABS(gData_02002484[direction][2])) >>
            4;
    height = (ABS(gData_02002484[direction][1]) +
              ABS(gData_02002484[direction][3])) >>
             4;
    dx = gData_0200249c[stepIndex] & 0xffff0000;
    dz = gData_0200249c[stepIndex] << 16;
    move->unk14 = 0;
    count = 0;
    layer = &actor->terrainId;
    *layer = 2;
    move->x = actor->x + dx;
    move->z = actor->z + dz;
    position[1] = actor->y;
    for (;;) {
      position[2] = move->z + (gData_02002484[direction][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = move->x + (gData_02002484[direction][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_02002338_Inline; as
           * Func_02002338(actor, position), 4 of 171 instructions differ in
           * tla-en. */
          if (Func_02002338_Inline(actor, position) == 2)
            goto blocked;
          position[0] += 0x100000;
        }
        position[2] += 0x100000;
      }
      move->x += dx;
      count++;
      move->z += dz;
    }
  blocked:
    *layer = 0;
    if (count == 0)
      return 0;
    {
      s32 x;
      // A once-only block: keeps the load of actor->x ahead of the multiply.
      do {
        x = actor->x;
      } while (0);
      move->x = x + dx * count;
    }
    move->y = actor->y;
    move->z = actor->z + dz * count;
    return 1;
  }
  return 0;
}
