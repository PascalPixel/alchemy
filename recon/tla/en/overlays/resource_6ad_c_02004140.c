// Draft, differ 234 of 304, skeleton gap 88 (tla-en, 2026-11-01)
// Control flow, types and calls follow the listing: every block, the restart
// after a blocked step, the rotated walk and the 64-object scan are in place.
// Left: GCSE's PRE hoists actor + 34 and the address of pos over the restart
// label onto the entry edge, which the listing does not ("PRE/HOIST: edge
// (0,1)" in the gcse dump). The listing recomputes both after the label and
// builds each &pos argument afresh, so nothing shared it. The hoisted pseudos
// take a high register, which pushes the listing's r6/r10 pointers, the
// stack-held radius and the high-register scan counter into other homes; most
// of the skeleton gap is that. Tried without gain: for (;;) and do/while for
// the restart (55/58, worse), a loop test on a constant local (no change to
// the hoist, and a device), terrainId through a pointer local, pos read without
// a pointer, scan locals in an inner block, an explicit flags pointer, s32
// direction. PRE only stops if the expressions are not anticipated at the
// label or the back edge disappears; no plain form found for either.
#include "types.h"
#include "actor.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

struct Position {
  s32 x, y, z;
};

s32 ArcTan2(s32, s32);
void Func_0200410c(void);
void Func_02005480(struct Actor *, s32);
void Func_020054b8(struct Actor *, s32, s32, s32);
void Func_020054c0(struct Actor *);
s32 Func_020054d0(s32, s32, s32);
s32 Func_02005538(s32 *, s32, struct Position *, s32);
s32 Map_GetTerrainHeight(s32, s32, s32);
void Actor_SetAnimationSpeed(struct Actor *, s32);
void Vector_AddPolarOffsetFar(s32, s32, struct Position *);
void wait(s32);
void Func_02004080(void);
extern s16 gData_02005f38[];

void Func_02004140(void) {
  struct Actor *actor;
  struct Position pos;
  struct Position *cursor;
  struct Actor *other;
  s32 radius;
  s32 centerX, centerZ, lastX, lastZ;
  s32 startCell, cell, i, dz;
  s16 direction;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
restart:
  Func_0200410c();
  actor->speedLimit = 0x10000;
  actor->acceleration = 0x8000;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  cursor = &pos;
  centerX = (actor->x & 0xfff00000) + 0x80000;
  cursor->x = centerX;
  cursor->y = actor->y;
  centerZ = (actor->z & 0xfff00000) + 0x80000;
  cursor->z = centerZ;
  lastZ = centerZ;
  lastX = centerX;
  startCell = Func_020054d0(actor->terrainId, lastX, lastZ);
  {
    s32 dx = lastX - actor->x;
    dz = lastZ - actor->z;
    if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
      direction = gData_02005f38[(gInput.held >> 4) & 15];
      if ((u16)direction == 0xffff)
        return;
    } else {
      direction = ArcTan2(lastZ - actor->z, lastX - actor->x);
      direction = (direction + 0x2000) & 0xffffc000;
    }
  }
  Vector_AddPolarOffsetFar(0x100000, direction, &pos);
  cell = Func_020054d0(actor->terrainId, cursor->x, cursor->z);
  if (cell != 255 &&
      Map_GetTerrainHeight(actor->terrainId, cursor->x, cursor->z) - actor->y <=
          0x80000) {
    cursor->x = centerX;
    cursor->z = centerZ;
    actor->speedLimit = 0x20000;
    actor->acceleration = 0x1999;
    actor->action = 0;
    Func_020054b8(actor, centerX, actor->y, centerZ);
    Func_02005480(actor, 2);
    Actor_SetAnimationSpeed(actor, 48);
    Func_020054c0(actor);
    actor->update = Func_02004080;
  } else {
    actor->angle = direction;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = centerX;
    actor->z = centerZ;
    goto stop;
  }
  while (Vector_AddPolarOffsetFar(0x100000, direction, &pos),
         cursor = &pos,
         (cell = Func_020054d0(actor->terrainId, cursor->x, cursor->z)) != 255) {
    if (Map_GetTerrainHeight(actor->terrainId, cursor->x, cursor->z) - actor->y >
        0x80000)
      goto blocked;
    radius = actor->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    for (i = 0; i <= 63; i++, other++) {
      if (other->script != 0 && (other->collisionFlags & 1) &&
          other != actor &&
          Func_02005538(&other->x, radius, &pos, other->radius - 2) >= 0)
        goto blocked;
    }
    lastX = cursor->x;
    lastZ = cursor->z;
    actor->speedLimit = 0x20000;
    actor->acceleration = 0x1999;
    Func_020054b8(actor, cursor->x, cursor->y, cursor->z);
    Func_020054c0(actor);
    if (cell != startCell) {
      wait(10);
      goto stop;
    }
  }
blocked:
  actor->speedLimit = 0x20000;
  actor->acceleration = 0x10000;
  Func_020054b8(actor, lastX, actor->y, lastZ);
  Func_020054c0(actor);
  wait(2);
  goto restart;
stop:
  actor->update = 0;
  actor->actionFlags |= 1;
  actor->acceleration = 0x4000;
  Func_02005480(actor, 1);
}
