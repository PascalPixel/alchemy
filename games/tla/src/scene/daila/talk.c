#include "types.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

void Daila_TalkBeenBefore(void) {
  s32 msg;

  if (Import_GameFlagIsSet(0x843) == 0) {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    msg = (s32)MsgDailaBeenBefore;
    Import_EventSetMessageId(msg);
    Import_FieldMessageShowAtActor(8, 0);
    if (Import_EventAskYesNoWithActorAnimation(Import_PartyGetLeaderActor(), 0) == 0) {
      Import_EventWait(10);
      Import_EventSetMessageId(msg + 1);
    } else {
      Import_EventWait(20);
      Import_EventSetMessageId(msg + 2);
    }
    Import_EventShowMessageAndWait(8, 0);
    Import_GameFlagSet(0x843);
    Import_EventEnd();
  } else {
    Import_EventSetMessageId((s32)MsgDailaWaveNoInjuries);
    Import_EventShowMessageAndWait(8, 0);
  }
}

void Daila_TalkIndraMoved(void) {
  s32 msg;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaIndraMoved;
  Import_EventSetMessageId(msg);
  Import_FieldMessageShowAtActor(9, 0);
  if (Import_EventAskYesNoWithActorAnimation(Import_PartyGetLeaderActor(), 0) == 0) {
    Import_EventWait(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    Import_EventWait(20);
    Import_EventSetMessageId(msg + 2);
  }
  Import_EventShowMessageAndWait(9, 0);
  Import_EventEnd();
}

void Daila_TalkBoat(void) {
  s32 msg;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaWantBoat;
  Import_EventSetMessageId(msg);
  Import_FieldMessageShowAtActor(9, 0);
  if (Import_EventAskYesNoWithActorAnimation(Import_PartyGetLeaderActor(), 0) == 0) {
    Import_EventWait(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    Import_EventWait(20);
    Import_EventSetMessageId(msg + 2);
  }
  Import_EventShowMessageAndWait(9, 0);
  Import_EventEnd();
}

void Func_02000288(s32 keeper) {
  u16 facing = Import_ActorGet(gMainRoutineWork.currentOwner)->facing;

  /* FAKEMATCH: the loop note ends the scheduling block: the native loads the
   * facing before it builds the quadrant offset (2 differ without it). */
  do {
  } while (0);
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopOpen(3, keeper);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaTaviRikiWorry);
    Import_EventShowMessageAndWait(keeper, 0);
    Import_EventEnd();
  }
}
