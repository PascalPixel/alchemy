#include "types.h"
#include "inventory.h"
#include "yallam_interiors.h"
#include "version.h"
#include "scene_tables.h"
#include "event_inline.h"

s32 Func_02000a90(void);
void Func_02000a60(s32 actor, s32 angle);
void Func_02000a80(s32 actor);

s32 Func_020004e0(void) {
  s32 item;

  for (item = 429; item <= 437; item++) {
    if (PartyInventory_FindOwner(item) != -1)
      return 1;
  }
  for (item = 417; item <= 427; item++) {
    if (PartyInventory_FindOwner(item) != -1)
      return 1;
  }
  return 0;
}

/// Actor 14's talk. Flag 0x10f follows Func_020004e0's item check (it stays
/// set while flag 0x301 or Func_02000a90 holds). Once 0x10f is set and
/// neither holds, a yes moves actor 14 and the leader, and if Func_02000a90
/// then holds, places actor 16 and sets flag 0x301.
void Func_02000524(void) {
  s32 message;

  if (Func_020004e0())
    Event_SetFlag(0x10f);
  else if (!Event_GetFlag(0x301) && !Func_02000a90())
    GameFlag_ClearBit(0x10f);

  if (!Event_GetFlag(0x10f)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessage(14, 0);
  } else if (Event_GetFlag(0x301)) {
    Event_SetMessageId((s32)MsgICanTWait);
    Event_ShowMessage(14, 0);
  } else if (Func_02000a90()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    Event_ShowMessage(14, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(14, 0);
      Func_02000a60(14, 0xc000);
      Actor_SetSingleChildFlags(((struct YallamInteriorsObject *)Actor_GetById(14)), 1);
      Actor_SetDestination(gMainRoutineWork.currentOwner, 488, 408);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
      Actor_SetDestination(14, 504, 408);
      Actor_WaitForMoveById(14);
      Actor_SetAngleToward(14, gMainRoutineWork.currentOwner, 0);
      Func_02000a80(14);
      if (Func_02000a90()) {
        Actor_SetSpeed(14, 0x6666, 0x3333);
        Actor_WalkTo(14, 568, 408);
        Actor_WaitForMoveById(14);
        Actor_WalkTo(14, 568, 360);
        Actor_WaitForMoveById(14);
        Actor_WalkTo(16, 632, 346);
        Actor_FaceDirection(16, FACING_SOUTH, 0);
        Actor_SetModeById(16, 0);
        Actor_ResetAndSetPositionInMode2(14, 600, 360);
        Actor_WaitForMoveById(14);
        Actor_SetModeById(14, 0);
        Actor_ArmCallback(14, FACING_SOUTH, 0);
        ((struct YallamInteriorsObject *)Actor_GetById(14))->collisionFlags = 129;
        ((struct YallamInteriorsObject *)Actor_GetById(14))->priorityFlags = 1;
        Actor_SetSpritePriority(14, 1);
        ((struct YallamInteriorsObject *)Actor_GetById(14))->priorityFlags |= 1;
#if TLA_PATCH_1
        Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
#endif
        GameFlag_SetBit(0x301);
      } else {
        Actor_ResetAndSetPosition(14, 491, 398);
        Actor_WaitForMoveById(14);
        Actor_ArmCallback(14, FACING_SOUTH, 0);
        Actor_SetSingleChildFlags(((struct YallamInteriorsObject *)Actor_GetById(14)), 0);
        Func_02000a60(14, 0);
      }
    } else {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(14, 0);
    }
  }
}
