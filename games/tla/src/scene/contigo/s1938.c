#include "contigo.h"
#include "scene_ids.h"

s32 PartyInventory_CountItem(s32 item);
s32 PartyInventory_CountFreeSlots(void);
void Func_0200422c(void);
void Func_0200403c(s32 value, s32 digits);
void Field_SetDestination(s32 scene, s32 entrance);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);

/// The lucky wheels attendant: checks the game tickets and free item slots, then
/// sends the player in.
void Func_02001938(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(228);
  s32 room;

  Func_0200422c();
  if (spoken == 0) {
    s32 message = (s32)MsgContigo10;

    Func_0200412c(message);
    Func_02004144(18, 0);
    if (tickets == 0) {
      Func_02004144(18, 0);
      return;
    }
    Func_0200412c(message + 2);
    Func_0200403c(tickets, 5);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(gSequenceParentWork.currentOwner, 0) == 1) {
      Func_02004144(18, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Func_0200412c(message + 4);
      FieldMessage_ShowAtActor(18, 0);
    } else if (room <= 6) {
      Func_0200412c(message + 5);
      FieldMessage_ShowAtActor(18, 0);
    }
    if (room <= 6 &&
        Event_AskYesNoWithActorAnimation(gSequenceParentWork.currentOwner, 0) == 1) {
      Func_0200412c((s32)MsgContigo12);
      Func_02004144(18, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Func_0200412c((s32)MsgContigo18);
      Func_02004144(18, 0);
      return;
    }
    Func_0200412c((s32)MsgContigo19);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Func_0200412c((s32)MsgContigo17);
      Func_02004144(18, 0);
      return;
    }
  }
  Func_0200412c((s32)MsgContigo13);
  Func_02004144(18, 0);
  GameFlag_SetBit(265);
  GameFlag_SetBit(0x9e5);
  wait(1);
  Field_SetDestination(508, 0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 20);
}
