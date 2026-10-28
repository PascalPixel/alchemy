#include "room.h"

void Func_0200006c(void) {
  struct SequenceParentWork *party = &gSequenceParentWork;

  if (party->entrance == 2) {
    Func_0200046c();
    Func_02000524(0);
    if (Actor_GetById(8)->x == 0) {
      Actor_CommitPositionAndActivate_(4, 0, -16);
      Func_0200052c_(8, 0, -16, 0x3000);
      Actor_WaitForMoveById(8);
      Actor_SetSpeedParameters_(8, 0x13333, 0x9999);
      Actor_SetPositionAndReset_(8, 472, 392);
      Func_020004f4_(8, 0x5000);
    } else {
      struct Actor *leader;

      Actor_CommitPositionAndActivate_(4, 0, 16);
      Actor_SetSpeedParameters_(8, 0x20000, 0x10000);
      Actor_SetPositionAndReset_(8, 440, 440);
      Actor_SetSpeedParameters_(8, 0x13333, 0x9999);
      Actor_SetModeById(8, 2);
      leader = Actor_GetById(party->currentOwner);
      if (leader != NULL)
        Actor_ResetAndSetPosition(8, S16_AT(leader, 10),
                                         S16_AT(leader, 18));
      Actor_WaitForMoveById(8);
      Func_020004bc(8, 0, 0);
    }
    Func_02000474();
  }
}

void Func_02000158(void) {
  struct SequenceParentWork *party;
  s32 id;
  s32 limit;
  s32 z;

  if (SCENE_WORK->unk166 != 0) {
    Actor_SetAngleToward(8, gSequenceParentWork.currentOwner, 0);
    Func_02000504_(8, 0x100, 30);
  }
  Func_020004dc((s32)MsgAreWeSetting);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Func_0200053c(), 0) == 0) {
    FieldEvent_Wait(15);
    Func_020004ec(8, 0);
    FieldEvent_Wait(5);
    Actor_Jump(8, 4, 0);
    Func_020004f4(8, 0);
    FieldEvent_Wait(5);
    party = &gSequenceParentWork;
    z = Actor_GetById(party->currentOwner)->z;
    /* FAKEMATCH: ⚓️ loads the leader's z before it builds the limit */
    asm volatile("" : "+l"(z));
    limit = 0x1880000;
    id = party->currentOwner;
    if (z < limit)
      Actor_SetPositionAndReset_(id, 472, 376);
    else
      Actor_SetPositionAndReset_(id, 472, 408);
    id = 4;
    /* FAKEMATCH: ⚓️ builds the id before the zero */
    asm volatile("" : "+l"(id));
    Func_020004f4(id, 0);
    FieldEvent_Wait(10);
    {
      struct SequenceParentWork *state = &gSequenceParentWork;

      /* FAKEMATCH: a one-pass block loads gSequenceParentWork before its offset is
       * built (one reordered row). */
      do {
      } while (0);
      state->ownerOverride = 1;
    }
    Func_0200050c(1);
  } else {
    FieldEvent_Wait(20);
    SCENE_WORK->unk1c4++;
    Func_020004ec(8, 0);
  }
}

void Func_02000254(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200046c();
  Func_02000524(0);
  Func_02000544(158);
  Func_02000454(warp->scene, warp->x, warp->z);
  Actor_GetById(gSequenceParentWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 2);
  id = gSequenceParentWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_Wait(10);
  Func_0200050c(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000474();
}

void Func_020002dc(s32 door) { Func_02000254(&gData_02000610, door, 0); }
