#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "PARTY_STATE.H"

void EventRuntime_RunMessage(s32 actor, s32 message)
{
    EventRuntime_Begin();
    EventRuntime_SetMessage(message);
    EventRuntime_ShowMessageAndWait(actor, 0);
    EventRuntime_End();
}

void EventRuntime_ResolveAllPendingActions(void)
{
    EventRuntime_ResolvePendingActions(0);
}

void EventRuntime_ResolvePendingActions(s32 skip)
{
    struct EventWork *state = Ram_HeapSlots->event_work;

    state->action_counter = 0;
    if (state->pending_first != 0 && !(skip & 1))
        EventRuntime_ExecutePackedAction(0x2090);
    if (state->pending_second != 0 && !(skip & 2))
        EventRuntime_ExecutePackedAction(0x209b);
    if (gPartyState.pending_third != 0 && !(skip & 4))
        EventRuntime_ExecutePackedAction(0x208b);
}
