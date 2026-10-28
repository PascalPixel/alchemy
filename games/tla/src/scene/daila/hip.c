#include "types.h"
#include "field_event.h"
#include "adventure.h"
#include "text/msg_ids.h"

void Daila_TalkHip(void) {
  s32 msg;

  Engine_EventStart();
  Engine_EventResolvePendingActions(0);
  msg = (s32)MsgDailaHurtHip;
  Engine_EventSetMessage(msg);
  Engine_EventOpenMessage(12, 0);
  if (Engine_EventChooseYesNo(Engine_PartyGetLeaderActor(), 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(msg + 1);
  } else {
    Engine_EventWait(20);
    Engine_EventSetMessage(msg + 2);
  }
  Engine_EventShowMessage(12, 0);
  Engine_EventEnd();
}

void Func_02000340(s32 keeper) {
  u16 facing = Engine_ActorGet(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: the loop note ends the scheduling block: the native loads the
   * facing before it builds the quadrant offset (2 differ without it). */
  do {
  } while (0);
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Engine_ShopOpen(3, keeper);
  } else {
    Engine_EventStart();
    Engine_EventResolvePendingActions(0);
    Engine_EventSetMessage((s32)MsgDailaTaviRikiHome);
    Engine_EventShowMessage(keeper, 0);
    Engine_EventEnd();
  }
}
