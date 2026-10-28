#include "types.h"
#include "field_scene.h"
#include "actor.h"
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
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Stop();
void Actor_Jump();
void Actor_ResetAndSetPosition();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_StartRepeatedMotion();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();
void Actor_SetModeById();
void Party_RemoveActiveOwner();
void FieldMessage_ShowAtActor();

s32 Func_02000284(void) {
  struct Actor *object;

#if (LANGUAGE == LANGUAGE_JAPANESE)
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
#else
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
#endif
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(14, 2);
  Actor_SetSpritePriority(10, 2);
  object = Actor_GetById(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  object = Actor_GetById(17);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  if (GameFlag_Test(0x966)) {
    Actor_SetPositionAndReset(10, 100, 112);
    Actor_GetById(10)->angle = 0x3000;
    WaitFrames(1);
  }
  if (GameFlag_Test(0x969))
    Func_02002874(18, 0, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200031c(void) {
  struct Actor *object0;
  struct Actor *object1;
  s32 result2;
  struct Actor *object3;

  Call1((void (*)())GameFlag_SetBit, 0x965);
  Call1((void (*)())Party_RemoveActiveOwner, 7);
  ((void (*)())Func_0200282c)();
  Call1((void (*)())Func_020028fc, 0);
  Call1((void (*)())Func_020028ac, (s32)MsgLemuriaInteriors07);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 112, 128);
  Call4((void (*)())Func_02002904, 12, 16, 0, 0xb000);
  Call4((void (*)())Func_02002904, 7, 16, -16, 0x8000);
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call1((void (*)())Actor_WaitForMoveById, 7);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x100, 30);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_Jump, 10, 6, 15);
  Call3((void (*)())Actor_Jump, 10, 6, 23);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 12, 0x100, 45);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 12, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 10, 0x2000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 30);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())Actor_ArmCallback, 12, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 45);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x102, 40);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_020028dc, 7, 0x100, 45);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 35);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x105, 60);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, 48, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, -24, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0x2000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x6000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x2000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Value3((s32 (*)())Func_020028dc, 7, 0x101, 30);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 7, 0x102, 40);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 12, 0x100, 30);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xd000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0x6000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 7, 0x109, 30);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 50);
  Call3((void (*)())Actor_ArmCallback, 12, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0xa000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_StartRepeatedMotion, 7, 3);
  Call2((void (*)())Func_020028e4, 7, 0x102);
  Value1((s32 (*)())FieldEvent_WaitFrames, 50);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, -12, 0);
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
  Call3((void (*)())Actor_ArmCallback, 10, 0xb000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_020028bc, 10, 0, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Actor_SetModeById, 7, 9);
  Value1((s32 (*)())FieldEvent_WaitFrames, 40);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xd000, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 7, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 10, 0, 10);
  Call2((void (*)())Actor_SetModeById, 7, 1);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x4ccc, 0x2666);
  object0 = Actor_GetById(7);
  object0->actionFlags &= 254;
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, 12, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 1);
  object1 = Actor_GetById(7);
  object1->actionFlags |= 1;
  Value1((s32 (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Actor_StartRepeatedMotion, 7, 3);
  Call2((void (*)())Func_020028e4, 7, 0x102);
  Value1((s32 (*)())FieldEvent_WaitFrames, 55);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 7);
  Call2((void (*)())Actor_WatchAndSetScript, 12, 7);
  Call2((void (*)())Actor_WatchAndSetScript, 10, 7);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x23333, 0x11999);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, -8, 88);
  Call1((void (*)())Actor_Stop, 4);
  Call1((void (*)())Actor_Stop, 12);
  Call1((void (*)())Actor_Stop, 10);
  Call3((void (*)())Func_02002874, 7, 0, 0);
  Value1((s32 (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_020028dc, 12, 0x109, 30);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
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
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 12, 2);
  object3 = Actor_GetById(gAdventure.currentOwner);
  if (object3 == 0)
    goto L_02008a36;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 12,
         *(s16 *)((u8 *)object3 + 10), *(s16 *)((u8 *)object3 + 18));
L_02008a36:
  Call1((void (*)())Actor_WaitForMoveById, 12);
  Call3((void (*)())Func_02002874, 12, 0, 0);
  ((void (*)())Func_02002834)();
  return;
}
