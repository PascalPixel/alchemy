#include "scene.h"
#include "actor.h"
#include "call.h"

void Func_02000b84(s32 x, s32 y, s32 z);
void Func_02000f24(void);
void Func_02002ca0(void);
void Func_02004100(s32 actor, s32 a, s32 b);
void Func_02004108(s32 actor, s32 a);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetModeById(u32 id, s32 mode);

#define OBJ(p) ((struct Actor *)(p))

void Func_02000fc8(void) {
  struct Actor *leader = OBJ(Actor_GetById(gSequenceParentWork.currentOwner));
  struct Actor *partner = OBJ(Actor_GetById(21));
  u32 i;
  s32 angle;

  if (!GameFlag_Test(0x88b))
    return;
  Func_02004040();
  Func_020041b8(0);
loop_1:
  OBJ(gData_0200480c)->scaleX += -0x147a;
  OBJ(gData_0200480c)->scaleY += -0x147a;
  wait(1);
  if (0xcccb < OBJ(gData_0200480c)->scaleX)
    goto loop_1;
loop_2:
  OBJ(gData_0200480c)->scaleX += 0x1999;
  OBJ(gData_0200480c)->scaleY += 0x1999;
  wait(1);
  if (0x1cccc >= OBJ(gData_0200480c)->scaleX)
    goto loop_2;
loop_3:
  OBJ(gData_0200480c)->scaleX += -0xa3d;
  OBJ(gData_0200480c)->scaleY += -0xa3d;
  wait(1);
  if (0x3332 < OBJ(gData_0200480c)->scaleX)
    goto loop_3;
  Scene_Call4090(22, 0, 0);
  FieldEvent_Wait(20);
  Scene_CamBounds(0xe80000, -1, 0xc40000, 1);
  Func_02004100(gSequenceParentWork.currentOwner, 0x101, 0);
  Scene_SetSpeed(gSequenceParentWork.currentOwner, 0xcccc, 0x6666);
  Scene_SetPositionAndReset(gSequenceParentWork.currentOwner, 232, 164);
  FieldEvent_Wait(20);
  angle = 0x4000;
  Scene_Call4098(21, 232 << 16, 184 << 16, angle);
  wait(1);
  Func_02000b84(232 << 16, 0, 184 << 16);
  Func_020041f8(176);
  partner->velocityY = 0x60000;
  /* FAKEMATCH(barrier): an empty asm after the velocity store, which
   * addresses priorityFlags after it as the native does; without it, 2 of 279
   * instructions differ in tla-en. */
  asm("");
  partner->priorityFlags &= ~2;
  Actor_SetSpritePriority(21, 2);
  Actor_SetPartAttribute(Actor_GetById(21), 0);
  Actor_SetSingleChildFlags(Actor_GetById(21), 1);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Scene_SetPositionAndCommit(21, 216, 184);
  Actor_ArmCallback(gSequenceParentWork.currentOwner, angle, 0);
  Func_020040e8(21, 0);
  FieldEvent_Wait(20);
  Func_02002ca0();
  Func_020041f8(136);
  Func_02000f24();
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  FieldEvent_Wait(20);
  Func_02004108(gSequenceParentWork.currentOwner, 0x102);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 28);
  FieldEvent_Wait(10);
  Func_020041f8(204);
  i = 0;
step_13:
  leader->scaleX += -0x400;
  leader->scaleY += -0x400;
  leader->y += -0x20000;
  wait(1);
  i++;
  if (i <= 13)
    goto step_13;
  Actor_SetPartAttribute(Actor_GetById(gSequenceParentWork.currentOwner), 15);
  Func_020041f8(153);
  partner->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(21), 0);
  Scene_SetPositionAndCommit(21, 232, 168);
  FieldEvent_Wait(3);
  Func_020041f8(204);
  i = 0;
step_5:
  partner->scaleX += -0x800;
  partner->scaleY += -0x800;
  partner->y += -0x70000;
  wait(1);
  i++;
  if (i <= 5)
    goto step_5;
  Scene_Call4090(21, 0, 0);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(4);
}
