// Matches: differ 0 (tla-en, 2026-11-01): walks a 0xffff-ended list of (actor,
// flag) pairs for the actor Func_02003c2c finds at the leader's position, and
// plays that actor's stage for the flag and the next one. A relative of
// Func_02004200 in magma_rock_inside/s4200.c, whose forms this follows.
// The barrier: the plain C has every instruction shape but leader, id and target
// in r8, r6, r7 where the native has r6, r7, r8. global.c ranks id above leader
// and target above leader, and target then evicts the motion address from r6.
// The asm takes leader from 5 refs to 11 (loop weight), which puts it first; it
// sits in the step block so that no scheduling changes. No plain form was found:
// split or shared pointers, p = target then p += 34 (cse folds it), comparing the
// call's result before assigning, inner-scope locals and flag + 1 inline.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
s32 Func_02003b8c(void);
struct Actor *Func_02003c2c(s32 *position, s32 flags);
void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003c54(s32 cue);
void Func_02003a54(struct Actor *object, s32 mode);
void Func_020034ac(s32 mode);
void Func_02003ac4(s32 x, s32 y, s32 z);
void Func_02003784(s32 level, s32 layer);
void Func_020035e0(void);

void Func_02002e30(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target != NULL)
    goto test;
  return;
body:
  {
    s32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      /* FAKEMATCH(barrier): the form kept; as no asm at all, 64 of 158 instructions differ in tla-en. */
      asm("" : "=r"(leader) : "0"(leader));
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02003aec();
      if (!GameFlag_Test(flag)) {
        Func_02003c54(125);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        wait(2);
        Func_02003a54(target, 0);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        Func_020034ac(0);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        leader->motionFlags = 0;
        Func_02003c54(185);
        Call3((void (*)())Func_02003ac4, 0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(target, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        Call3((void (*)())Func_02003ac4, -1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Func_02003c54(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
    }
    Func_02003af4();
    return;
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
