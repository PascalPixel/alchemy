#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field_sprite.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "madra_interiors.h"

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void FieldEvent_Wait();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Field_DispatchTypeHandler();
void Func_02001d50();
void Func_02001d58();
void Func_02001db8();
void Func_02001e08();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();
void Func_02001eb8();
void Func_02001ec8();
void Func_02001ef0();
void Func_02001f10();
void GameFlag_SetBit();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_SetSingleChildFlags();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Stop();
void Actor_SetMovementTypeById();
void Actor_Jump();
void Actor_OffsetPositionAndResetMotion();
void Actor_ResetAndSetPosition();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_WaitForAnimationChange();
void Camera_FollowActor();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();
void Actor_TurnToFaceEachOther();
void Actor_RefreshSelectorById();
void Actor_SetModeById();
void FieldMessage_ShowAtActor();

s32 Scene_PrepareMap(void) { return 0; }

s32 Func_020007ac(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->animation;
  if (object != NULL && sprite != NULL)
    sprite->rotation -= 0x800;
  return 0;
}

s32 Func_020007c8(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->animation;
  if (object != NULL && sprite != NULL)
    sprite->rotation = 0;
  return 0;
}

void Func_020007dc(void) {
  s32 i;
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;
  struct Actor *object8;
  struct Actor *object9;
  struct Actor *object10;
  struct Actor *object11;
  struct Actor *object12;
  s32 result13;
  struct Actor *object14;

  ((void (*)())Func_02001d50)();
  Call1((void (*)())Func_02001eb8, 0);
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call3((void (*)())Func_02001db8, 4, 0x1900000, 0x3780000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1980000, -1, 0x3500000, 1);
  ((void (*)())Event_StartScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0xcccc, 0x6666);
  Call1((void (*)())Func_02001e18, (s32)MsgMadraInteriors28);
  Call3((void (*)())Func_02001e28, 10, 0, 10);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 0x18c, 0x365);
  Call4((void (*)())Func_02001ec8, 11, 25, 19, 0xc000);
  Call1((void (*)())Actor_RefreshSelectorById, 11);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0x8000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0xe000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x3340000, 1);
  ((void (*)())Func_02001e68)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_02001e48, 9, 0x107, 30);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Call2((void (*)())Func_02001e40, 7, 0xc000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, 0, -16);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 9, 0x103, 0);
  object0 = Actor_GetById(9);
  object0->actionFlags &= 254;
  for (i = 2; i >= 0; i--) {
    Call3((void (*)())Actor_CommitPositionAndActivate, 9, 0, -8);
    Call1((void (*)())Func_02001f10, 134);
    Call3((void (*)())Actor_CommitPositionAndActivate, 9, 0, 8);
  }
  object1 = Actor_GetById(9);
  object1->actionFlags |= 1;
  Call1((void (*)())FieldEvent_Wait, 30);
  Value3((s32 (*)())Func_02001e48, 7, 0x102, 30);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Value3((s32 (*)())Func_02001e48, 7, 0x107, 30);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 9, 0x108, 30);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Call3((void (*)())Actor_SetAngleToward, 10, 9, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call3((void (*)())Func_02001e28, 10, 0, 10);
  Call3((void (*)())Actor_SetAngleToward, 9, 10, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 9, 0x107, 20);
  Call2((void (*)())Func_02001e40, 9, 0xc000);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 9, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
  Call3((void (*)())Func_02001e28, 10, 0, 10);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, 0, 16);
  Call2((void (*)())Func_02001e90, 24, 1);
  Call2((void (*)())Func_02001e98, 7, 8);
  ((void (*)())Func_02001eb0)();
  Value3((s32 (*)())Func_02001e48, 9, 0x101, 30);
  Call3((void (*)())Actor_SetAngleToward, 9, 7, 0);
  Call3((void (*)())Actor_SetAngleToward, 10, 7, 0);
  object2 = Actor_GetById(9);
  Call2((void (*)())Actor_SetSingleChildFlags, (s32)object2, 0);
  Value2((s32 (*)())Actor_SetMovementTypeById, 9, gData_0200349c);
  Call1((void (*)())Field_DispatchTypeHandler, 1);
  ((void (*)())Func_02001ea0)();
  ((void (*)())Func_02001ea8)();
  Call3((void (*)())Func_02001e28, 11, 0, 10);
  Call1((void (*)())Actor_Stop, 9);
  object3 = Actor_GetById(9);
  object3->scaleX = 0x10000;
  object4 = Actor_GetById(9);
  object4->scaleY = 0x10000;
  Call1((void (*)())FieldEvent_Wait, 4);
  object5 = Actor_GetById(9);
  object5->motionFlags = 2;
  object6 = Actor_GetById(9);
  object6->actionFlags &= 254;
  Call3((void (*)())Actor_OffsetPositionAndResetMotion, 9, -8, 0);
  Value2((s32 (*)())Actor_SetMovementTypeById, 9, gData_02003418);
  Call1((void (*)())Actor_RefreshSelectorById, 9);
  Call1((void (*)())Func_02001f10, 127);
  Call1((void (*)())FieldEvent_Wait, 60);
  object7 = Actor_GetById(9);
  object7->actionFlags = (object7->actionFlags | 1);
  Call3((void (*)())Actor_Jump, 9, 6, 0);
  Call1((void (*)())FieldEvent_Wait, 3);
  Call2((void (*)())Func_02001ef0, 9, 0);
  object8 = Actor_GetById(9);
  Call2((void (*)())Actor_SetSingleChildFlags, (s32)object8, 1);
  Call3((void (*)())Actor_ArmCallback, 9, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 10, 0xc000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_02001e48, 10, 0x100, 30);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_Jump, 9, 6, 20);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetAngleToward, 9, 7, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  object9 = Actor_GetById(10);
  object9->actionFlags &= 254;
  object10 = Actor_GetById(9);
  object10->actionFlags &= 254;
  Call3((void (*)())Actor_CommitPositionAndActivate, 9, -16, 16);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 9, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_Jump, 9, 6, 20);
  Call1((void (*)())FieldEvent_Wait, 20);
  object11 = Actor_GetById(9);
  object11->actionFlags = (object11->actionFlags | 1);
  Value2((s32 (*)())Actor_SetMovementTypeById, 9, gData_020033c8);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 9);
  Call2((void (*)())Actor_WatchAndSetScript, 11, 9);
  Call3((void (*)())Func_02001e28, 9, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetAngleToward, 10, 7, 0);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 10);
  Call2((void (*)())Actor_WatchAndSetScript, 11, 10);
  Call3((void (*)())Func_02001e28, 10, 0, 10);
  Value3((s32 (*)())Func_02001e48, 7, 0x105, 30);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
  Call3((void (*)())Actor_CommitPositionAndActivate, 10, -16, 16);
  Call3((void (*)())Func_02001e28, 10, 0, 1);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_CommitPositionAndActivate, 10, -16, 8);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02001e28, 10, 0, 10);
  object12 = Actor_GetById(10);
  object12->actionFlags |= 1;
  Value2((s32 (*)())Actor_SetMovementTypeById, 10,
         gData_020033c8);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call1((void (*)())Actor_Stop, 4);
  Call1((void (*)())Actor_Stop, 11);
  Call1((void (*)())Actor_RefreshSelectorById, 10);
  Call3((void (*)())Func_02001db8, 9, 0, 0);
  Call3((void (*)())Func_02001db8, 10, 0, 0);
  Call2((void (*)())Actor_SetModeById, 8, 6);
  Call2((void (*)())Func_02001e08, 8, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
  Call1((void (*)())Actor_WaitForAnimationChange, 8);
  Call2((void (*)())Camera_FollowActor, 4, 1);
  ((void (*)())Func_02001e68)();
  Call3((void (*)())Actor_TurnToFaceEachOther, 11, 4, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 11, 0);
  result13 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (result13 != 0)
    goto L_02008e40;
  Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
  Call3((void (*)())Func_02001e28, 11, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_02008e64;
L_02008e40:
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 4);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Call3((void (*)())Func_02001e28, 11, 0, 10);
L_02008e64:
  Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02001e28, 11, 0, 10);
  Call2((void (*)())Actor_SetModeById, 4, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetModeById, 11, 2);
  object14 = Actor_GetById(4);
  if (object14 == 0)
    goto L_02008eb2;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 11,
         *(s16 *)((u8 *)object14 + 10), *(s16 *)((u8 *)object14 + 18));
