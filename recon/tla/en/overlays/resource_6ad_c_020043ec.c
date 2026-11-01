// Draft, differ 258 of 347, skeleton gap 94 (tla-en, 2026-11-01)
// Control flow, types and frame (40 bytes) agree with the listing. What is left
// starts at the outer retry loop: gcse hoists actor + 34 and pos's address onto
// the edge into `start` (07.gcse: PRE/HOIST edge (0,1)), where the listing
// computes them inside the loop and rematerialises `add rN, sp, #28`. That takes
// r8 from the multiply's address, spills it, and moves the literal pools; the
// other differences follow from it. Tried: for(;;) against goto loops, a kept
// terrain pointer, separate p and q (worse), merged counters (worse), declaration
// orders. gData_020074c8 is volatile s16: it gives the ldrh, lsls, cmp reads.
#include "types.h"
#include "actor.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 ArcTan2(s32, s32);
void Func_0200410c(void);
void Func_02005480(struct Actor *, s32);
void Func_020054b8(struct Actor *, s32, s32, s32);
void Func_020054c0(struct Actor *);
s32 Func_020054d0(s32, s32, s32);
s32 Func_02005530(struct Actor *);
s32 Func_02005538(s32 *, s32, s32 *, s32);
s32 Map_GetTerrainHeight(s32, s32, s32);
void Actor_SetAnimationSpeed(struct Actor *, s32);
void Vector_AddPolarOffsetFar(s32, s32, s32 *);
void wait(s32);
void Func_02004080(void);
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));
extern const s16 gData_02005f38[];
extern volatile s16 gData_020074c8;
extern u32 gInput;
extern u8 gMainRoutineWork[];

void Func_020043ec(void) {
  struct Actor *actor;
  s32 pos[3];
  s32 *p;
  s32 cellX, cellZ, x, z, startCell, cell, i, j, dx, dz, dist, radius;
  s32 angle;
  struct Actor *other;

  actor = ObjectTable_Get(FIELD_AT(gMainRoutineWork, s32, 532));
  gData_020074c8 = 0;
start:
    Func_0200410c();
    actor->speedLimit = 0x10000;
    actor->acceleration = 0x8000;
    actor->targetX = 0x80000000;
    actor->targetZ = 0x80000000;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    p = pos;
    cellX = (actor->x & 0xfff00000) + 0x80000;
    p[0] = cellX;
    p[1] = actor->y;
    cellZ = (actor->z & 0xfff00000) + 0x80000;
    p[2] = cellZ;
    x = cellX;
    z = cellZ;
    startCell = Func_020054d0(actor->terrainId, x, z);
    dx = x - actor->x;
    dz = z - actor->z;
    dist = IwramMulQ16(dx, dx);
    dist += IwramMulQ16(dz, dz);
    if (dist >= 0x40000)
      goto far;
    angle = gData_02005f38[(gInput >> 4) & 15];
    if ((u16)angle == 0xffff)
      goto done;
    goto move;
  far:
    angle = (s16)ArcTan2(z - actor->z, x - actor->x);
    angle = (s16)((angle + 0x2000) & -0x4000);
  move:
    Vector_AddPolarOffsetFar(0x100000, angle, pos);
    cell = Func_020054d0(actor->terrainId, p[0], p[2]);
    if (cell != 255 &&
        Map_GetTerrainHeight(actor->terrainId, p[0], p[2]) - actor->y <= 0x80000) {
      p[0] = cellX;
      p[2] = cellZ;
      actor->speedLimit = 0x20000;
      actor->acceleration = 0x1999;
      actor->action = 0;
      Func_020054b8(actor, cellX, actor->y, cellZ);
      Func_02005480(actor, 2);
      Actor_SetAnimationSpeed(actor, 48);
      i = 0;
    } else {
      actor->angle = angle;
      actor->velocityX = 0;
      actor->velocityZ = 0;
      actor->x = cellX;
      actor->z = cellZ;
      goto done;
    }
    while (i <= 179 && Func_02005530(actor) == 0) {
      if (gData_020074c8 != 0)
        goto done;
      wait(1);
      i++;
    }
    actor->update = Func_02004080;
    goto test;
  body:
      if (Map_GetTerrainHeight(actor->terrainId, p[0], p[2]) - actor->y > 0x80000)
        goto blocked;
      if (gData_020074c8 != 0)
        goto done;
      radius = actor->radius - 2;
      other = FIELD_AT(0x3000000, struct Actor *, 20);
      for (j = 0; j <= 63; j++, other++) {
        if (other->script != 0 && (other->collisionFlags & 1) && other != actor) {
          if (Func_02005538(&other->x, radius, pos, other->radius - 2) >= 0)
            goto blocked;
        }
      }
      x = p[0];
      z = p[2];
      actor->speedLimit = 0x20000;
      actor->acceleration = 0x1999;
      Func_020054b8(actor, p[0], p[1], p[2]);
      i = 0;
      while (i <= 179 && Func_02005530(actor) == 0 && gData_020074c8 == 0) {
        wait(1);
        i++;
      }
      if (gData_020074c8 != 0)
        goto done;
      if (cell != startCell)
        goto settle;
  test:
      Vector_AddPolarOffsetFar(0x100000, angle, pos);
      p = pos;
      cell = Func_020054d0(actor->terrainId, p[0], p[2]);
      if (cell != 255)
        goto body;
  blocked:
    actor->speedLimit = 0x20000;
    actor->acceleration = 0x10000;
    Func_020054b8(actor, x, actor->y, z);
    Func_020054c0(actor);
    wait(2);
    goto start;
settle:
  wait(10);
done:
  actor->update = 0;
  actor->actionFlags |= 1;
  actor->acceleration = 0x4000;
  Func_02005480(actor, 1);
}
