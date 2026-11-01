// Draft, differ 155 of 236, skeleton gap 21/28 (tla-en, 2026-11-01)
// Each pass checks the terrain ahead and the other solid actors, then steps
// by unk68 and unk6a and eases toward the centre of its cell. Left: gcse PRE
// (the edge into the outer loop, in the .gcse dump) hoists work + 104, work +
// 106, the frame address of point and actor + 34 above the loop's first call.
// The listing rebuilds all four in the loop, so its registers differ from
// there on: the two field addresses in sl and r9, the search's counter in r8
// where this spills it through r4.
// point's address cannot be killed, so the cause is not a set inside the loop.
// Measured: a real for (;;) keeps the exit blocks in line, a goto outer loop
// lets loop.c move them out (166). A running flag tested at the loop's top
// after a label stops PRE and reaches 130, and second dead sets stop loop.c's
// move of the pointers (skeleton 12/19), but the flag is never cleared, so
// that is a device; it did not approach differ 0 and was taken out. A copy of
// work at the top, one pointer set at each site, pointer locals against
// direct fields, clamp constants in locals (178), an indexed search (232) and
// the call inline in the compare (163) gained nothing. Func_02004ca8 takes
// the actor alone (r1 and r2 are not set for it), and work is really read
// uninitialized when renderMode is nonzero. Same family as the landing-burst
// drafts: the next step is finding why the native does not hoist here.
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
      kind = Func_02004c18(actor->terrainId, actor->x, actor->z);
      if (kind == 7) {
        s32 i;
        actor->velocityX = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = 0x1000;
        for (i = 0; i <= 9; i++) {
          point[0] = actor->x + (work->unk68 << 19);
          point[1] = actor->y;
          point[2] = actor->z + (work->unk6a << 19);
          if (Func_02004c38(actor, point) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            goto done;
          }
          wait(1);
        }
        goto done;
      }
      point[0] = actor->x + (work->unk68 << 19);
      point[1] = actor->y;
      point[2] = actor->z + (work->unk6a << 19);
      result = Func_02004c38(actor, point);
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
        x = actor->x + (work->unk68 << 17);
        actor->x = x;
        z = actor->z + (work->unk6a << 17);
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
