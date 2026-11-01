// Draft, differ 226 of 304, skeleton gap 88 (tla-en, 2026-11-01)
// Control flow, types and calls follow the listing: every block, the restart
// after a blocked step, the rotated walk and the 64-object scan are in place.
// The locals are in the order of the listing's slots: pos 28, startCell 24,
// lastX 20, lastZ 16, centerX 12, centerZ 8, the terrain address 4, radius 0.
// Left: the listing behaves as if the back edge to restart were absent from gcse
// and flow, and as if PRE never touched pos. Two separate effects, both measured.
// - PRE hoists actor + 34 and &pos over the label ("PRE/HOIST: edge (0,1)") and
//   then deletes every later &pos as fully redundant. The listing builds &pos
//   afresh at each call with r10 still live, and sets a second pointer in r6 at
//   the bottom test; PRE would have removed both, so stopping the hoist alone
//   is not enough.
// - Weight: flow counts the restart loop at depth 1, which is what moves radius,
//   the scan counter and the flags pointer out of the listing's homes.
// Measured probes, none kept: `return` for `goto restart` gives 165, gap 57. A
// never-false once test after the label (folded by gcse's last cprop) stops the
// hoist, and a combine-folded jump from the entry into the loop (always-taken
// form, so combine can fold it) makes it irreducible, depth 0; together they
// also give 165 with the goto kept, but PRE still merges &pos, so they came out
// (a separate step pointer and a terrain local: 187, gap 83). With gcse off as
// well, by a dead setjmp call, separate step and terrain give 114, gap 46, and
// what remains is register homes, so the listing's shape is the one this source
// gets with no PRE and depth 0. That probe is a per-function switch on a pass,
// which K1 forbids; it is not a route for this draft. Whether the compiler
// differs here is a compiler question for Pascal (K3), and 020043ec and 0200470c
// stop at the same retry loop.
// Tried without gain: for (;;) and do/while for the restart (55/58, worse), a
// loop test on a constant local alone, terrainId through a pointer local (92), a
// separate step pointer (92), pos read without a pointer, scan locals in an inner
// block, an explicit flags pointer, s32 direction, i != 64 (92), ArcTan2(dz, dx)
// (92), lastX and lastZ swapped, and other declaration orders (no change), a
// computed goto through a local (95).
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

  s32 startCell, lastX, lastZ, centerX, centerZ;
  s32 radius;
  s32 cell, i, dz;
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
