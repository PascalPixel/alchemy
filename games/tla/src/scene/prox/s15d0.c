#include "../../../../games/tla/src/scene/prox/local.h"
#include "actor.h"

s32 Func_02004464();
s32 Func_020045ec();
s32 Actor_SetSpeedParameters();
s32 Actor_GetById();
s32 Actor_SetPositionAndReset();
s32 FieldEvent_Wait();
s32 Actor_ArmCallback();
s32 Func_0200463c();
s32 Func_02004584();
s32 Func_020044cc();
s32 Func_02004444();
s32 Func_0200455c();
s32 Func_02004404();
s32 Func_0200442c();
s32 GameFlag_SetBit();
s32 Func_0200446c();

void Func_020015d0(void) {
  struct Actor *object;

  Func_02004464();
  Call1(Func_020045ec, 0);
  Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0x20000,
        0x10000);
  Call3(Actor_SetSpeedParameters, 20, 0x20000, 0x10000);
  object = (struct Actor *)Value1(Actor_GetById, 20);
  object->terrainId = 1;
  Call2((void (*)())Actor_SetSpritePriority, 20, 3);
  object = (struct Actor *)Value1(Actor_GetById, gMainRoutineWork.currentOwner);
  object->actionFlags &= 0xfe;
  Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 0x198, 0x146);
  Call1(FieldEvent_Wait, 1);
  object = (struct Actor *)Value1(Actor_GetById, gMainRoutineWork.currentOwner);
  object->actionFlags |= 1;
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0xc000, 0);
  Call1(Func_0200463c, 0xa1);
  object = (struct Actor *)Func_02004584();
  object->y += 0x20000;
  object = (struct Actor *)Value1(Actor_GetById, 20);
  object->actionFlags &= 0xfe;
  Call3(Actor_SetPositionAndReset, 20, 0x198, 0xc8);
  Call1(FieldEvent_Wait, 1);
  object = (struct Actor *)Value1(Actor_GetById, 20);
  object->actionFlags |= 1;
  Call3(Func_020044cc, 20, 0, 0);
  Call1(Func_0200463c, 0xb0);
  Call3(Func_02004444, 0x10000, 0x20000, 0x10000);
  Call1(FieldEvent_Wait, 30);
  Call3(Func_02004444, -1, -1, 0xe666);
  Call1(Func_0200463c, 0x7f);
  Call1(FieldEvent_Wait, 30);
  Call1(Func_0200463c, 0x92);
  Call1(FieldEvent_Wait, 20);
  Call3(Func_0200455c, gMainRoutineWork.currentOwner, 0x105, 40);
  Call2((void (*)())Actor_SetSpritePriority, 20, 2);
  object = (struct Actor *)Value1(Actor_GetById, 20);
  object->priorityFlags |= 1;
  Call6(Func_02004404, 0x48, 3, 0x45, 3, 3, 3);
  Call6(Func_02004404, 0x30, 0x24, 0x45, 8, 3, 2);
  Call6(Func_0200442c, 0x48, 0xd, 1, 3, 0x46, 0x14);
  Call1(GameFlag_SetBit, 0xa2d);
  Func_0200446c();
}
