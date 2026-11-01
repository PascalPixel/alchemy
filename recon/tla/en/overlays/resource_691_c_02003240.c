// Matches: differ 0 (tla-en, 2026-11-01): the same function as overlay 68F's
// Func_02002764 and 67F's Func_02000708. From the list of {actor id, flag}
// pairs ended by 0xffff it finds the entry for the actor Func_02003c2c finds at
// the leader's position. If the entry's flag is clear, actor 8 flashes and the
// flag is set; if it is set and the next flag is clear, the leader's motion
// flags are cleared and set to 3 around a longer sequence, and the next flag is
// set. The names are this draft's guesses at most: nothing here is from the
// game's text.
// Everything turned on the native's allocation order, read from global.c: the
// leader in r5, actor 8 in r7, the id in r8 and the target in r10 need the
// leader and actor 8 to outrank the id and the target, which they do only if
// the loop's references are not weighted. As a plain goto loop the blocks have
// loop_depth 1, the id and the target get r5 and r7, and 65 of 191 instructions
// differ. The native behaves as if its loop had depth 0. The dead entry below
// gives the loop two entries while life analysis runs, so flow_loops_find sees
// no natural loop; combine then folds the test away, since a u16 has no bit 16,
// and no instruction of it survives. Reshaping tried first: the flags pointer
// set at several places, the call's result compared through a second local, the
// assignment inside the test, and u16 or block-scoped ids.
// `flags` is one pseudo in r8: adds r5, #85 and mov r8, r5 are reload's, and the
// terrain address stays a CSE pseudo in r5 only if no local holds it. A real
// lead for Pascal, outside this draft: the depth-0 reading may be a difference
// between alchemy-gcc's flow and Camelot's compiler, and other goto loops would
// show the same order.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_02003b8c(void);
struct Actor *Func_02003c2c(s32 *position, s32 flags);
void Func_02003aec(void);
s32 Func_02003af4(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003c54(s32 cue);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void wait(s32 frames);
void Func_02003a54(struct Actor *object, s32 mode);
void Func_02003224(struct Actor *object, struct Actor *target);
void Func_020034ac(s32 mode);
void Func_02003ac4(s32 x, s32 y, s32 z);
void Func_02003784(s32 level, s32 layer);
void Func_020035e0(void);

void Func_02003240(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target == 0)
    return;
  /* FAKEMATCH(dead): a test no u16 can pass enters the loop a second way, so it
   * is no natural loop and its references are not weighted; as if (target != 0)
   * goto test; return;, 65 of 191 instructions differ in tla-en. */
  if (list[0] & 0x10000)
    goto body;
  goto test;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02003aec();
      if (!GameFlag_Test(flag)) {
        Func_02003a54(actor8, 1);
        Func_02003224(actor8, target);
        wait(1);
        Func_02003c54(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(2);
        Func_02003a54(actor8, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_020034ac(0);
        Func_02003224(target, actor8);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02003a54(actor8, 0);
        Func_02003224(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Func_02003c54(185);
        /* FAKEMATCH(wrapper): forwarded through Call3, which builds each 0x10000
         * apart; as Func_02003ac4(0x10000, 0x40000, 0x10000), 21 of 191
         * instructions differ in tla-en. */
        Call3((void (*)())Func_02003ac4, 0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(actor8, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        /* FAKEMATCH(wrapper): forwarded through Call3, which builds each -1
         * apart; as Func_02003ac4(-1, -1, 0xe666), 23 of 191 instructions differ
         * in tla-en. */
        Call3((void (*)())Func_02003ac4, -1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Func_02003c54(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02003af4();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