L_02008eb2:
  Call1((void (*)())Actor_WaitForMoveById, 11);
  Call3((void (*)())Func_02001db8, 11, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0x911);
  ((void (*)())Func_02001d58)();
  return;
}

void Func_02000ed8(void) {
  ((void (*)())Func_02001d50)();
  Call1((void (*)())Func_02001eb8, 0);
  Call1((void (*)())Func_02001e18, (s32)MsgMadraInteriors29);
  Call3((void (*)())Actor_ArmCallback, 11, 0x4000, 0);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  ((void (*)())Func_02001d58)();
  return;
}

void Func_02000f0c(void) {
  ((void (*)())Func_02001d50)();
  Call1((void (*)())Func_02001eb8, 0);
  Call1((void (*)())Func_02001e18, (s32)MsgMadraInteriors30);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 7, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 7, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0x4000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02001e28, 7, 0, 10);
  ((void (*)())Func_02001d58)();
  return;
}

void Func_02000fe0(void) {
  Call2((void (*)())Func_02001e90, 24, 1);
  Call2((void (*)())Func_02001e98, 7, 8);
  ((void (*)())Func_02001eb0)();
  Call1((void (*)())Field_DispatchTypeHandler, 1);
  ((void (*)())Func_02001ea0)();
  ((void (*)())Func_02001ea8)();
  return;
}
