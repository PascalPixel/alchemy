#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "PARTY_STATE.H"
#include "OBJECT_RUNTIME.H"

void DisplayTransition_Start(s32 mode, s32 frames);
void DisplayTransition_Finish(s32 mode, s32 frames);

void EventRuntime_StartScreen(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    DisplayTransition_Start(event->screen_effect, event->delay);
    event->screen_status = 1;
}

void EventRuntime_FinishScreen(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    DisplayTransition_Finish(event->screen_effect, event->delay);
    event->screen_status = 0;
}

void EventRuntime_WaitDelay(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    WaitFrames(event->delay);
}

void EventRuntime_SetDestination(s32 scene, s32 entrance)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    struct PartyState *party;

    event->countdown = 999;
    /* FAKEMATCH: plain source forms and 5,739 permutations retain the reordered literal load. Materialize the party pointer before building the scene offset, as the complete 44-byte listing does. */
    asm volatile ("" : "=r" (party) : "0" (&gPartyState));
    party->scene = scene;
    party->entrance = entrance;
}

void EventRuntime_SetSavedDestination(s32 scene, s32 entrance)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    struct PartyState *party;

    event->countdown = 999;
    /* FAKEMATCH: plain source forms retain the reordered literal load. Materialize the party pointer before building the saved-scene offset, as the complete 44-byte listing does. */
    asm volatile ("" : "=r" (party) : "0" (&gPartyState));
    party->saved_scene = scene;
    party->saved_entrance = entrance;
}

void EventRuntime_SetCountdown(u16 frames)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->countdown = frames;
}

u16 BattleFx_GetWeightedResult(s32 row, s32 column);
s32 BattleFx_LookupResult(void *position);
s32 BattleFx_SelectBattleCue(s32 row, s32 column);

/* ☀️'s scripted battle request: store the encounter table's enemy group for
   (row, column) as the next battle, let a controlled object look it up, and
   pick the battle's cue. ⚓️ asks for the controlled object rather than
   reading it. */
void BattleFx_SetWeightedResult(s32 row, s32 column)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->battle_group = BattleFx_GetWeightedResult(row, column);
    /* FAKEMATCH: the ARM9 scheduling would address the control mode before storing the group. */
    __asm__("");
    if (event->control_mode == 3)
        BattleFx_LookupResult((u8 *)ObjectTable_Get(EventRuntime_GetControlledOwner()) + 8);
    BattleFx_SelectBattleCue(row, column);
}
