// Draft, differ 112 of 236, skeleton gap 8/12 (tla-en, 2026-11-01)
// Overlay 6ac's Func_020024e4, the same slide as 6ab Func_02002148 and 6ad
// Func_0200297c. All three loops are goto loops: real ones move the exit blocks
// to the end of the function (loop.c). Func_02002368 stays unprototyped: that
// gives -10's argument order. The renderMode test reaches the last store with
// work unset, in JA too.
// What closed the gap this round, from differ 136, gap 18/24, frame 20:
// - base and data are gone. work is pooled, and update_equiv_regs refuses an
//   equivalence for a MEM source in a likely-spilled class, so with every use
//   through work itself it keeps a stack slot at sp+0, as the listing does. A
//   copy changes its preferred class, and it is rematerialized from the pool.
// - The frame loop is entered at its test, as in 6ad, so PRE leaves actor + 34,
//   work + 104 and work + 106 in the loop. CHECK THIS FORM: running holds one
//   constant (S2) and its test emits nothing; gcse's last cprop folds it after
//   PRE. Without it, differ 160, gap 19/25; the old base/data form with the
//   exit below is differ 135, gap 18/23. No plain source for it is known.
// - pos is set in the head, the slide and the scan, always to &position, and
//   each is read by a call. That leaves PRE's reaching register spilled with a
//   REG_EQUIV, so frame 16 and no slot for it, and the slide and scan rebuild
//   the address as the listing does.
// - `if (i > 9) goto done; goto sliding;` gives ble/b. `if (i <= 9) goto sliding`
//   threads through the `goto done` after it to bgt/b/b.
// Left:
// - pos takes r4, the listing's r1. The scan's use conflicts with r0 and r1,
//   since a pseudo feeding r2 lives through their argument sets. The listing's
//   scan builds r2 itself; here that needs pos killed in the scan loop or the
//   last cprop turns R back into pos and pos crosses 4 calls (differ 207). Only
//   an unread set did it (`pos = &position` after j++, differ 91, `dead`, not
//   kept; 6ad's header has the same result, and there R then takes a register).
//   A second pointer copied from pos merges in cse2.
// - radius takes sl and stepX, stepZ r9, fp; the listing has fp and sl, r9.
//   Priorities are radius 5 refs over 27 insns, stepX 9 over 83, stepZ 9 over
//   87. Its refs match the CFG, so the listing's lives differ, cause unknown.
//   Tried: step pointer order (no gain), radius at function scope (same), radius
//   in the call (differ 124).
// - The x store stays before stepZ's ldrsh in the listing and sinks here; spelled
//   as `x = actor->x += ...` it does not move. A load that conflicts with it
//   would explain that. Scheduling pairs: i = 0, and -1's negs at Func_02003200,
//   unmoved by i = 0's position or Func_02003200's prototype.
// Tried without gain: flag sets on the exits (differ 175 and worse), real while
// or do-while (gap 23/28), for or do-while for the slide (gap 14/19), a
// bottom-entered slide or scan (no gain, or an entry branch), a one-pass block
// (experiment, worse), pos set once before the loop (differ 127, j takes r4).
#include "types.h"
#include "actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"

struct SlideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SlideWork {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x62];
  s16 stepX;
  s16 stepZ;
};

void Animation_ApplyChildValues(struct Actor *actor, s32 value);
s32 Func_02002368();
void Func_02003118(struct Actor *actor, s32 value);
s32 Func_02003168(s32 layer, s32 x, s32 z);
s32 Func_02003180(struct Actor *actor, struct SlideVector *position);
s32 Func_020031b0(s32 *a, s32 b, struct SlideVector *c, s32 d);
void Func_020031e0(void);
s32 Func_02003200(s32 a, s32 b, s32 c);
void Func_02003280(s32 a);
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Func_020024bc(void);
extern u8 gData_020023c4[];


void Func_020024e4(void) {
  struct SlideWork *work;
  struct Actor *actor;
  struct Actor *other;
  struct SlideVector position;
  s16 *stepX;
  s16 *stepZ;
  u8 *flags;
  s32 result;
  s32 i;
  s32 j;
  s32 running;
  struct SlideVector *pos;

  if (gMainRoutineWork.renderMode == 0) {
    Func_02002368(-10, 0);
    actor = Actor_GetById(gMainRoutineWork.currentOwner);
    work = (struct SlideWork *)(gData_020023c4 + 0x188);
    Func_020031e0();
    Func_02003280(0);
    actor->update = Func_020024bc;
    actor->angle = 0x4000;
    Func_02003118(actor, 49);
    running = 1;
    goto test;
  top:
    {
      result = Func_02003168(actor->terrainId, actor->x, actor->z);
      stepX = &work->stepX;
      stepZ = &work->stepZ;
      pos = &position;
      if (result == 7) {
        actor->velocityX = *stepX << 17;
        actor->velocityZ = *stepZ << 17;
        actor->acceleration = 0x1000;
        i = 0;
      sliding:
        pos = &position;
        position.x = actor->x + (*stepX << 19);
        position.y = actor->y;
        position.z = actor->z + (*stepZ << 19);
        if (Func_02003180(actor, pos) != 0) {
          actor->velocityX = 0;
          actor->velocityZ = 0;
          goto done;
        }
        wait(1);
        i++;
        if (i > 9)
          goto done;
        goto sliding;
      }
      position.x = actor->x + (*stepX << 19);
      position.y = actor->y;
      position.z = actor->z + (*stepZ << 19);
      result = Func_02003180(actor, pos);
      if (result > 0)
        goto done;
      if (result < 0) {
        actor->angle = 0x4000;
        Func_02003200(gMainRoutineWork.currentOwner, 6, -1);
        goto done;
      }
      {
        s32 radius = actor->radius - 2;
        s32 x;
        s32 z;
        s32 snap;

        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        j = 0;
      scan:
        if (other->script != NULL && (*flags & 1) != 0 && other != actor) {
          pos = &position;
          if (Func_020031b0(&other->x, radius, pos, other->radius - 2) >= 0) {
            wait(1);
            goto done;
          }
        }
        j++;
        flags += sizeof(struct Actor);
        other++;
        if (j <= 63)
          goto scan;
        x = actor->x += *stepX << 17;
        z = actor->z + (*stepZ << 17);
        actor->z = z;
        snap = (x & 0xfffff) - 0x80000;
        if (snap > 0x10000)
          snap = 0x10000;
        if (snap < -0x10000)
          snap = -0x10000;
        actor->x = x - snap;
        snap = (z & 0xfffff) - 0x80000;
        if (snap > 0x10000)
          snap = 0x10000;
        if (snap < -0x10000)
          snap = -0x10000;
        actor->z = z - snap;
        work->unk04 = 0;
        wait(1);
      }
    }
  test:
    if (running)
      goto top;
  done:
    actor->update = NULL;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
