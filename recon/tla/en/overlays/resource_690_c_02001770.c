// Draft, differ 226 of 304, skeleton gap 88 (tla-en, 2026-11-01)
// Written from overlay 68f's Func_0200189c, the same 302-instruction routine:
// from a whole cell, take a step toward the keys' heading (or snap toward the
// cell centre first), walk on one cell at a time while the ground is level and
// none of the 64 objects blocks it, retrying after a blocked step. Calls,
// branches, loops and fields follow the listing.
// Left: GCSE's PRE hoists &position and &object->terrainId over the retry label
// onto edge (0,1) (07.gcse: expressions 3 and 11) and takes every later
// position address from that one pseudo, a long-lived high register. The
// listing sets both after the label, keeps the address in r10 and r6 only where
// it is used, and builds each position argument afresh with add rN, sp, #28,
// so r10 is free again for the scan counter. Most other differences follow.
// Why no plain form was found: PRE needs only that an operand stays unchanged
// around the loop, and a frame address has no operand C can set; for a function
// this size gcse.c stands aside only if it calls setjmp. Reload cannot
// rematerialize the address either: the pseudo PRE creates has no REG_EQUIV note.
// Measured, not kept: empty asm operands re-setting object at the label do stop
// the terrainId hoist; with the two pointer sets also through them differ was
// 211, gap 30 and 33, but the hoisted address then took a stack slot (sub sp,
// #44), so short of differ 0 the devices came back out.
// An analysis-only compile with -fno-gcse rebuilds the address after the label
// as the listing does (with r8 for r10); only its first blocks were read, and no
// whole-function differ was taken. That is a compiler question (K3) for Pascal,
// out of scope here. Overlays 68f (0200189c) and 6ad (02004140) stop at the same
// point, so one answer would reach this family.
// Tried at the real options: separate p and q (232, gap 92), s32 angle (217, but
// gap 95, so not kept), position[] in either or both parts (232), p or layer set
// before the label (232), layer through object each time (226), scan locals in
// an inner block (232), cell declared first (226).
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
  dx = targetX - object->x;
  dz = targetZ - object->z;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020022b6[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = Func_02002028(*layer, p[0], p[2]);
  if (cell != 255 &&
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y <= 0x80000) {
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
  } else {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  while (Vector_AddPolarOffsetFar(0x100000, angle, position), p = position,
         (cell = Func_02002028(*layer, p[0], p[2])) != 255) {
    if (Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000)
      goto blocked;
    radius = object->radius - 2;
    entry = Ram_HeapSlots->scriptObjects;
    flags = &entry->collisionFlags;
    for (j = 0; j <= 63; j++) {
      if (entry->script != NULL && (1 & *flags) && entry != object) {
        if (Func_02002050(&entry->x, radius, position, entry->radius - 2) >= 0)
          goto blocked;
      }
      flags += sizeof(*entry);
      entry++;
    }
    targetX = p[0];
    targetZ = p[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    Func_02002008(object, p[0], p[1], p[2]);
    Func_02002010(object);
    if (cell != startCell)
      goto moved;
  }
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
