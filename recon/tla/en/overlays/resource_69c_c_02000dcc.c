// Draft, differ 208 of 171, skeleton gap 36 (tla-en, 2026-11-01)
// How far a pushed object slides: from the object Func_02000f34 finds, steps
// its footprint one step at a time in the push direction until a cell of it
// is blocked (Func_02002338 returns 2), and leaves the stopping point in move;
// returns 0 when it cannot move at all. Shared with overlay 6a1 and others.
// Left: gcse's PRE. The expressions direction << 2 and sfp - 12 (the frame
// address of position) reach back to the top of the function here, one long
// pseudo each (the frame address has 25 refs and wins a callee-saved
// register). The native computes direction * 4 once per pass at the top of the
// step loop (sp+4, read from the stack copy at sp+20) and rebuilds the frame
// address per row (r5), for the call (r1) and after the row (r2), with direct
// [sp,#32] and [sp,#36] stores. Its registers: move r6, object r4 (saved around
// the cell test), row r8, col r7, dx r9, dz r10, count r11; direction, width,
// height, layer and the index spilled.
// Compiler lead for Pascal, out of scope here (K1 to K3): an analysis-only
// compile of this shape with -fno-gcse, frame 40, reproduces the native's
// three frame-address forms and direct stores exactly, with only the row's
// direction * 4 and dx, dz and count's spilling left. This is the family of
// "frame address rebuilt inside a loop with calls". gcse.c cannot be stopped
// from plain C: sfp is never killed, and LCM always lifts the expression out of
// a loop that computes it on every entry. The loops read move->direction,
// though the native reads its stack copy: that measures best in plain C
// (direction: 219, skeleton 46; this form 213, 38). cellX is real: it gives the
// per-row address and leaves the call's own. Tried 1 November: real for loops
// (243 with for (;;), 235 inside a goto step); a local reassigned to split the
// top's direction from the loops' (223, PRE still lifts it); a volatile
// direction with a per-pass copy (219, skeleton 41; keeps the shift in the
// loop); the same through a taken address (210, skeleton 40) or a per-pass copy
// local (210, skeleton 44), both devices that do not reach 0, so taken back
// out; direction and stepIndex in a frame struct (219); static inline Abs, an
// offset local, const tables and declaration order (no change). Twin: The
// Broken Seal's PushedActor_FindClearPosition (scene/shared/ staged_actor.h,
// matched there), with the roles of its position and probe->position swapped:
// move steps and position probes. Its direction is stepIndex here and its
// probe->footprintIndex is move->direction here, so that field's name is the
// draft's and wants renaming with the struct. Its statements did not close the
// gap: its for (;;) and for loops over a position struct measure 237,
// skeleton 45. Closed 1 November: the early test is inverted, if (object != 0)
// around the whole body with return 0 last, which puts the native's movs r0, #0
// at the end (213 to 208, skeleton 38 to 36). if (count != 0) around the tail
// gives 207 but drops the native's movs r0, #0 before cmp, skeleton 37, so it
// is not kept. Also tried 1 November, with and without that test: a flat s8
// view of the table and the twin's offset = footprint * 4, set at the top, per
// pass or both (PRE folds them, 219, skeleton 46); the loops reading a
// footprint local (212, skeleton 44); offset per pass from move->direction
// (231); a stack struct of height, width, footprint and stepIndex with a
// per-pass offset, which reloads width and height as the native does (208,
// skeleton 32, but 38 added, a base register the native lacks, and the grouping
// is not a recovered type, so taken back out); count * dx, dz before dx and
// declaration orders (no gain).
#include "types.h"
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
s32 Func_02002338(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000dcc(struct PushMove *move) {
  struct Actor *object;
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
  s32 *cellX;

  object = Func_02000f34(&stepIndex, &move->unk10, &move->direction);
  if (object != 0) {
    direction = move->direction;
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
    layer = &object->terrainId;
    *layer = 2;
    move->x = object->x + dx;
    move->z = object->z + dz;
    position[1] = object->y;
  step:
    position[2] = move->z + (gData_02002484[move->direction][1] << 16);
    row = 0;
    if (row < height) {
    nextRow:
      cellX = position;
      *cellX = move->x + (gData_02002484[move->direction][0] << 16);
      col = 0;
      if (col < width) {
      nextCol:
        if (Func_02002338(object, position) == 2)
          goto blocked;
        *cellX += 0x100000;
        col++;
        if (col < width)
          goto nextCol;
      }
      position[2] += 0x100000;
      row++;
      if (row < height)
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
    move->x = object->x + dx * count;
    move->y = object->y;
    move->z = object->z + dz * count;
    return 1;
  }
  return 0;
}
