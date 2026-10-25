// Draft, differ 68 of 158: the push or strike of a listed actor the leader
// faces, set by its two flags. The list walk is a goto loop, as the native
// has no loop notes there. Left: register choice.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"
#include "flag_call.h"

void WaitFrames(s32 frames);
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
  if (target == NULL)
    return;
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
      WaitFrames(2);
      Func_02003ba0(target, 0);
      Object_SetPartAttribute(Object_GetById(id), 0);
      WaitFrames(2);
      Object_SetPartAttribute(Object_GetById(id), 7);
      WaitFrames(4);
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
        WaitFrames(8);
        Func_02003ba0(target, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        WaitFrames(16);
        Func_02000c4c(-2, target->terrainId);
        WaitFrames(4);
        Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
        WaitFrames(8);
        leader->motionFlags = 3;
        WaitFrames(5);
        Object_SetPositionAndResetMotion(target, 0, 0, 0);
        WaitFrames(2);
        Func_02003df8(188);
        Func_02000aa8();
        WaitFrames(20);
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
