// Matches (tla-en, 2026-11-08, -mtune=gba; the same in tla-ja, -de, -es, -fr and -it).
// Func_02000c44 of overlay 6a1: overlay 69c's Func_02000dcc with this overlay's
// symbols. 6 to 2 by that draft's devices; to 0 under -mtune=gba with actor->x
// and move->direction read plainly (its once-only block and pointer temporary
// no longer pay). The wrapper still does: as a plain call, 2 of 171.
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

extern s8 gData_020022dc[][4];
extern s32 gData_020022f4[];

struct Actor *Func_02000dac(s32 *step, s32 *a, s32 *direction);
s32 Func_020021b8(struct Actor *actor, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_020021b8_Inline(struct Actor *actor, s32 *position) {
  return Func_020021b8(actor, position);
}

s32 Func_02000c44(struct PushMove *move) {
  struct Actor *actor;
  s32 stepIndex;
  s32 position[3];
  s32 direction;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  actor = Func_02000dac(&stepIndex, &move->unk10, &move->direction);
  if (actor != 0) {
    direction = move->direction;
    width = (ABS(gData_020022dc[direction][0]) +
             ABS(gData_020022dc[direction][2])) >>
            4;
    height = (ABS(gData_020022dc[direction][1]) +
              ABS(gData_020022dc[direction][3])) >>
             4;
    dx = gData_020022f4[stepIndex] & 0xffff0000;
    dz = gData_020022f4[stepIndex] << 16;
    move->unk14 = 0;
    count = 0;
    layer = &actor->terrainId;
    *layer = 2;
    move->x = actor->x + dx;
    move->z = actor->z + dz;
    position[1] = actor->y;
    for (;;) {
      position[2] = move->z + (gData_020022dc[direction][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = move->x + (gData_020022dc[direction][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_020021b8_Inline; as
           * Func_020021b8(actor, position), 2 of 171 instructions differ in
           * tla-en. */
          if (Func_020021b8_Inline(actor, position) == 2)
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
    move->x = actor->x + dx * count;
    move->y = actor->y;
    move->z = actor->z + dz * count;
    return 1;
  }
  return 0;
}
