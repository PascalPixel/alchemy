// Draft, differ 224 of 304, skeleton gap 42/44 (tla-en, 2026-11-01)
// Written from overlay 68f's Func_0200189c, the same 302-instruction routine:
// from a whole cell, take a step toward the keys' heading (or snap toward the
// cell centre first), walk on one cell at a time while the ground is level and
// none of the 64 objects blocks it, retrying after a blocked step. Calls,
// branches, loops and fields follow the listing. No devices are in this draft.
// Left: GCSE's PRE hoists &position and &object->terrainId over the retry label
// (07.gcse: expressions 3 and 11) and takes every later position address from
// that one pseudo, here r8 for the whole function. The listing builds the
// address afresh after each call (add r2, sp, #28; add r6, sp, #28), sets
// terrainId's address after the label, and so frees its high registers: r8 for
// IwramMulQ16 then flags, r10 for p then the scan counter, r9 angle, r11 cell.
// Here z takes r9, angle r11, cell r10, the scan counter a caller-saved r4 and
// IwramMulQ16 a caller-save slot, sp+4. Every skeleton line left is one of
// these reloads or a schedule that follows it; none is a source shape.
// Why no plain form was found: PRE needs only that an operand stays unchanged
// around the loop, and a frame address has no operand C can set. Reload cannot
// rematerialize the address either: PRE's pseudo has no REG_EQUIV note.
// Tried at the real options, no instruction changed (differ 224): declaration
// order of x and z, angle, cell, the scan locals, radius; s32 angle; a MulQ
// local; a dist local; targetX before targetZ; layer through object each time;
// x and z set apart from their stores; flags as entry + 89; the scan's steps in
// its for header; i < 64; goto forms of the walk, the scan, or both. Worse: q
// apart from p (230, gap 43/47), no p at all (230), p set once (230).
// Measured, not kept: empty asm operands re-setting object at the label stop
// the terrainId hoist; with the two pointer sets too, 211, gap 30/33, but the
// hoisted address then took a stack slot. Such devices are ruled out here.
// A compiler question, out of scope, for Pascal: every &position site in the
// listing follows a call, as if a call killed frame addresses for GCSE, and
// IwramMulQ16's callee-saved home fits refs not weighted by the retry loop's
// depth. Overlays 68f (0200189c), 6ab (02002dc4) and 6ad (02004140) stop at the
// same point, so one answer would reach this family.
#include "call.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"

extern s16 gData_020022b6[];
void Func_020016b0(void);
void Func_0200173c(void);
s32 Func_02002028(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02002008(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02001fe0(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002010(struct Actor *object);
s32 Func_02002050(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001770(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *p;
  s32 *q;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
retry:
  Func_0200173c();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = Func_02002028(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020022b6[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse's hoist of the
   * position's address; as no asm, 2 of 304 instructions differ in tla-en. */
  __asm__("" : : : "sfp");
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = Func_02002028(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->action = 0;
  Func_02002008(object, x, object->y, z);
  Func_02001fe0(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Func_02002010(object);
  object->update = Func_020016b0;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Func_02002050, (s32)&entry->x, radius, (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Func_02002008(object, q[0], q[1], q[2]);
  Func_02002010(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): as the one above; as no asm, 126 of 304 instructions
   * differ in tla-en. */
  __asm__("" : : : "sfp");
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  q = position;
  cell = Func_02002028(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02002008(object, targetX, object->y, targetZ);
  Func_02002010(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02001fe0(object, 1);
}
