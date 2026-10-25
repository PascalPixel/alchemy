// Draft, differ 8 of 279: a Yampi Desert scene with actor 21 after flag
// 0x88b: the camera target shrinks, actor 21 lands in a ring of effects and
// both sink away. Left: in each scale loop the native loads the scale before
// it finishes the constant, and it addresses priorityFlags after storing the
// velocity (permute reached 6 only with a one-pass block over half the
// function).
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "object_runtime.h"
#include "call.h"

void Func_02000b84(s32 x, s32 y, s32 z);
void Func_02000f24(void);
void Func_02002ca0(void);
void Func_02004100(s32 actor, s32 a, s32 b);
void Func_02004108(s32 actor, s32 a);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Object_SetModeById(u32 id, s32 mode);

#define OBJ(p) ((struct ObjectRuntime *)(p))

void Func_02000fc8(void) {
  struct ObjectRuntime *leader = OBJ(Object_GetById(gPartyState.currentOwner));
  struct ObjectRuntime *partner = OBJ(Object_GetById(21));
  u32 i;
  s32 angle;

  if (!GameFlag_Test(0x88b))
    return;
  Func_02004040();
  Func_020041b8(0);
loop_1:
  OBJ(gData_0200480c)->scaleX += -0x147a;
  OBJ(gData_0200480c)->scaleY += -0x147a;
  WaitFrames(1);
  if (0xcccb < OBJ(gData_0200480c)->scaleX)
    goto loop_1;
loop_2:
  OBJ(gData_0200480c)->scaleX += 0x1999;
  OBJ(gData_0200480c)->scaleY += 0x1999;
  WaitFrames(1);
  if (0x1cccc >= OBJ(gData_0200480c)->scaleX)
    goto loop_2;
loop_3:
  OBJ(gData_0200480c)->scaleX += -0xa3d;
  OBJ(gData_0200480c)->scaleY += -0xa3d;
  WaitFrames(1);
  if (0x3332 < OBJ(gData_0200480c)->scaleX)
    goto loop_3;
  Scene_Call4090(22, 0, 0);
  FieldEvent_WaitFrames(20);
  Scene_CamBounds(0xe80000, -1, 0xc40000, 1);
  Func_02004100(gPartyState.currentOwner, 0x101, 0);
  Scene_SetSpeed(gPartyState.currentOwner, 0xcccc, 0x6666);
  Scene_SetPositionAndReset(gPartyState.currentOwner, 232, 164);
  FieldEvent_WaitFrames(20);
  angle = 0x4000;
  Scene_Call4098(21, 232 << 16, 184 << 16, angle);
  WaitFrames(1);
  Func_02000b84(232 << 16, 0, 184 << 16);
  Func_020041f8(176);
  partner->velocityY = 0x60000;
  partner->priorityFlags &= ~2;
  ObjectMotion_SetActionVariant(21, 2);
  Object_SetPartAttribute(Object_GetById(21), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(21), 1);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Scene_SetPositionAndCommit(21, 216, 184);
  ObjectMotion_ArmCallback(gPartyState.currentOwner, angle, 0);
  Func_020040e8(21, 0);
  FieldEvent_WaitFrames(20);
  Func_02002ca0();
  Func_020041f8(136);
  Func_02000f24();
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  FieldEvent_WaitFrames(20);
  Func_02004108(gPartyState.currentOwner, 0x102);
  Object_SetModeById(gPartyState.currentOwner, 27);
  FieldEvent_WaitFrames(30);
  Object_SetModeById(gPartyState.currentOwner, 28);
  FieldEvent_WaitFrames(10);
  Func_020041f8(204);
  i = 0;
step_13:
  leader->scaleX += -0x400;
  leader->scaleY += -0x400;
  leader->y += -0x20000;
  WaitFrames(1);
  i++;
  if (i <= 13)
    goto step_13;
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  Func_020041f8(153);
  partner->velocityY = 0x80000;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(21), 0);
  Scene_SetPositionAndCommit(21, 232, 168);
  FieldEvent_WaitFrames(3);
  Func_020041f8(204);
  i = 0;
step_5:
  partner->scaleX += -0x800;
  partner->scaleY += -0x800;
  partner->y += -0x70000;
  WaitFrames(1);
  i++;
  if (i <= 5)
    goto step_5;
  Scene_Call4090(21, 0, 0);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(4);
}
