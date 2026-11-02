// Draft, differ 211 of 304, skeleton gap 43 (tla-en, 2026-11-01)
// Calls, branches, loops and fields follow the native and the adopted relative
// Func_02001b48 (tundaria_tower/s1b48.c), whose plain statement order and goto
// loops this text now takes. Left: GCSE PRE hoists &position and
// &object->terrainId above the retry label and turns every later &position into
// a copy of one register (07.gcse: edge 0 to 1, expressions 3 and 11). The
// native recomputes add sp, #28 after every call and computes &terrainId on each
// pass. The rest of the gap is allocation that follows from the hoisted register.
// Func_02001b48 reaches the native only with tagged devices this draft may not
// add: an empty asm clobbering sfp at each loop stops the hoist, and j holds the
// position's address and then the overlap counter (the native's r10 in both).
// Ruled out as an allocation effect: PRE's reaching register has no REG_EQUIV
// note (17.lreg, the set on edge 0 to 1), so losing its hard register would give
// a stack slot, never the native's rematerialized add sp, #28. All five sites
// reach gcse as sets of a pseudo and hash to one expression, so no plain spelling
// separates them.
// Compiler question for Pascal (K3), not pursued here: gcse.c does not count a
// call as setting sfp, and every native add sp, #28 follows a call since the
// last one, as if the native's did. That would not explain &terrainId. An
// analysis-only compile with -Os left 32 of 302 lines differing, before this
// round's reordering, against 177 with -fno-gcse.
// Tried on this text, no gain: declaration order of j, flags, q, p, entry,
// layer and cell (identical); a set-once local for Iwram_MulQ16 (identical);
// s32 angle (differ 209, gap 47); (s16) casts on it (identical); p for q,
// position for p or q, q as the overlap argument (identical or worse); one pair
// of targets (218); dx before dz (worse); for (;;) in place of goto retry (245).
// Earlier: a head-tested loop flag (gap 117), angle == -1, targets from p[].
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"

extern s16 gData_020032b2[];
void Func_020017dc(void);
void Func_02001868(void);
s32 Func_02002fe0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02002fb0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002fb8(struct Actor *object);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_0200189c(void) {
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
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = Func_02002fe0(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = Func_02002fe0(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->velocityX = 0;
    object->velocityZ = 0;
    object->angle = angle;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->action = 0;
  Func_02002fb0(object, x, object->y, z);
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Func_02002fb8(object);
  object->update = Func_020017dc;
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
    if (Func_02003028(&entry->x, radius, position, entry->radius - 2) >= 0)
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
  Func_02002fb0(object, q[0], q[1], q[2]);
  Func_02002fb8(object);
  if (cell != startCell)
    goto moved;
walk:
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  q = position;
  cell = Func_02002fe0(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
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
