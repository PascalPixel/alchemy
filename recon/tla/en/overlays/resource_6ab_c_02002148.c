// Draft, differ 146 of 236, skeleton gap 40 (tla-en, 2026-11-01)
// Control flow, types and every load agree with the listing; 242 instructions
// against 236, frame 24 against 16. Left, one cause: gcse's PRE (.07.gcse,
// "PRE/HOIST: edge (1,2)") moves &actor->terrainId, work + 104, work + 106 and
// &position from the loop onto its entry edge. The bytes build all four inside
// the loop, after the terrain call. The four new pseudos are live across the
// loop: &position takes r8 (the listing's scan counter), &actor->terrainId
// spills to sp + 4, and the counter falls to r4 with a caller save at sp + 0
// (11 weighted refs beat 4 * 1 call). Every other register difference follows.
// Nothing in plain C kills these expressions: actor and work are set once and
// the frame address never changes, and the sibling drafts 64b_020006d0,
// 652_02000878 and 661_02003b80 reach the same end. Spelling the fields through
// pointers or a pos local, separate clamp locals, a for (;;) head and hoisting
// the pointers by hand change nothing or lose. A real for or do-while for the
// steps loses (153): loop.c moves the velocity-clearing block out; a for for
// the scan loses too (153). Sharing i between the two loops loses (158).
// Call3 stays: a direct Func_0200436c gives 148 or 157.
// Func_02001fcc's -10 stays negated before its 0, listing the other way round;
// a prototype or a one-constant local does not move it.
// `goto done` has to be the test: `if (i < 10) goto step` threads into a far
// branch and costs one b.n.
// The renderMode test reaches the last store with `work` never set, in JA too:
// the skip path reads an unset local.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "vector3.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_02001fcc();
struct Actor *Actor_GetById(s32 id);
void Func_020042dc(void);
void Func_0200444c(s32 a);
void Func_020041cc(struct Actor *actor, s32 a);
s32 Func_0200422c(s32 terrain, s32 x, s32 z);
s32 Func_0200424c(struct Actor *actor);
void Func_0200436c();
s32 Func_0200428c(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002120(void);
extern u8 gData_020023c4[];

void Func_02002148(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

  if (main->renderMode == 0) {
    struct Actor *actor;

    ((s32 (*)())Func_02001fcc)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_020042dc();
    Func_0200444c(0);
    actor->update = Func_02002120;
    actor->angle = 0x4000;
    Func_020041cc(actor, 49);
  loop:
    {
      s32 result;

      result = Func_0200422c(actor->terrainId, actor->x, actor->z);

      if (result == 7) {
        s32 i;

        actor->velocityX = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = 0x1000;
        i = 0;
      step:
        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        if (Func_0200424c(actor) != 0) {
          actor->velocityX = 0;
          actor->velocityZ = 0;
          goto done;
        }
        wait(1);
        i++;
        if (i >= 10)
          goto done;
        goto step;
      } else {
        s32 result2;

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        result2 = Func_0200424c(actor);
        if (result2 > 0)
          goto done;
        if (result2 < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_0200436c, gMainRoutineWork.currentOwner, 6,
                -1);
          goto done;
        }
        {
          s32 radius = actor->radius - 2;
          struct Actor *other = Ram_HeapSlots->scriptObjects;
          s32 i;
          s32 x, z, off;

          u8 *flags = &other->collisionFlags;

          i = 0;
        scan:
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Func_0200428c(&other->x, radius, &position, other->radius - 2) >=
                  0) {
            wait(1);
            goto done;
          }
          i++;
          flags += 128;
          other++;
          if (i < 64)
            goto scan;
          x = actor->x += work->unk68 << 17;
          z = actor->z + (work->unk6a << 17);
          actor->z = z;
          off = (x & 0xfffff) + 0xfff80000;
          if (off > 0x10000)
            off = 0x10000;
          if (off < (s32)0xffff0000)
            off = 0xffff0000;
          actor->x = x - off;
          off = (z & 0xfffff) + 0xfff80000;
          if (off > 0x10000)
            off = 0x10000;
          if (off < (s32)0xffff0000)
            off = 0xffff0000;
          actor->z = z - off;
          work->unk04 = 0;
          wait(1);
          goto loop;
        }
      }
    }
  done:
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
