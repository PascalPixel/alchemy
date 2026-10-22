#include "ram_buffer.h"
#include "event_work.h"
#include "party_state.h"
#include "item.h"

void EventRuntime_RunMessage(s32 actor, s32 message) {
  EventRuntime_Start();
  EventRuntime_SetMessage(message);
  EventRuntime_ShowMessageAndWait(actor, 0);
  EventRuntime_End();
}

void EventRuntime_ResolveAllPendingActions(void) {
  EventRuntime_ResolvePendingActions(0);
}

void EventRuntime_ResolvePendingActions(s32 skip) {
  struct EventWork *state = Ram_HeapSlots->eventWork;

  state->actionCounter = 0;
  if (state->pendingFirst != 0 && !(skip & 1))
    EventRuntime_ExecutePackedAction(0x2090);
  if (state->pendingSecond != 0 && !(skip & 2))
    EventRuntime_ExecutePackedAction(0x209b);
  if (gPartyState.pendingThird != 0 && !(skip & 4))
    EventRuntime_ExecutePackedAction(0x208b);
}

s32 EventRuntime_GetControlledOwner(void) {
  struct PartyState *state = &gPartyState;
  s32 actor;

  /* FAKEMATCH: load the party state's address before the default owner. */
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
