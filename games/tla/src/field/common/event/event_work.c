#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "party_state.h"
#include "object_runtime.h"
#include "display_transition.h"

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
    /* FAKEMATCH: materialize the party pointer before building the scene offset. */
    asm volatile ("" : "=r" (party) : "0" (&gPartyState));
    party->scene = scene;
    party->entrance = entrance;
}

void EventRuntime_SetSavedDestination(s32 scene, s32 entrance)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    struct PartyState *party;

    event->countdown = 999;
    /* FAKEMATCH: materialize the party pointer before building the saved-scene offset. */
    asm volatile ("" : "=r" (party) : "0" (&gPartyState));
    party->saved_scene = scene;
    party->saved_entrance = entrance;
}

void EventRuntime_SetCountdown(u16 frames)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->countdown = frames;
}


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
