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
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"
#include "flag_call.h"

void WaitTicks(s32 ticks);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
s32 Func_02003d20(void);
struct ObjectRuntime *Func_02003dd8(s32 *position, s32 mode);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003df8(s32 cue);
void Func_02003c00(s32 a, s32 b, s32 c);
void Func_02003ba0(struct ObjectRuntime *object, s32 mode);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);

void Func_020002f8(u16 *list) {
  struct ObjectRuntime *leader;
  struct ObjectRuntime *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Object_GetById(gPartyState.currentOwner);
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

    if (Object_GetById(id) != target) {
      list += 2;
      goto test;
    }
    flag = list[1];
    Func_02003c28();
    if (GameFlag_Test(flag) == 0) {
      Func_02003df8(125);
      Object_SetPartAttribute(Object_GetById(id), 7);
      WaitTicks(2);
      Func_02003ba0(target, 0);
      Object_SetPartAttribute(Object_GetById(id), 0);
      WaitTicks(2);
      Object_SetPartAttribute(Object_GetById(id), 7);
      WaitTicks(4);
      Object_SetPartAttribute(Object_GetById(id), 0);
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
        WaitTicks(8);
        Func_02003ba0(target, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        WaitTicks(16);
        Func_02000c4c(-2, target->terrainId);
        WaitTicks(4);
        Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
        WaitTicks(8);
        leader->motionFlags = 3;
        WaitTicks(5);
        Object_SetPositionAndResetMotion(target, 0, 0, 0);
        WaitTicks(2);
        Func_02003df8(188);
        Func_02000aa8();
        WaitTicks(20);
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
