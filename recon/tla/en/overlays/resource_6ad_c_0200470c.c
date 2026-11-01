// Draft, differ 272 of 246, skeleton gap 38 (tla-en, 2026-11-01)
// Twin confirmed: Imil_RunIceSlide, without its key test, and with a collision
// sweep over the script objects. Control flow is the listing's: the outer loop
// is a label, the inner while keeps its test at the bottom. The listing's stack
// is pos sp+20, angle 16, startCell 12, lastX 8, lastZ 4, radius 0.
// What blocks the match is gcse PRE (the .07.gcse dump: redundant insn in bb 1,
// bb 10, 12, 13, inserted on edge (0,1)). It hoists two expressions above the
// label, the listing hoists neither:
// - actor + 34. Solved but not kept: `owner = ObjectTable_Get(...)` above the
//   label and `actor = owner;` as the loop's first line kills the operand. Then
//   terrain sits in r11 in the first block and the stack slots above come out
//   exactly, differ 257 to 262, gap 33. That is a temporary with no match, so
//   it came back out; put it back once the second item is solved.
// - sfp + K (pos). Unsolved. The listing has one pseudo for the first region
//   (r6: add r6, sp, #20) and another for the inner loop (r7), so neither PRE
//   nor the last cprop merged them, though sfp cannot be killed. The hoisted
//   pseudo takes r8 and pushes x, z and actor down one register each. It is
//   not an unallocated equivalence: that would give ldr r1, [sp, #20], which
//   the listing never has.
// Tried with no gain: struct { s32 x, y, z; } for pos (differ 303), for (;;)
// outer loop (hoists more), pos declared in the loop body (295), no pointer
// locals, one p or a separate one per region (same or worse), terrain read as
// actor->terrainId with no local, declaration order fixed to the listing's
// slots without the copy (276, gap 38), p set before the polar call or not at
// all, indexing the table (36/46, worse overall). No device was tried for the
// frame address: a clobber of sfp would stop its elimination.
#include "types.h"
#include "actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
void Func_0200410c();
void Func_02005480();
void Func_020054b8();
void Func_020054c0();
s32 Func_020054d0();
s32 Func_02005538();
s32 Map_GetTerrainHeight();
void Actor_SetAnimationSpeed();
void Vector_AddPolarOffsetFar();
void wait();
extern u8 Func_02004080[];

void Func_0200470c(void) {
  s32 radius, lastZ, lastX, startCell, angle;
  s32 pos[3];
  struct Actor *actor;
  struct Actor *other;
  u8 *terrain;
  s32 *p;
  s32 x, z, i, cell;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (s16)((actor->angle + 0x2000) & -0x4000);
top:
    Func_0200410c();
    actor->speedLimit = 0x10000;
    actor->acceleration = 0x8000;
    actor->targetX = 0x80000000;
    actor->targetZ = 0x80000000;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    x = (actor->x & 0xfff00000) + 0x80000;
    p = pos;
    p[0] = x;
    p[1] = actor->y;
    z = (actor->z & 0xfff00000) + 0x80000;
    p[2] = z;
    lastX = x;
    lastZ = z;
    terrain = &actor->terrainId;
    startCell = Func_020054d0(*terrain, x, z);
    Vector_AddPolarOffsetFar(0x100000, angle, p);
    cell = Func_020054d0(*terrain, p[0], p[2]);
    if (cell != 255 &&
        Map_GetTerrainHeight(*terrain, p[0], p[2]) - actor->y <= 0x80000) {
      p[0] = lastX;
      p[2] = lastZ;
      actor->speedLimit = 0x20000;
      actor->acceleration = 0x1999;
      actor->action = 0;
      Func_02005480(actor, 2);
      Actor_SetAnimationSpeed(actor, 48);
      actor->update = (void (*)(void))Func_02004080;
    } else {
      actor->angle = angle;
      actor->velocityX = 0;
      actor->velocityZ = 0;
      actor->x = x;
      actor->z = z;
      goto done;
    }
    while (Vector_AddPolarOffsetFar(0x100000, angle, pos), p = pos,
           (cell = Func_020054d0(*terrain, p[0], p[2])) != 255) {
      if (Map_GetTerrainHeight(*terrain, p[0], p[2]) - actor->y > 0x80000)
        goto blocked;
      radius = actor->radius - 2;
      other = Ram_HeapSlots->scriptObjects;
      for (i = 0; i <= 63; i++, other++) {
        if (other->script != NULL && (other->collisionFlags & 1) &&
            other != actor) {
          if (Func_02005538(&other->x, radius, pos, other->radius - 2) >= 0)
            goto blocked;
        }
      }
      lastX = p[0];
      lastZ = p[2];
      actor->speedLimit = 0x20000;
      actor->acceleration = 0x1999;
      Func_020054b8(actor, p[0], p[1], p[2]);
      Func_020054c0(actor);
      if (cell != startCell)
        goto arrived;
    }
  blocked:
    actor->speedLimit = 0x20000;
    actor->acceleration = 0x10000;
    Func_020054b8(actor, lastX, actor->y, lastZ);
    Func_020054c0(actor);
    wait(2);
    goto top;
arrived:
  wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = 0x4000;
  Func_02005480(actor, 1);
}
