#include "contigo.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 PartyInventory_CountItem(s32 item);
s32 PartyInventory_CountFreeSlots(void);
void Func_0200422c(void);
void Func_0200403c(s32 value, s32 digits);
void Field_SetDestination(s32 scene, s32 entrance);

/// The lucky wheels attendant: checks the game tickets and free item slots, then
/// sends the player in.
void Func_02001938(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(228);
  s32 room;

  Func_0200422c();
  if (spoken == 0) {
    s32 message = (s32)MsgContigo10;

    Event_SetMessageId(message);
    Event_ShowMessageAndWait(18, 0);
    if (tickets == 0) {
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    Func_0200403c(tickets, 5);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Event_SetMessageId(message + 4);
      FieldMessage_ShowAtActor(18, 0);
    } else if (room <= 6) {
      Event_SetMessageId(message + 5);
      FieldMessage_ShowAtActor(18, 0);
    }
    if (room <= 6 &&
        Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
      Event_SetMessageId((s32)MsgContigo12);
      Event_ShowMessageAndWait(18, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Event_SetMessageId((s32)MsgContigo18);
      Event_ShowMessageAndWait(18, 0);
      return;
    }
    Event_SetMessageId((s32)MsgContigo19);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Event_SetMessageId((s32)MsgContigo17);
      Event_ShowMessageAndWait(18, 0);
      return;
    }
  }
  Event_SetMessageId((s32)MsgContigo13);
  Event_ShowMessageAndWait(18, 0);
  GameFlag_SetBit(265);
  GameFlag_SetBit(0x9e5);
  wait(1);
  Field_SetDestination(508, 0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 20);
}
