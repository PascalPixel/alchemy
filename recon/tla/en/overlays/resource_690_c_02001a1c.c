// Draft, differ 237 of 347, skeleton gap 45 removed, 48 added (tla-en, 2026-11-01)
// Written from the listing (no twin: the same retry, step and 64-object scan as
// Func_02001770, Func_02001d3c, 6ad 02004140 and 68f 02001b48, which stand at
// gap 88 to 97 or are unmeasured). Calls, branches, loops and frame agree.
// Left: GCSE's PRE ("PRE/HOIST: edge (0,1)" in the gcse dump) lifts sfp - 12
// (&probe) and object + 34 (&terrainId) over the retry label. The listing has
// neither hoisted: it rebuilds &probe at every use, even where r10 or r6 holds
// it, and sets [sp, #4] after the label. The hoisted pseudos take high
// registers, so target (r10, then r6 in the step loop) and the scan's flags
// (r8) lose their homes; the rest of the gap is that, plus reload following it.
// Both are anticipated at the label in any spelling: the first probe store
// needs sfp - 12 in a register and the first terrainId read needs object + 34,
// both in the head block, so moving target and layer past the heading exit
// changed nothing (dump checked). Why the native escapes PRE is a compiler
// question, out of scope for a draft.
// Measured lead, not kept: object = actor copied at the label stops the
// object + 34 hoist: differ 229, gap 41/42. It is a forced temporary (S2) and
// does not reach differ 0, so it came back out.
// gData_02002ce8 measures best as volatile s16 (reloaded at each test, ldrh with
// lsls 16; volatile u16 51/55, plain s16 57/64), an inference from the code, so
// this draft has its own includes: local.h says plain u16. Adopting needs that declaration
// changed and S2's two-unrelated-functions test for a volatile global checked.
// The step loop's while, a goto second wait loop, one target pointer and a
// separate scan counter each closed part of the gap.
// Tried, no gain: goto for the step loop (49/55); target split in two as the
// listing's registers suggest (45/50, differ 267); no pointer (57/60);
// u16 heading (62/67); heading == -1 (47/50); terrainId direct, a flags
// pointer, for-step other++, inner scopes for dx and dz, the sum in the compare,
// s32 heading, a function-pointer local for the multiply: all unchanged;
// x and z declared first (247); indexing other from slot (252).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct SettlementsProbe {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gInput;
extern volatile s16 gData_02002ce8;
void wait(s32 frames);
void Func_0200173c(void);
void Func_02001fe0(struct Actor *object, s32 a);
extern s16 gData_020022b6[];
void Func_020016b0(struct Actor *object);
s32 ArcTan2(s32 y, s32 x);
void Vector_AddPolarOffsetFar(s32 distance, s32 heading,
                              struct SettlementsProbe *probe);
s32 Func_02002028(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002008(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002010(struct Actor *object);
s32 Func_02002048(struct Actor *object);
s32 Func_02002050(s32 *a, s32 radiusA, struct SettlementsProbe *b, s32 radiusB);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void Func_02001a1c(void) {
  struct Actor *object;
  struct SettlementsProbe probe;
  s32 startCell;
  s32 lastX;
  s32 lastZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  struct SettlementsProbe *target;
  s16 heading;
  s32 nextCell;
  s32 i;
  s32 slot;
  s32 distance;
  s32 dx;
  s32 dz;
  struct Actor *other;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02002ce8 = 0;
again:
  Func_0200173c();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  target = &probe;
  x = (object->x & 0xfff00000) + 0x80000;
  target->x = x;
  target->y = object->y;
  z = (object->z & 0xfff00000) + 0x80000;
  target->z = z;
  lastZ = z;
  lastX = x;
  layer = &object->terrainId;
  startCell = Func_02002028(*layer, lastX, lastZ);
  dx = lastX - object->x;
  dz = lastZ - object->z;
  distance = Iwram_MulQ16(dx, dx);
  distance += Iwram_MulQ16(dz, dz);
  if (distance < 0x40000) {
    heading = gData_020022b6[(gInput >> 4) & 15];
    if ((u16)heading == 0xffff)
      goto end;
  } else {
    heading = ArcTan2(lastZ - object->z, lastX - object->x);
    heading = (heading + 0x2000) & 0xc000;
  }
  Vector_AddPolarOffsetFar(0x100000, heading, &probe);
  nextCell = Func_02002028(*layer, target->x, target->z);
  if (nextCell != 255 &&
      Map_GetTerrainHeight(*layer, target->x, target->z) - object->y <=
          0x80000) {
    target->x = x;
    target->z = z;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    object->action = 0;
    Func_02002008(object, x, object->y, z);
    Func_02001fe0(object, 2);
    Actor_SetAnimationSpeed(object, 48);
  } else {
    object->angle = heading;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  for (i = 0; i <= 179 && !Func_02002048(object); i++) {
    if (gData_02002ce8 != 0)
      goto end;
    wait(1);
  }
  object->update = (void (*)(void))Func_020016b0;
  while (Vector_AddPolarOffsetFar(0x100000, heading, &probe), nextCell = Func_02002028(*layer, target->x, target->z), nextCell != 255) {
    if (Map_GetTerrainHeight(*layer, target->x, target->z) - object->y > 0x80000)
      goto stopped;
    if (gData_02002ce8 != 0)
      goto end;
    radius = object->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    for (slot = 0; slot <= 63; slot++) {
      if (other->script != 0 && (other->collisionFlags & 1) && other != object &&
          Func_02002050(&other->x, radius, &probe, other->radius - 2) >= 0)
        goto stopped;
      other++;
    }
    lastX = target->x;
    lastZ = target->z;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    Func_02002008(object, target->x, target->y, target->z);
    i = 0;
    goto walking;
  walkLoop:
    wait(1);
    i++;
  walking:
    if (i <= 179 && !Func_02002048(object) && gData_02002ce8 == 0)
      goto walkLoop;
    if (gData_02002ce8 != 0)
      goto end;
    if (nextCell != startCell) {
      wait(10);
      goto end;
    }
  }
stopped:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02002008(object, lastX, object->y, lastZ);
  Func_02002010(object);
  wait(2);
  goto again;
end:
  object->update = 0;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02001fe0(object, 1);
}
