// Draft, differ 6 of 175, skeleton gap 3 (tla-en, 2026-11-01)
// Twin of The Broken Seal's PushedActor_AdvancePair (scene/shared/staged_actor.h):
// the leader pushes the actor in front of it one cell, same calls, offsets,
// animation 8, 15 frames, sound 185 and rate 65536 / 5. Actor_FindInCell is the
// twin's PushedActor_FindAtTile.
// Left: allocation and everything else match; only the scheduling of three
// instructions after Func_02003bdc differs: the native loads dst.z before
// next->velocityX's store, and stores next->velocityZ and copies lead to r2
// around the 0x80000000 build. The twin's store order gave 8; velocityX beside
// x gives 6.
// Tried: all 720 orders of next's four stores with lead's two target stores
// (best 6), dst as s32[3] and as a union array, a local for the rise byte and
// for zero (all 6), dst.x and dst.z through locals (10), a chained zero (8).
#include "actor.h"
#include "system/main_routine_work.h"
#include "types.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gData_02003d10[];
struct Actor *Actor_GetById(s32 id);
struct Actor *Actor_FindInCell(struct FieldPosition *position,
                               struct Actor *self);
s32 Func_02003aac(struct Actor *actor, struct FieldPosition *position);
void Func_02003a54(struct Actor *actor, s32 animation);
void wait(s32 frames);
void Func_02003c54(s32 sound);
void Func_02003a84(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_02003a8c(struct Actor *actor);
void Func_02003bdc(void);

void Func_020003b4(void) {
  struct FieldPosition dst;
  struct Actor *lead;
  struct Actor *next;
  struct Actor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Actor_GetById(gMainRoutineWork.currentOwner);
  dir = lead->angle >> 12;
  step = gData_02003d10[dir];
  dst.x = lead->x + (step & 0xffff0000);
  dst.y = lead->y;
  step <<= 16;
  dst.z = lead->z + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gData_02003d10[dir];
  dst.x = next->x + (step & 0xffff0000);
  dst.y = next->y;
  step <<= 16;
  dst.z = next->z + step;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x;
  dst.y = next->y + 0x100000;
  dst.z = next->z;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->terrainId = 2;
  step = gData_02003d10[dir];
  dst.x = next->x + (step & 0xffff0000);
  dst.y = next->y;
  step <<= 16;
  dst.z = next->z + step;
  if (Func_02003aac(next, &dst) > 0)
    return;

  if (((u8 *)next)[98] != 0)
    return;

  Func_02003a54(lead, 8);
  rate = 0x10000 / 5;
  wait(15);
  Func_02003c54(185);
  next->speedLimit = rate;
  next->acceleration = rate;
  Func_02003a84(next, dst.x, dst.y, dst.z);
  lead->speedLimit = rate;
  lead->acceleration = rate;
  Func_02003a84(lead, dst.x, dst.y, dst.z);
  Func_02003a8c(next);
  Func_02003bdc();
  next->x = dst.x;
  next->velocityX = 0;
  next->z = dst.z;
  next->velocityZ = 0;
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x = ((s16 *)&lead->x)[1] << 16;
  lead->z = ((s16 *)&lead->z)[1] << 16;
  Func_02003a54(lead, 1);
}
