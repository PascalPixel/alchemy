// Draft, differ 85 of 191, skeleton gap 25 (tla-en, 2026-11-01)
// The same function as overlay 691's Func_02003240, which matches, and 67F's
// Func_02000708: from a 0xffff-ended list of {actor id, flag} pairs it finds the
// entry for the actor Func_02003188 finds at the leader's position, then runs
// actor 8's flash, or the leader's longer sequence if the next flag is clear.
// The names are guesses at most: nothing here is from the game's text.
// This draft is the plain C, with no device. Its shape, calls, branches and
// frame are the native's; the gap is two causes, measured here:
// - The native's allocation (leader r5, actor 8 r7, id r8, target r10) needs the
//   loop's references unweighted: refs and live lengths in the lreg dump give
//   that order only at loop_depth 0, and here the blocks have depth 1, so the
//   id takes r5. With the twin's FAKEMATCH(dead) entry (a test no u16 can pass,
//   so the loop has two entries) differ falls from 85 to 28.
// - Func_02003000's arguments: CSE shares 0x10000 and -1. Through Call3, as in
//   the twin, differ falls to 65; with both devices, differ 0, gap 0.
// Neither device is here: this round adds none, and a tagged copy of the twin is
// a decision for its owner. Why the native behaves as depth 0 is a compiler
// question (K3) for Pascal, as the twin's header says.
// Tried, no real gain (some with Call3, at 64 to 87): do-while or labels for the loop, an if around
// it or an early return; a real while (hoists -2 and the flags address, 136);
// flag and next as locals; flags set at the top of the arm, the else (shape
// lost) or not at all; u16 id (80 with Call3); assignment inside the test;
// Func_02003000 unprototyped or unsigned; constants in set-once locals.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 Func_020030f8(void);
struct Actor *Func_02003188(s32 *position, s32 mode);
void Func_02003048(void);
void Func_02003050(void);
void Func_020031a8(s32 sound);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void wait(s32 frames);
void Func_02002748(struct Actor *from, struct Actor *to);
void Func_02002f80(struct Actor *object, s32 mode);
void Func_020029d0(s32 mode);
void Func_02003000(s32 a, s32 b, s32 c);
void Func_02002ca8(s32 a, s32 layer);
void Func_02002b04(void);

void Func_02002764(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Func_020030f8() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003188(position, 0);
  if (target != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02003048();
      if (!GameFlag_Test(flag)) {
        Func_02002f80(actor8, 1);
        Func_02002748(actor8, target);
        wait(1);
        Func_020031a8(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(2);
        Func_02002f80(actor8, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_020029d0(0);
        Func_02002748(target, actor8);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02002f80(actor8, 0);
        Func_02002748(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Func_020031a8(185);
        Func_02003000(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Func_02002f80(actor8, 2);
        Func_02002ca8(-2, target->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, target->terrainId);
        wait(4);
        Func_02003000(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Func_020031a8(188);
        Func_02002b04();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02003050();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
