#include "ram_buffer.h"
#include "event_work.h"
#include "adventure.h"
#include "item.h"

void Event_RunMessage(s32 actor, s32 message) {
  Event_Start();
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

void Event_ResolveAllPendingActions(void) {
  Event_ResolvePendingActions(0);
}

void Event_ResolvePendingActions(s32 skip) {
  struct EventWork *state = Ram_HeapSlots->eventWork;

  state->actionCounter = 0;
  if (state->pendingFirst != 0 && !(skip & 1))
    Event_ExecutePackedAction(0x2090);
  if (state->pendingSecond != 0 && !(skip & 2))
    Event_ExecutePackedAction(0x209b);
  if (gAdventure.pendingThird != 0 && !(skip & 4))
    Event_ExecutePackedAction(0x208b);
}

s32 Party_GetLeaderActor(void) {
  struct Adventure *state = &gAdventure;
  s32 actor;

  /* FAKEMATCH: load the adventure's address before the default owner. */
  asm("");
  actor = 8;
  if (state->ownerOverride == 0)
    actor = state->currentOwner;
  return actor;
}

s32 Item_MatchesFilter(s32 filter, s32 item_id) {
  if (filter == item_id)
    return 1;
  if (filter == 0x1ff)
    return 1;
  if (filter == 0x1fe && (u32)(Item_Get(item_id)->type - 1) <= 3)
    return 1;
  return 0;
}
