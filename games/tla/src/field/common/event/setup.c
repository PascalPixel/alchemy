#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "party_state.h"
#include "object_runtime.h"
#include "callback_scheduler.h"
#include "game_flags.h"
#include "system.h"


void EventRuntime_Wait(s32 frames)
{
    struct EventWork *state = Ram_HeapSlots->event_work;
    if (state->wait_mode == 0 && frames != 0)
        WaitFrames(frames);
}

void EventRuntime_PrepareCurrentObject(void)
{
    struct ObjectRuntime *object =
        ObjectTable_Get(EventRuntime_GetControlledOwner());
    object->speed_limit = 0x10000;
    object->acceleration = 0x8000;
    object->target_x = 0x80000000;
    object->target_z = 0x80000000;
    object->velocity_x = 0;
    object->velocity_z = 0;
    /* FAKEMATCH: this boundary keeps the zero stores before party-mode address construction. */
    __asm__ volatile("" : : : "memory");
    if (gPartyState.render_mode == 2)
        Object_SetMode(object, 12);
    else
        Object_SetMode(object, 1);
}

void EventRuntime_Begin(void)
{
    struct EventWork *state = Ram_HeapSlots->event_work;
    UiTimedNotice_CloseIfActiveFar();
    EventRuntime_PrepareCurrentObject();
    if (state->action_counter != 0)
        EventRuntime_ResolveAllPendingActions();
    state->unknown_cb0 = 0;
    state->unknown_cb2 = 0;
    state->unknown_cb4 = 0;
    state->delay = 16;
    state->wait_mode = 0;
    state->message_actor = 0xffff;
    state->choice = -1;
    state->message_state = -1;
    Scheduler_AddOrUpdateCallback((s32)EventRuntime_UpdateWaitMode, 0x480);
    GameFlag_ClearBit(0x132);
    {
        struct PartyState *party = &gPartyState;
        /* FAKEMATCH: the pointer handoff keeps its load before the destination offset. */
        __asm__ volatile("" : "+r"(party));
        state->current_owner = party->current_owner;
        state->object_status = 0;
    }
}

void EventRuntime_End(void)
{
    Scheduler_RemoveCallback((u32)EventRuntime_UpdateWaitMode);
    {
        struct PartyState *party = &gPartyState;
        /* FAKEMATCH: the pointer handoff keeps its load before the override offset. */
        __asm__ volatile("" : "+r"(party));
        if (party->owner_override == 0)
            Object_AttachWorkTargetToObject(party->current_owner, 1);
        else
            Object_AttachWorkTargetToObject(8, 1);
        GameFlag_RefreshLureCapFar();
    }
}

void EventRuntime_EmptyHook(void)
{
}
