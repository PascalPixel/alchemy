#include "shaman_village.h"
#include "scene_tables.h"
#include "event_inline.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

void Actor_WaitForAnimationChange(s32 actor);
void Func_02003b24(void);

extern const u8 gData_02004f3c[];
extern const u8 gData_02004f80[];
extern const u8 gData_02004fc4[];
extern const u8 gData_02005008[];
extern const u8 gData_0200504c[];
extern const u8 gData_020050bc[];

void Func_020030bc(void) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ShamanVillageObject *object = ((struct ShamanVillageObject *)Actor_GetById(16));

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  work[34] = 0;
  work[35] = 0;
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Sound_Play(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(528), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 168, 536);
  Func_02004944(18, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(5, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004f80);
  Actor_SetMovementTypeById(5, gData_02004fc4);
  Actor_SetMovementTypeById(18, gData_02004f3c);
  Actor_SetMovementTypeAndWaitById(7, gData_02005008);
  Actor_ArmCallbackAndRefresh(5, 0);
  Event_SetMessageId((s32)MsgShamanCheerForCompanion);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(0x2006, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  Actor_ArmCallbackAndRefresh(6, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 168, 524);
  Actor_FaceDirection(15, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Func_02003b24();
  Sound_Play(29);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(540), 1);
  Func_020049f4();
  Event_Wait(20);
  Actor_SetAttachedEffect(15, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAttachedEffect(17, 258);
  Event_ShowMessage(0x8011, 0);
  Actor_SetAttachedEffect(16, 258);
  FieldEvent_ShowMessageThenWait(0x8010, 0, 20);
  Motion_SetVarCbAndRefresh(15, 1);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(0x200f, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 0)
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessageAndWait(15, 0);
  Actor_Jump(6, 4, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(0x4007, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_SetModeById(15, 4);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ArmCallback(18, 0, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 256, 20)
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessageAndWait(0x200f, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 257, 40)
  Event_ShowMessageAndWait(0x4012, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(16, 256, 0)
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(17, 1);
  Event_ShowMessageAndWait(17, 0);
  Actor_StartRepeatedMotion(16, 1);
  Event_ShowMessageAndWait(16, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST);
  Actor_SetModeById(17, 4);
  Event_ShowMessageAndWait(0x8011, 0);
  Actor_FaceDirectionAndWait(16, FACING_NORTHEAST);
  Actor_SetModeById(16, 4);
  Event_ShowMessageAndWait(0x8010, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(10);
  Actor_SetModeById(16, 3);
  Motion_SetModeAndWaitAnimation(17, 3);
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(560), 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_WalkTo(16, 120, 560);
  Actor_WalkToAndWait(17, 216, 560);
  Actor_SetModeById(17, 1);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(16, 0);
  Actor_WalkTo(16, 152, 560);
  Actor_SetPositionAndReset(17, 184, 560);
  Actor_SetModeById(16, 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetSpeed(15, 0x10000, 0x8000);
  Actor_SetModeById(15, 2);
  Actor_SetModeById(17, 2);
  Actor_SetModeById(16, 2);
  Actor_OffsetPositionAndResetMotion(15, 0, 16);
  Actor_OffsetPositionAndResetMotion(17, 0, 16);
  Actor_OffsetPositionAndResetMotion(16, 0, 16);
  Actor_WaitForMoveById(15);
  Actor_SetModeById(15, 1);
  Actor_SetModeById(17, 1);
  Actor_SetModeById(16, 1);
  Actor_FaceDirectionAndWait(18, 0x3000);
  Event_ShowMessageAndWait(18, 0);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 265, 20)
  Event_ShowMessageAndWait(18, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 257, 0)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(16, 257, 20)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 264, 40)
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 20)
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(15, FACING_NORTH, 20);
  Motion_SetModeAndWaitAnimation(15, 4);
  Event_Wait(20);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 262, 0)
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(17, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(17, 0);
  Actor_ArmCallbackAndRefresh(16, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessageAndWait(16, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 40);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTH, 40);
  Actor_ArmCallbackAndRefresh(15, FACING_NORTHEAST);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 265, 40)
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(15, 4);
  FieldMessage_ShowAtActor(15, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_ArmCallback(18, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  done = &object->unk63;
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Motion_SetModeAndWaitAnimation(15, 4);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Motion_SetModeAndWaitAnimation(15, 4);
    EVENT_WORK->field1c4++;
    Event_ShowMessageAndWait(15, 0);
    Actor_SetModeById(15, 3);
    Event_ShowMessageAndWait(15, 0);
  }
  Actor_WalkToAndWait(15, 212, 582);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(500), 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(6, 15);
  *done = 0;
  Actor_SetMovementTypeById(15, gData_0200504c);
  Event_Wait(20);
  Actor_SetMovementTypeById(17, gData_0200504c);
  Actor_SetMovementTypeById(16, gData_0200504c);
  Sound_Play(78);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Func_02004a2c();
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(540), 1);
  Event_Wait(20);
  angle = 0x4000;
  Actor_SetPositionAndFacing(15, PIXELS(224), PIXELS(280), angle);
  Actor_SetPositionAndFacing(16, PIXELS(208), PIXELS(264), angle);
  Actor_SetPositionAndFacing(17, PIXELS(240), PIXELS(264), angle);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(18, 0, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_ArmCallbackAndRefresh(6, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetModeById(18, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(18))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(5))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(7))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(6))->update = NULL;
  wait(1);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetMovementTypeById(7, gData_020050bc);
  Actor_SetMovementTypeById(6, gData_020050bc);
  Actor_SetMovementTypeById(5, gData_020050bc);
  Actor_SetMovementTypeAndWaitById(18, gData_020050bc);
  GameFlag_SetBit(0x94c);
  Func_020048cc();
}
