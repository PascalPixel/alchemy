// Draft, differ 246 of 246, skeleton gap 80 (tla-en, 2026-11-01)
// Calls, branches, loops and fields are the native's; the twin Imil_RunIceSlide
// is confirmed (same probe, same two cell tests, same retry and walk), and this
// function adds the overlap scan over the script objects. The gap is allocation
// that follows from GCSE. PRE hoists &position (sp+20) and &object->terrainId
// onto the edge above the retry label (07.gcse: edge 0 to 1) and replaces the
// native's recomputations (add r6, sp, #20 in the retry block, add r7 and
// add r2, sp, #20 in the walk loop) with copies of one register. So the entry
// gains instructions, &terrainId takes sp+4 instead of r11 and the position
// pointer is one register for the whole function. Compiled with -fno-gcse
// (analysis only) it keeps the native's recomputations and slots, and object
// and the position pointer swap r5 and r6.
// No plain C form found stops this: gcse.c never counts a call as setting sfp,
// and the retry block evaluates both addresses on every path from the label.
// Func_0200189c has the same gap and its header records the compiler question
// (K3) for Pascal, out of scope here; Func_02001b48 reached its native only
// with tagged devices. What should stop the hoist here is not allowed this
// round and was not tried: an empty asm clobbering sfp, a copy of object at the
// top of the loop, or a test that folds only after gcse.
// The overlap loop is goto only because that measures best: with no loop notes
// the flags byte is read from entry each pass, which frees a register and lets
// the position pointer take r7 rather than r8. The native keeps a flags pointer
// in r8, which a for loop gets from loop.c with no hand-kept pointer
// (entry->collisionFlags in a for loop and a hand-stepped u8 * compile the
// same, differ 272, gap 86). Once the hoist is gone, go back to the for loop.
// Tried, no gain: p, q and layer dropped for position[n] and object->terrainId
// (identical); layer or p set above the label (identical on this text); z read
// back after x as the twin does, x and z declared above the targets, q as the
// overlap argument, the walk loop as for (;;) or goto, j++ before entry++, a
// declaration moved (all identical); s32 prototype on Func_02001868
// (identical), on the move and wait calls (gap 82); entry set before radius
// (gap 84); one pair of targets for x, z and targetX, targetZ (gap 94); angle
// by ~0x3fff, -0x4000 or through u16 (identical), in two statements (worse);
// for (;;) in place of goto retry (gap 84, differ 294, 254 instructions);
// break in place of goto end (233).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void Func_020017dc(void);
void Func_02001868(void);
s32 Func_02002fe0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002fb0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002fb8(struct Actor *object);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001e68(void) {
  struct Actor *object;
  s16 angle;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 radius;
  s32 position[3];
  s32 *p;
  s32 x;
  s32 z;
  u8 *layer;
  s32 cell;
  s32 *q;
  struct Actor *entry;
  s32 j;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (object->angle + 0x2000) & 0xc000;
retry:
  Func_02001868();
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
  targetX = x;
  targetZ = z;
  layer = &object->terrainId;
  startCell = Func_02002fe0(*layer, x, z);
  Vector_AddPolarOffsetFar(0x100000, angle, p);
  cell = Func_02002fe0(*layer, p[0], p[2]);
  if (cell != 255 &&
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y <= 0x80000) {
    p[0] = targetX;
    p[2] = targetZ;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    object->action = 0;
    Func_02002f80(object, 2);
    Actor_SetAnimationSpeed(object, 48);
    object->update = Func_020017dc;
  } else {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  while (Vector_AddPolarOffsetFar(0x100000, angle, position), q = position,
         (cell = Func_02002fe0(*layer, q[0], q[2])) != 255) {
    if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
      goto blocked;
    radius = object->radius - 2;
    entry = Ram_HeapSlots->scriptObjects;
    j = 0;
  overlap:
    if (entry->script != NULL && (1 & entry->collisionFlags) &&
        entry != object) {
      if (Func_02003028(&entry->x, radius, position, entry->radius - 2) >= 0)
        goto blocked;
    }
    entry++;
    j++;
    if (j <= 63)
      goto overlap;
    targetX = q[0];
    targetZ = q[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    Func_02002fb0(object, q[0], q[1], q[2]);
    Func_02002fb8(object);
    if (cell != startCell)
      goto moved;
  }
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02002fb0(object, targetX, object->y, targetZ);
  Func_02002fb8(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002f80(object, 1);
}
