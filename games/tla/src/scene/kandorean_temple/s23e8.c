#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "event_inline.h"

extern u8 MsgKandoreanTemple08;

void Func_02005330(struct Actor *object, s32 mode);
void Func_02005148(struct Actor *object);

void Func_020023e8(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 180 of 260 instructions differ in
   * tla-en. */
  s16 angle = *(s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->angle;
  struct Actor *object;
  struct FieldSprite *sprite;

  Func_020051c0();
  FieldEvent_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = 0;
  ((void (*)())Actor_ArmCallback)(12, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple08);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 12, 0);
  Actor_SetAnimation(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimation(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, (u16)angle, 10);
  if (gData_020062ac != 0)
    Func_02005330(gData_020062ac, 3);
  Party_GiveItem(198, 0);
  if (gData_020062ac != 0)
    Func_02005148(gData_020062ac);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(0x800c, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(0x1a80000, 0, 0x1900000, 1);
  object = Actor_GetById(12);
  sprite = object->animation;
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 424, 376);
  FieldActor_SetSpritePriority(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  FieldActor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 444, 376);
  Actor_SetAnimation(12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  object->motionFlags = 0;
  object->y += 0x40000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetSpeed(12, 0x6666, 0x3333);
  FieldActor_SetMoveTarget(object, 0x1ec0000, 0x80000, 0x1780000);
  FieldActor_WaitForMoveById(12);
  object->motionFlags = 3;
  object->terrainHeight = 0;
  object->scaleX = 0x10000;
  sprite->rotation = 0;
  Actor_SetAnimation(12, 1);
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  FieldActor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 504, 376);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 496, 394);
  Actor_WalkToAndWait(12, 432, 394);
  FieldActor_WatchAndSetScript(12, gMainRoutineWork.currentOwner);
  FieldActor_SetSpritePriority(12, 2);
  Event_SetFlag(0x84a);
  Func_020051c8();
}
