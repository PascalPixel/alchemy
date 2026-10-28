#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"

void FieldEvent_WaitFrames();
void Func_0200282c();
void Func_02002834();
void Func_02002874();
void Func_020028ac();
void Func_020028bc();
void Func_020028dc();
void Func_020028e4();
void Func_020028fc();
void Func_02002904();
void GameFlag_SetBit();
void Event_AskYesNoWithActorAnimation();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_CommitPositionAndActivate();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_Launch();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetVariantCallback();
struct ObjectRuntime *Object_GetById();
void Object_LinkObjectAndSetCallback();
void Object_SetModeById();
void Party_RemoveActiveOwner();
void FieldMessage_ShowAtActor();

s32 Func_02000284(void) {
  struct ObjectRuntime *object;

#if (LANGUAGE == LANGUAGE_JAPANESE)
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
#else
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
#endif
  ObjectMotion_SetActionVariant(11, 1);
  ObjectMotion_SetActionVariant(14, 2);
  ObjectMotion_SetActionVariant(10, 2);
  object = Object_GetById(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  object = Object_GetById(17);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  if (GameFlag_Test(0x966)) {
    ObjectMotion_SetPositionAndReset(10, 100, 112);
    Object_GetById(10)->angle = 0x3000;
    WaitFrames(1);
  }
  if (GameFlag_Test(0x969))
    Func_02002874(18, 0, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200031c(void) {
  struct ObjectRuntime *object0;
  struct ObjectRuntime *object1;
  s32 result2;
  struct ObjectRuntime *object3;

  Call1((void (*)())GameFlag_SetBit, 0x965);
  Call1((void (*)())Party_RemoveActiveOwner, 7);
  ((void (*)())Func_0200282c)();
  Call1((void (*)())Func_020028fc, 0);
  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors07);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 112, 128);
  Call4((void (*)())Func_02002904, 12, 16, 0, 0xb000);
  Call4((void (*)())Func_02002904, 7, 16, -16, 0x8000);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xc000, 0);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 7);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x100, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_Launch, 10, 6, 15);
  Call3((void (*)())ObjectMotion_Launch, 10, 6, 23);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 12, 0x100, 45);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0xb000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x2000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 30);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0x8000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0xb000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 12, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 45);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x102, 40);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_020028dc, 7, 0x100, 45);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x105, 60);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, 48, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, -24, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x2000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x6000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x2000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Value3((s32 (*)())Func_020028dc, 7, 0x101, 30);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 7, 0x102, 40);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 12, 0x100, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0xd000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x6000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 7, 0x109, 30);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0x8000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 50);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0xb000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 7, 3);
  Call2((void (*)())Func_020028e4, 7, 0x102);
  Value1((s32 (*)())FieldEvent_WaitFrames, 50);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, -12, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x109, 30);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0xb000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_020028bc, 10, 0, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Object_SetModeById, 7, 9);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0xd000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call2((void (*)())Object_SetModeById, 7, 1);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 7, 0x4ccc, 0x2666);
  object0 = Object_GetById(7);
  object0->actionFlags &= 254;
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, 12, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 1);
  object1 = Object_GetById(7);
  object1->actionFlags |= 1;
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 7, 3);
  Call2((void (*)())Func_020028e4, 7, 0x102);
  Value1((s32 (*)())FieldEvent_WaitFrames, 55);
  Call2((void (*)())Object_LinkObjectAndSetCallback, 4, 7);
  Call2((void (*)())Object_LinkObjectAndSetCallback, 12, 7);
  Call2((void (*)())Object_LinkObjectAndSetCallback, 10, 7);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 7, 0x23333, 0x11999);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 7, -8, 88);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 4);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 12);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 10);
  Call3((void (*)())Func_02002874, 7, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_020028dc, 12, 0x109, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 12, 0x8000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200c, 0);
  result2 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (result2 != 0)
    goto L_020089be;
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_020089e4;
L_020089be:
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
L_020089e4:
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 12, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call2((void (*)())Object_SetModeById, 12, 2);
  object3 = Object_GetById(gAdventure.currentOwner);
  if (object3 == 0)
    goto L_02008a36;
  Value3((s32 (*)())ObjectMotion_ResetAndSetPosition, 12,
         *(s16 *)((u8 *)object3 + 10), *(s16 *)((u8 *)object3 + 18));
L_02008a36:
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 12);
  Call3((void (*)())Func_02002874, 12, 0, 0);
  ((void (*)())Func_02002834)();
  return;
}
