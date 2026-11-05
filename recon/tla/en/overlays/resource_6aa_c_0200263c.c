// Draft, differ 155 of 236, skeleton gap 17/26 (tla-en, 2026-11-01)
// Each pass checks the terrain ahead and the other solid actors, then steps
// by unk68 and unk6a and eases toward the centre of its cell. Left: gcse PRE
// (edge 1 to 2 in the .gcse dump) hoists actor + 34, work + 104, work + 106 and
// the frame address of point above the loop's first call. The listing keeps
// all four in the loop: actor + 34 first thing, the other three after the call,
// straight before cmp r0, #7, in sl, r9 and r1. Everything else follows from
// that: this draft spills actor + 34 (frame 24 against 16), so registers differ.
// Measured with dx, dz and p set at that point, as here: alchemy-gcc reproduces
// the listing's whole first block once PRE is stopped. A goto loop opening with
// `if (running)` on a flag that is 1 and never cleared does it (cprop folds the
// test but PRE still sees its edge; skeleton 17/22, differ 169); that flag is a
// device, so it is not used. Tried without it: for (;;) with no locals (156),
// goto outer loop (162), a done flag in while (212, rotated, so PRE still
// hoists), the flag cleared on each exit with goto top (185 to 194), a loop
// once-only block after the call (158), locals' order, set order and locals set
// before the loop (155 to 159).
// The listing also does not merge the search's point with p: `add r2, sp, #4`
// is rebuilt in the loop and p dies in r1 at the call. PRE here replaces it
// with p's register, which then lives across calls in r4. No C form here keeps
// the two apart, since sfp + N is never killed. Both facts read as no PRE at
// all in this function, and three addresses placed just before the compare
// read like a hoist to the end of the block. That is a compiler question, out
// of scope for a draft and not tested; same family as the landing-burst drafts.
// Func_02004ca8 takes the actor alone (r1 and r2 are not set for it), and work
// is really read uninitialized when renderMode is nonzero.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

struct RideWork {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x62];
  s16 unk68; ///< added to x, << 19 for the point ahead, << 17 for a step
  s16 unk6a; ///< the same for z
};

extern u8 gData_020023c4[];

s32 Func_020024c0(s32 a, s32 b);
struct Actor *Actor_GetById(s32 id);
s32 Func_02004ca8(struct Actor *actor);
s32 Func_02004da0(s32 a);
void Func_02004bc0(struct Actor *actor, s32 a);
s32 Func_02004c18(s32 terrainId, s32 x, s32 z);
s32 Func_02004c38(struct Actor *actor, s32 *point);
s32 Func_02004c78(s32 *position, s32 radius, s32 *point, s32 otherRadius);
s32 Func_02004ce8(s32 id, s32 a, s32 b);
void Func_02002614(void);
s32 Animation_ApplyChildValues(struct Actor *actor, s32 a);
void wait(s32 frames);

void Func_0200263c(void) {
  struct RideWork *work;
  struct Actor *actor;
  s32 point[3];

  if (gMainRoutineWork.renderMode == 0) {
    Func_020024c0(-10, 0);
    actor = Actor_GetById(gMainRoutineWork.currentOwner);
    work = (struct RideWork *)(gData_020023c4 + 0x188);
    Func_02004ca8(actor);
    Func_02004da0(0);
    actor->update = Func_02002614;
    actor->angle = 0x4000;
    Func_02004bc0(actor, 49);
    for (;;) {
      s32 result;
      s32 kind;
      s16 *dx, *dz;
      s32 *p;
      kind = Func_02004c18(actor->terrainId, actor->x, actor->z);
      dx = &work->unk68;
      dz = &work->unk6a;
      p = point;
      if (kind == 7) {
        s32 i;
        actor->velocityX = *dx << 17;
        actor->velocityZ = *dz << 17;
        actor->acceleration = 0x1000;
        for (i = 0; i <= 9; i++) {
          point[0] = actor->x + (*dx << 19);
          point[1] = actor->y;
          point[2] = actor->z + (*dz << 19);
          if (Func_02004c38(actor, point) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            goto done;
          }
          wait(1);
        }
        goto done;
      }
      p[0] = actor->x + (*dx << 19);
      p[1] = actor->y;
      p[2] = actor->z + (*dz << 19);
      result = Func_02004c38(actor, p);
      if (result > 0)
        goto done;
      if (result < 0) {
        actor->angle = 0x4000;
        Func_02004ce8(gMainRoutineWork.currentOwner, 6, -1);
        goto done;
      }
      {
        struct Actor *other;
        s32 radius;
        s32 i;
        s32 x, z, d;

        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        for (i = 0; i <= 63; i++, other++) {
          if (other->script != 0 && (other->collisionFlags & 1) != 0 &&
              other != actor &&
              Func_02004c78(&other->x, radius, point, other->radius - 2) >= 0) {
            wait(1);
            goto done;
          }
        }
        x = actor->x + (*dx << 17);
        actor->x = x;
        z = actor->z + (*dz << 17);
        actor->z = z;
        d = (x & 0xfffff) - 0x80000;
        if (d > 0x10000)
          d = 0x10000;
        if (d < -0x10000)
          d = -0x10000;
        actor->x = x - d;
        d = (z & 0xfffff) - 0x80000;
        if (d > 0x10000)
          d = 0x10000;
        if (d < -0x10000)
          d = -0x10000;
        actor->z = z - d;
      }
      work->unk04 = 0;
      wait(1);
    }
  done:
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
