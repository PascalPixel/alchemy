
#include "types.h"
#include "field_event.h"
#include "party_state.h"
#include "ram_buffer.h"

void DeriMura_RunExitWalk(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_ActorSetSpeed(gPartyState.current_owner, 0x8000, 0x4000);
    Engine_ActorSetSpritePriority(gPartyState.current_owner, 2);
    Engine_ActorGet(gPartyState.current_owner)->motion_flags = 0;
    Engine_AudioPlayCue(123);
    Engine_ActorSetAnimation(gPartyState.current_owner, 2);
    Engine_ActorCenterAndWalk(gPartyState.current_owner, 2, -6);
    Engine_EventWait(10);
    Engine_EventSetCountdown(work->exit);
    Engine_EventCloseScreen();
    Engine_EventWaitDelay();
    Engine_EventEnd();
}

void DeriMura_ReturnMovedObject(s32 mode)
{
    Engine_ActorReturnHome(mode, 66, 0x300);
}
