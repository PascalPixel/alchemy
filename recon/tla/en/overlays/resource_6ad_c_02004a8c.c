// Differ 2 of 171 (tla-en, 2026-11-08, -mtune=gba). Func_02004a8c of overlay
// 6ad: overlay 69c's Func_02000dcc with this overlay's symbols (6 to 2); see
// that draft. The forwarding call layer gives the call its own add r1, sp, #28
// and the once-only block sets r0 = 1 in the native's place. Left: the load of
// actor->x lands ahead of the mov r1, r9 copy of dx instead of after it.
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

extern s8 gData_02005f64[][4];
extern s32 gData_02005f7c[];

struct Actor *Func_02004bf4(s32 *step, s32 *a, s32 *direction);
s32 Func_020054f8(struct Actor *actor, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 Func_020054f8_Inline(struct Actor *actor, s32 *position) {
  return Func_020054f8(actor, position);
}

s32 Func_02004a8c(struct PushMove *move) {
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

  actor = Func_02004bf4(&stepIndex, &move->unk10, p = (s32 *)&move->direction);
  if (actor != 0) {
    direction = *p;
    width = (ABS(gData_02005f64[direction][0]) +
             ABS(gData_02005f64[direction][2])) >>
            4;
    height = (ABS(gData_02005f64[direction][1]) +
              ABS(gData_02005f64[direction][3])) >>
             4;
    dx = gData_02005f7c[stepIndex] & 0xffff0000;
    dz = gData_02005f7c[stepIndex] << 16;
    move->unk14 = 0;
    count = 0;
    layer = &actor->terrainId;
    *layer = 2;
    move->x = actor->x + dx;
    move->z = actor->z + dz;
    position[1] = actor->y;
    for (;;) {
      position[2] = move->z + (gData_02005f64[direction][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = move->x + (gData_02005f64[direction][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_020054f8_Inline; as
           * Func_020054f8(actor, position), 4 of 171 instructions differ in
           * tla-en. */
          if (Func_020054f8_Inline(actor, position) == 2)
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
