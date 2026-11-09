#include "local.h"
#include "event_work.h"
#include "version.h"
#include "event_inline.h"

extern u8 MsgKandoreanTemple07;
extern const u8 gData_02005564[];
extern const u8 gData_020055a0[];
extern const u8 gData_020055e4[];

void Func_02000bcc(void);
void Func_020047f0(void);

#define WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

void Func_0200482c(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *flags;

  Func_020051c0();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  wait(1);
  Actor_SetPositionAndFacing(16, 0x1780000, 0x1880000, 0);
  Actor_SetHorizontalPositionAndAngleWithTerrain(gMainRoutineWork.currentOwner,
                                                 0x1780000, 0x1980000, 0);
  {
    s32 angle = 0xb000;
    Actor_SetPositionAndFacing(12, 0x1d80000, 0x1c60000, angle);
  }
  WORK->screenEffect = 0x202;
  Event_StartScreenTransition();
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 464, 428);
  Actor_WalkToAndWait(12, 432, 408);
  Actor_FaceDirection(12, 0x8000, 0);
  Func_080d489c(12, 0x102);
  Event_SetMessageId((s32)&MsgKandoreanTemple07);
  Event_ShowMessageAndWait(12, 0);
  Func_080d47b4(12, 0x105, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_ArmCallback(12, 0, 40);
  Event_ShowMessageAndWait(12, 0);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetVarCbAndRefresh(12, 2);
  Actor_FaceDirection(12, 0x8000, 20);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(16, 0x3000, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Event_OpenMessage(16, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(12, 4);
    Event_ShowMessageAndWait(12, 0);
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Actor_SetModeById(12, 3);
    Event_ShowMessageAndWait(12, 0);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_ShowEmote(16, 0x101, 20);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetAttachedEffect(12, 0x102);
  Event_ShowMessageAndWait(12, 0);
  Actor_SetModeById(16, 3);
  Event_ShowMessageThenWait(16, 0, 40);
  Actor_FaceDirection(12, 0x5000, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 40);
  Actor_FaceDirection(12, 0x8000, 20);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(12, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Func_080d47b4(16, 0x101, 0);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(0x1a80000, 0x100000, 0x1800000, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005564);
  Actor_SetMovementTypeById(16, gData_020055a0);
  Actor_SetMovementTypeAndWaitById(12, gData_020055e4);
  Event_Wait(40);
  Func_02000bcc();
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 20);
  Event_OpenMessage(0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_020047f0();
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Func_020047f0();
  }
  Actor_ArmCallback(16, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_ShowMessage(0x400c, 0);
  Func_080d47b4(16, 0x100, 40);
  Event_ShowMessage(0x5010, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 0);
  Event_OpenMessage(0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(12, 3);
    Event_ShowMessageAndWait(0x400c, 0);
    Actor_ArmCallback(16, 0, 0);
    Func_080d47b4(16, 0x107, 20);
    Event_ShowMessageAndWait(0x5010, 0);
    WORK->messageId += 2;
  } else {
    WORK->messageId += 2;
    Actor_SetModeById(12, 4);
    Event_ShowMessageAndWait(0x400c, 0);
    Actor_ArmCallback(16, 0, 0);
    Func_080d47b4(16, 0x107, 20);
    Event_ShowMessageAndWait(0x5010, 0);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  FieldCamera_MoveTo(0x1a80000, 0, 0x1900000, 1);
  FieldCamera_SetSpeed(0x9999, 0x1333);
  Actor_SetSpeed(12, 0x9999, 0x4ccc);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 12);
  Actor_WatchAndSetScript(16, 12);
  Actor_WalkToAndWait(12, 488, 416);
  Actor_WalkToAndWait(12, 472, 432);
  Actor_FaceDirection(12, 0x5000, 0);
  Event_ShowMessage(0x400c, 0);
  Event_ShowMessageThenWait(0x1010, 0, 80);
  Func_080d47b4(12, 0x108, 0);
  Actor_FaceDirection(12, 0xb000, 20);
  Event_ShowMessage(0x400c, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(0x400c, 0);
  FieldCamera_MoveTo(0x1a80000, 0x100000, 0x1800000, 1);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 488, 416);
  Actor_WalkToAndWait(12, 504, 376);
  Actor_ArmCallback(12, 0x8000, 20);
  object = Actor_GetById(12);
  sprite = object->animation;
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 492, 376);
  Actor_SetModeById(12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  /* FAKEMATCH(temporary): the motion flags written through a byte pointer, so
   * the store may alias the rotation and stays after it; as
   * object->motionFlags, which keeps the struct's alias set, 51 of 783
   * instructions differ in tla-en. */
  flags = &object->motionFlags;
  *flags = 0;
  object->y += 0x40000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetSpeed(12, 0x6666, 0x3333);
  Actor_SetMoveTarget((s32)object, 0x1bc0000, 0x280000, 0x1780000);
  Actor_WaitForMoveById(12);
  *flags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  sprite->rotation = 0;
  object->scaleX = 0x10000;
  object->terrainHeight = 0x200000;
  Actor_SetModeById(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 424, 376);
  Camera_MoveWithinMapBounds(0x1980000, 0x100000, 0x1900000, 1);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 392, 360);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetSpritePriority(12, 2);
  Actor_Stop(16);
  Actor_Stop(gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetModeById(16, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if (leader != NULL)
      Actor_ResetAndSetPosition(16, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
  }
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  GameFlag_SetBit(0x849);
#if TLA_PATCH_1
  WORK->screenEffect = 0x209;
#endif
  Func_020051c8();
}
