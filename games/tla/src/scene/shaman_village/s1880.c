#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"

extern const u8 gData_02004c5c[];
extern const u8 gData_02004cac[];

void Func_02001880(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner));
  shaman = ((struct ShamanVillageObject *)Actor_GetById(15));
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetModeById(16, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(16, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetModeById(15, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 15, 0);
  Actor_FaceDirection(17, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Motion_SetModeAndWaitAnimation(17, 3);
  Actor_SetModeById(17, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(17, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessageAndWait(15, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(200), 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004c5c);
  Actor_SetMovementTypeById(15, gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  Func_02003040();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(168), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 184, 128);
  Actor_WalkToAndWait(15, 264, 128);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020048b4();
  Import_EventSetExitRequest(11);
}

void Func_02001aa4(void) {
  struct ShamanVillageObject *owner;
  struct ShamanVillageObject *shaman;
  struct ShamanVillageObject *target;

  owner = ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner));
  shaman = ((struct ShamanVillageObject *)Actor_GetById(15));
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Actor_Stop(16);
  Actor_Stop(17);
  Actor_Stop(15);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(17, 3);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_SetModeById(17, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(17, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(17);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetModeById(15, 3);
  Event_SetMessageId((s32)MsgShamanGuardsTakeOtherPath);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 15, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetModeById(16, 2);
  target = ((struct ShamanVillageObject *)Actor_GetById(15));
  if (target != NULL)
    Actor_ResetAndSetPosition(16, ((s16 *)target)[5],
                                     ((s16 *)target)[9]);
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Event_ShowMessageAndWait(15, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Camera_SetSpeedParameters(0x19999, 0x3333);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(200), 1);
  owner->unk63 = 0;
  shaman->unk63 = 0;
  Actor_SetMovementTypeById(15, gData_02004c5c);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004cac);
  do {
    wait(1);
  } while (owner->unk63 == 0 || shaman->unk63 == 0);
  Func_02003040();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(168), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_ResetAndSetPositionInMode2(15, 184, 128);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 128);
  Actor_SetModeById(15, 1);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020048b4();
  Import_EventSetExitRequest(12);
}
