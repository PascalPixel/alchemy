// Draft, differ 250 of 347, skeleton gap 94 (tla-en, 2026-11-01)
// Control flow, types and the 40-byte frame agree with the listing; the slots
// are in the listing's order (startCell, x, z, cellX, cellZ, then the terrain
// address, then radius). What is left comes from the outer retry loop, in two
// ways, and both are measured:
// - PRE: gcse hoists actor + 34 and pos's address onto the entry edge (07.gcse:
//   PRE/HOIST edge (0,1)). The listing computes actor + 34 after the label, and
//   builds pos at each call with p still live, so nothing unified those.
// - Weight: flow counts the block at depth 1 (12.life), so the multiply address
//   has 6 refs across 1 call and CALLER_SAVE_PROFITABLE puts it in r3 with a
//   save at sp+4. The listing has it in r8 with no save, which fits 3 refs,
//   depth 0: the listing's allocation looks as if the back edge were not there.
// Evidence, none of it kept: `goto start` replaced by `return` gives differ 155,
// with the frame, the slots, r8 and cellZ on the stack all right, so the weight
// is the larger part. A dead test at the top of the loop (once = 1 before the
// label, `if (once == 0) goto done;` after it, folded by gcse's last cprop) stops
// the hoist but leaves depth 1: 244. A never-taken jump from the entry to a label
// before wait(2) makes the loop irreducible, depth 0 (205), but PRE then hoists
// on both entry edges, and `(u8)(s32)actor >> 8` folds only to a cmp the listing
// lacks. An empty asm rewriting actor stops actor + 34 only: 257. None reaches
// differ 0, so all came out. No plain form was found that drops the back edge
// from gcse and flow and still ends in `b` to the label; the sibling drafts
// 02004140 and 0200470c stop at the same retry loop and 02003d80 at the same
// hoist. No PRE plus depth-0 refs is what optimize_size gives (gcse.c, flow.c):
// that is a compiler question, for Pascal (K3), and is untested here.
// Tried without gain: separate p and q (256, cse merges them or PRE keeps them
// apart), pos with no pointer (256), a terrain local (no change), for (;;) (290),
// radius or dx, dz, dist in other scopes and orders (no change), j != 64 (no
// differ change, gap 98), inline x and z (252), returns-twice on Func_0200410c
// (353). gData_020074c8 is volatile s16: it gives the ldrh, lsls, cmp reads.
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
  s32 startCell, x, z, cellX, cellZ, cell, i, j, dx, dz, dist, radius;
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
