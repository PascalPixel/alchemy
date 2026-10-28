// Draft, differ 64 of 158 (was 68): the push or strike of a listed actor the
// leader faces, set by its two flags. The list walk is a goto loop, as the
// native has no loop notes there, inside the test that the leader faces an
// actor, which gives the native's branches. Left: register choice: the
// native keeps the leader in r6, the actor id in r7 and the faced actor in
// r8; here the leader takes r8, the faced actor r7 and the id r6. A
// permuter form that enters the walk when nothing is faced (not the same
// behaviour) gets the native's registers (differ 5), so the allocation turns
// on that edge.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"
#include "flag_call.h"

void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
s32 Func_02003d20(void);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003df8(s32 cue);
void Func_02003c00(s32 a, s32 b, s32 c);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);

void Func_020002f8(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Actor_GetById(gAdventure.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003dd8(position, 0);
  if (target != NULL) {
  goto test;
next:
  {
    s32 id = list[0];
    s32 flag;

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    flag = list[1];
    Func_02003c28();
    if (GameFlag_Test(flag) == 0) {
      Func_02003df8(125);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(2);
      Func_02003ba0(target, 0);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      Func_02000974(0);
      GameFlag_SetBit(flag);
    } else {
      s32 done = flag + 1;
      s32 result = GameFlag_Test(done);

      if (result == 0) {
        leader->motionFlags = result;
        Func_02003df8(185);
        Call3((void (*)())Func_02003c00, 0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02003ba0(target, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, target->terrainId);
        wait(4);
        Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Func_02003df8(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(done);
      }
    }
    Func_02003c30();
    return;
  }
test:
  if (*list != 0xffff)
    goto next;
  }
}
