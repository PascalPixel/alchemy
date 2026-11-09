#include "room.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_0200006c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->entrance == 2) {
    Func_0200046c();
    Event_ResolvePendingActions(0);
    if (Actor_GetById(8)->x == 0) {
      Actor_WalkByAndWait(4, 0, -16);
      FieldActor_LaunchFromLeader(8, 0, -16, FACING_SOUTHEAST + FACING_STEP);
      Actor_WaitForMoveById(8);
      Actor_SetSpeed(8, 0x13333, 0x9999);
      Actor_WalkToAndWait(8, 472, 392);
      Actor_FaceDirectionAndWait(8, FACING_SOUTH + FACING_STEP);
    } else {
      struct Actor *leader;

      Actor_CommitPositionAndActivate(4, 0, 16);
      Actor_SetSpeed(8, 0x20000, 0x10000);
      Actor_WalkToAndWait(8, 440, 440);
      Actor_SetSpeed(8, 0x13333, 0x9999);
      Actor_SetModeById(8, 2);
      leader = Actor_GetById(party->currentOwner);
      if (leader != NULL)
        Actor_ResetAndSetPosition(8, S16_AT(leader, 10), S16_AT(leader, 18));
      Actor_WaitForMoveById(8);
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    }
    Func_02000474();
  }
}

void Func_02000158(void) {
  struct MainRoutineWork *party;
  s32 id;
  s32 limit;
  s32 z;

  if (SCENE_WORK->unk166 != 0) {
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    Actor_ShowEmote(8, 0x100, 30);
  }
  Event_SetMessageId((s32)MsgAreWeSetting);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(15);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(5);
    Actor_Jump(8, 4, 0);
    Actor_ArmCallbackAndRefresh(8, FACING_EAST);
    Event_Wait(5);
    party = &gMainRoutineWork;
    z = Actor_GetById(party->currentOwner)->z;
    limit = 0x1880000;
    id = party->currentOwner;
    if (z < limit)
      Actor_SetPositionAndReset(id, 472, 376);
    else
      Actor_SetPositionAndReset(id, 472, 408);
    id = 4;
    Actor_ArmCallbackAndRefresh(id, FACING_EAST);
    Event_Wait(10);

    gMainRoutineWork.ownerOverride = 1;
    Import_EventSetExitRequest(1);
  } else {
    Event_Wait(20);
    SCENE_WORK->unk1c4++;
    Event_ShowMessageAndWait(8, 0);
  }
}

void Func_02000254(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200046c();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000474();
}

void Func_020002dc(s32 door) { Func_02000254(&gData_02000610, door, 0); }
