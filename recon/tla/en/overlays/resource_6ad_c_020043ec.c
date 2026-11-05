// Draft, differ 210 of 347, skeleton gap 72 (tla-en, 2026-11-01)
// Twin: The Broken Seal's Imil_RunIceSlide (a `layer` local, probe, x and z).
// Control flow, types and the 40-byte frame agree with the listing. The slots
// are the listing's: pos 28, startCell 24, x 20, z 16, cellX 12, cellZ 8, layer 4,
// radius 0. Closed this round in plain C, in order: the second wait loop as labels
// (a real while hoists gData_020074c8's address: 248 to 233), `multiply`, a local
// holding IwramMulQ16 for its whole life (228: it takes r8 as the listing does,
// and the x, z and layer slots line up), dz before dx (210). No device was added;
// the draft carries none to remove. `multiply` is set at the top of the loop,
// though the listing loads it just before the first multiply: set there it crosses
// one call, is saved around it in r3 and differs by 247. Set above the label: 218.
// What is left comes from the outer retry loop, in two ways:
// - PRE: gcse hoists actor + 34 and pos's address onto the entry edge (07.gcse:
//   PRE/HOIST edge (0,1)). The listing computes actor + 34 after the label, keeps
//   p in r10 for the first region, makes another in r6 at the test, and builds pos
//   at each call. Both expressions are anticipated at the label and available on
//   the back edge, so a loop that always runs them is hoisted, whatever its form.
//   This is most of the skeleton gap (35 removed, 37 added): the hoisted pseudo
//   also holds r10, so radius takes r8 rather than sp+0, j takes r4 with a save at
//   sp+0 rather than r10, the giv takes r6 rather than r8, and angle and cell swap
//   r9 and r11.
// - Weight: flow counts the block at depth 1 (12.life). The listing's allocation
//   fits depth 0, as if the back edge were not there.
// Earlier measurements, none kept: `goto start` replaced by `return` gave 155. A
// dead test at the top of the loop stopped the hoist but left depth 1 (244); a
// never-taken jump to a label before wait(2) made the loop irreducible, depth 0
// (205), but PRE then hoisted on both edges; an empty asm rewriting actor stopped
// actor + 34 only (257). All are devices. The sibling drafts 02004140 and 0200470c
// stop at the same retry loop and 02003d80 at the same hoist. No PRE plus depth-0
// refs is what optimize_size gives (gcse.c, flow.c): a compiler question for Pascal
// (K3), out of scope here and untested.
// Tried without gain this round: no pointer, or no `p = pos` at the test (230);
// separate p and q (230); struct pos (233); a `layer` set above the label (238);
// volatile off (235) or u16 (259); inline x and z, angle or cell declared elsewhere,
// radius and j in a block, the scan with continue, other++ in the body, nested ifs
// in the poll test, cell assigned in the test, if/else for angle, one sum for dist,
// z before x, dist > 0x3ffff, << 16 for the marker (all no change); other before
// radius (214); velocity before angle in the blocked arm (247); dz and dx locals
// for ArcTan2 (341). One expression for the two angle lines gives differ 206 but
// gap 92, so it came back out: it drops the listing's lsls and asrs pair.
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
  s32 (*multiply)(s32, s32);
  s32 startCell, x, z, cellX, cellZ;
  u8 *layer;
  s32 cell, i, j, dx, dz, dist, radius;
  s32 angle;
  struct Actor *other;

  actor = ObjectTable_Get(FIELD_AT(gMainRoutineWork, s32, 532));
  gData_020074c8 = 0;
start:
    multiply = IwramMulQ16;
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
    z = cellZ;
    x = cellX;
    layer = &actor->terrainId;
    startCell = Func_020054d0(*layer, x, z);
    dz = z - actor->z;
    dx = x - actor->x;
    dist = multiply(dx, dx);
    dist += multiply(dz, dz);
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
    cell = Func_020054d0(*layer, p[0], p[2]);
    if (cell != 255 &&
        Map_GetTerrainHeight(*layer, p[0], p[2]) - actor->y <= 0x80000) {
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
      if (Map_GetTerrainHeight(*layer, p[0], p[2]) - actor->y > 0x80000)
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
      goto poll;
    pause:
      wait(1);
      i++;
    poll:
      if (i <= 179 && Func_02005530(actor) == 0 && gData_020074c8 == 0)
        goto pause;
      if (gData_020074c8 != 0)
        goto done;
      if (cell != startCell)
        goto settle;
  test:
      Vector_AddPolarOffsetFar(0x100000, angle, pos);
      p = pos;
      cell = Func_020054d0(*layer, p[0], p[2]);
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
