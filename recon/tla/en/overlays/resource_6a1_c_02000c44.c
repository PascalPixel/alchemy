// Draft, differ 113 of 171 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Overlay 6a1's copy of the function drafted in resource_69c_c_02000dcc.c, whose
// header says what it does. How far a pushed object slides.
// Left: gcse's PRE still treats sfp - 12 (position) as one expression across the
// loops, one long pseudo (reg 174, r6 here; the box's base is a second, r8) that
// takes a callee-saved register. The native rebuilds it per row (r5), for the
// call (r1) and after the row (r2), and stores [sp,#32] and [sp,#36] directly.
// Everything else follows: the native has object r4, move r6, row r8, col r7,
// dx r9, dz r10, count r11 and spills direction, width, height, layer and the
// per-pass direction * 4 (sp+4), read back from sp+20; here direction * 4 is
// lifted to the top and dx, dz are spilled.
// box is memory: stepIndex's address escapes, so height and width reload at each
// loop test as the native's [sp,#12] and [sp,#16] do. The grouping is a measured
// form, not a recovered type, and the native addresses them from sp directly,
// with no base. Compiler lead, out of scope here (K1 to K3): the native behaves
// as if a call killed sfp in gcse; alchemy-gcc's gcse.c exempts it. An
// analysis-only compile without gcse rebuilt the address (69c's header).
// Tried 1 November, differ/skeleton: 69c's form 213/38 (start 219/46); box with
// direction too, per-pass offset local 207/34, without it 224/34, with the loops
// reading move->direction 224/30; this box, loops reading move->direction 229/30
// (the skeleton's best, differ's worst); a per-pass offset here (no change);
// stepIndex first in box 206/40; one struct with position too 228/40; struct
// without stepIndex 228/37; split dx and dz from one load 218/32; declaration
// orders (no change). (void)&local on direction, width and height gave 188/40
// through ADDRESSOF: a taken-address device, so not kept.
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
s32 Func_020021b8(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))
s32 Func_02000c44(struct PushMove *move) {
  struct Actor *object;
  s32 position[3];
  struct { s32 height; s32 width; s32 stepIndex; } box;
  s32 direction;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  object = Func_02000dac(&box.stepIndex, &move->unk10, &move->direction);
  if (object == 0)
    return 0;
  direction = move->direction;
  box.width = (ABS(gData_020022dc[direction][0]) +
           ABS(gData_020022dc[direction][2])) >> 4;
  box.height = (ABS(gData_020022dc[direction][1]) +
            ABS(gData_020022dc[direction][3])) >> 4;
  dx = gData_020022f4[box.stepIndex] & 0xffff0000;
  dz = gData_020022f4[box.stepIndex] << 16;
  /* FAKEMATCH(barrier): the empty asm on dz changes its allocation rank; as no
   * asm, 130 of 171 instructions differ in tla-en. */
  asm("" : "=r"(dz) : "0"(dz));
  move->unk14 = 0;
  count = 0;
  layer = &object->terrainId;
  *layer = 2;
  move->x = object->x + dx;
  move->z = object->z + dz;
  position[1] = object->y;
step:
  position[2] = move->z + (gData_020022dc[direction][1] << 16);
  row = 0;
  /* FAKEMATCH(barrier): the empty asm on row changes its allocation rank; as no
   * asm, 124 of 171 instructions differ in tla-en. */
  asm("" : "=r"(row) : "0"(row));
  if (row < box.height) {
  nextRow:

    position[0] = move->x + (gData_020022dc[direction][0] << 16);
    col = 0;
    if (col < box.width) {
    nextCol:
      if (Func_020021b8(object, position) == 2)
        goto blocked;
      position[0] += 0x100000;
      col++;
      if (col < box.width)
        goto nextCol;
    }
    position[2] += 0x100000;
    row++;
    if (row < box.height)
      goto nextRow;
  }
  move->x += dx;
  count++;
  move->z += dz;
  goto step;
blocked:
  *layer = 0;
  if (count == 0)
    return 0;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 115 of 171 instructions differ
   * in tla-en. */
  do {
    move->x = object->x + dx * count;
  } while (0);
  move->y = object->y;
  move->z = object->z + dz * count;
  return 1;
}
