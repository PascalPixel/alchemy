/* NONMATCHING: resource_651 at 0x0200809c and 0x02008118, for
 * FIELD/DERI_MURA, stay listing.
 *
 * Remaining difference: one instruction's place in each. The ROM builds the
 * last argument between the other two: movs r2, #6; movs r1, #2; negs r2
 * for the walk, and movs r2, #192; movs r1, #66; lsls r2, #2 for the search
 * answer. Stock agscc finishes r2 before it sets r1. Every other byte,
 * including the event work read through Ram_HeapSlots, matches.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"

/* The leader walks off the map by the event's exit while the screen closes. */
void DeriMura_RunExitWalk(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    Engine_EventBegin();
    Engine_EventPrepareSpeakers(0);
    Engine_ActorSetSpeed(gPartyState.current_owner, 0x8000, 0x4000);
    Engine_ActorSetSpritePriority(gPartyState.current_owner, 2);
    Engine_ActorGet(gPartyState.current_owner)->motion_flags = 0;
    Engine_AudioPlayCue(123);
    Engine_ActorSetAnimation(gPartyState.current_owner, 2);
    Engine_ActorCenterAndWalk(gPartyState.current_owner, 2, -6);
    Engine_EventWait(10);
    Engine_EventRequestExit(work->exit);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

/* Answers a Psynergy search of the object the town moved aside. */
void DeriMura_ReturnMovedObject(s32 mode)
{
    Engine_ActorReturnHome(mode, 66, 0x300);
}
