// Draft, differ 224 of 304, skeleton gap 42/46 (tla-en, 2026-11-01): the
// shape is whole (loops, branches, tails, every call and field). A two-stage
// step: rounds the leader to its cell's centre, then, while the keys or the
// distance give an angle, slides from cell to cell until blocked.
// Left, one cause: GCSE keeps position's address (sfp - 12) in one pseudo,
// 3 substitutions in the gcse dump. The native rebuilds it at all four sites
// (add r2, sp, #28 at both Vector_AddPolarOffsetFar calls and at
// Func_0200428c, add r6, sp, #28 in the loop) with p in r10 ending before the
// loop, so i takes r10, q r6, angle r9 and cell r11; startX and startZ spill
// to sp+12 and sp+8. Here that pseudo holds a high register throughout and the
// rest rotate. Same family as the landing bursts; K3's question, not this
// draft's: no plain form keeps a fully redundant frame address.
// Measured: the outer goto loop needs entry at its bottom test to keep
// &actor->terrainId and p in the body. A set-once `running = 1` with
// `goto again; top: ... again: if (running) goto top;` does that (PRE sees an
// exit there, the last cprop then folds the test), and gives differ 216, gap 37/39, the best
// measured; the address stays a full redundancy from p, so no match and the
// device stayed out. Keep it for when the address is solved.
// Also tried: indexing other[i] (gap 47/56; the stepped pointer is the
// native's r5), Iwram_MulQ16 with no local (worse), s16 angle, no q, a dx
// local, cell and i declared first (all no change), a zero index behind a
// one-pass block at each site to rebuild the address (373, frame 48).
#include "types.h"
#include "actor.h"
#include "call.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

s32 ArcTan2(s32 z, s32 x);
void Func_02002d90(void);
void Func_020041cc(struct Actor *actor, s32 mode);
void Func_02004204(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_0200420c(struct Actor *actor);
s32 Func_02004224(s32 terrainId, s32 x, s32 z);
s32 Func_0200428c(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Map_GetTerrainHeight(s32 terrainId, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
void wait(s32 frames);
void Func_02002d04(void);
extern s16 gData_0200483c[];

void Func_02002dc4(void) {
  s32 position[3];
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 startX;
  s32 startZ;
  u8 *terrainId;
  s32 radius;
  struct Actor *actor;
  struct Actor *other;
  s32 *p;
  s32 *q;
  s32 (*mul)(s32 a, s32 b);
  s32 dz;
  s32 distance;
  s32 angle;
  s32 cell;
  s32 i;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
again:
  Func_02002d90();
  actor->speedLimit = 0x10000;
  actor->acceleration = 0x8000;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  p = position;
  p[0] = startX = (actor->x & 0xfff00000) + 0x80000;
  p[1] = actor->y;
  p[2] = startZ = (actor->z & 0xfff00000) + 0x80000;
  targetZ = startZ;
  targetX = startX;
  terrainId = &actor->terrainId;
  startCell = Func_02004224(*terrainId, targetX, targetZ);
  mul = Iwram_MulQ16;
  dz = targetZ - actor->z;
  {
    s32 dx = targetX - actor->x;
    distance = mul(dx, dx);
  }
  distance += mul(dz, dz);
  if (distance < 0x40000) {
    angle = gData_0200483c[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = (s16)ArcTan2(targetZ - actor->z, targetX - actor->x);
    angle = (s16)((angle + 0x2000) & 0xffffc000);
  }
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = Func_02004224(*terrainId, p[0], p[2]);
  if (cell != 255 &&
      Map_GetTerrainHeight(*terrainId, p[0], p[2]) - actor->y <= 0x80000) {
    p[0] = startX;
    p[2] = startZ;
    actor->speedLimit = 0x20000;
    actor->acceleration = 0x1999;
    actor->action = 0;
    Func_02004204(actor, startX, actor->y, startZ);
    Func_020041cc(actor, 2);
    Actor_SetAnimationSpeed(actor, 48);
    Func_0200420c(actor);
    actor->update = Func_02002d04;
  } else {
    actor->angle = angle;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto finish;
  }
  goto test;
body:
  if (Map_GetTerrainHeight(*terrainId, q[0], q[2]) - actor->y > 0x80000)
    goto blocked;
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  for (i = 0; i <= 63; i++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != actor) {
      if (Func_0200428c(&other->x, radius, position, other->radius - 2) >= 0)
        goto blocked;
    }
    other++;
  }
  targetX = q[0];
  targetZ = q[2];
  actor->speedLimit = 0x20000;
  actor->acceleration = 0x1999;
  Func_02004204(actor, q[0], q[1], q[2]);
  Func_0200420c(actor);
  if (cell != startCell) {
    goto stop;
  }
test:
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  q = position;
  cell = Func_02004224(*terrainId, q[0], q[2]);
  if (cell != 255)
    goto body;
blocked:
  actor->speedLimit = 0x20000;
  actor->acceleration = 0x10000;
  Func_02004204(actor, targetX, actor->y, targetZ);
  Func_0200420c(actor);
  wait(2);
  goto again;
stop:
  wait(10);
finish:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = 0x4000;
  Func_020041cc(actor, 1);
}
