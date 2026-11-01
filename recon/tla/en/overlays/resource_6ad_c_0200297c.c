// Draft, differ 148 of 236, skeleton gap 10/16 (tla-en, 2026-11-01)
// Overlay 6ad's copy of the slide seen at 6ab Func_02002148 and 6ac Func_020024e4.
// The frame loop's bottom test closes the PRE problem those two drafts name: with
// `running` tested at the bottom, entered by `goto test`, the loop has an exit edge
// while PRE runs, so actor + 34, work + 104, work + 106 stay in the loop. gcse folds
// the test only in its last cprop pass, after PRE, which leaves the listing's plain
// back branch; a real while loop lets loop.c hoist them instead (gap 23/29). The
// same form should open both siblings. `running` holds one constant for its whole
// life (S2); no source form is known, only this effect.
// Left, the frame address. The listing sets r1 in the loop head for the else arm,
// rebuilds it in the slide loop, and builds r2 at the scan call, with no slot for
// it. Here PRE keeps one reaching register. That costs a stack word, and r1 lost.
// This closes most of it, to differ 87, gap 11/12, frame 20: declare pos in the
// loop's block, set pos = &position in the head, use pos in the else arm, and add
// pos = &position after i++ and after j++. Those two stores are unread (`dead`),
// and they only stop gcse's last cprop folding the reaching register into pos. They
// came back out because they do not reach differ 0; the full text is
// work2/resource_6ad_c_0200297c/x3_with_dead_stores.c in the scratchpad.
// Left in that form: j takes r4 by caller-saves (refs 11, 1 call), radius r8, the
// reaching register fp and work a register, where the listing has j r8, radius fp,
// work spilled to sp+0, no frame word for the save. So something holds r4 across the
// scan at global-alloc, since radius (refs 5, 1 call) avoids it too, and reload then
// spills r4, evicting it. Likeliest the reaching register, which needs refs over
// 16 for 4 calls and priority over j (0.69); it has 13 and 0.25, its live length
// doubled by REG_EQUIV. Unproved.
// Left, the slide loop's exit: `if (i <= 9) goto sliding` gives ble/b/b, one branch
// too many. A do-while gives ble/b but loop.c moves the velocity-clearing block out
// (find_and_verify_loops needs a barrier at done's depth, so a real outer loop would
// stop it, but that hoists). The form that gives it here sent the exit to the loop
// test, which is the wrong target (differ 74, not kept).
// Func_02002800 returns s32 and stays unprototyped: that gives -10's order.
// Tried without gain on the differ 87 form: scan as do-while or for (93), pos set
// in the scan body (96) or before it (146), clamp held in a local from before the
// scan (131), mask likewise (90), low and high clamp together (165), x's store
// spelled three ways, the test at the else arm's end, a test at done.
#include "types.h"
#include "call.h"
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
s32 Func_02002800();
void Func_02005480(struct Actor *actor, s32 value);
s32 Func_020054d8(s32 layer, s32 x, s32 z);
s32 Func_020054f8(struct Actor *actor, struct SlideVector *position);
s32 Func_02005538(s32 *a, s32 b, struct SlideVector *c, s32 d);
void Func_02005560(void);
void Func_020055a8();
void Func_02005648(s32 a);
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Func_02002954(void);
extern u8 gData_020023c4[];

void Func_0200297c(void) {
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
  s32 running; // One constant for its whole life: see the header.

  if (gMainRoutineWork.renderMode == 0) {
    Func_02002800(-10, 0);
    actor = Actor_GetById(gMainRoutineWork.currentOwner);
    work = (struct SlideWork *)(gData_020023c4 + 0x188);
    Func_02005560();
    Func_02005648(0);
    actor->update = Func_02002954;
    actor->angle = 0x4000;
    Func_02005480(actor, 49);
    running = 1;
    goto test;
  top:
    {
      result = Func_020054d8(actor->terrainId, actor->x, actor->z);
      stepX = &work->stepX;
      stepZ = &work->stepZ;
      if (result == 7) {
        actor->velocityX = *stepX << 17;
        actor->velocityZ = *stepZ << 17;
        actor->acceleration = 0x1000;
        i = 0;
      sliding:
        position.x = actor->x + (*stepX << 19);
        position.y = actor->y;
        position.z = actor->z + (*stepZ << 19);
        if (Func_020054f8(actor, &position) != 0) {
          actor->velocityX = 0;
          actor->velocityZ = 0;
          goto done;
        }
        wait(1);
        i++;
        if (i <= 9)
          goto sliding;
        goto done;
      }
      position.x = actor->x + (*stepX << 19);
      position.y = actor->y;
      position.z = actor->z + (*stepZ << 19);
      result = Func_020054f8(actor, &position);
      if (result > 0)
        goto done;
      if (result < 0) {
        actor->angle = 0x4000;
        Call3((void (*)())Func_020055a8, gMainRoutineWork.currentOwner, 6, -1);
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
          if (Func_02005538(&other->x, radius, &position, other->radius - 2) >=
              0) {
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
