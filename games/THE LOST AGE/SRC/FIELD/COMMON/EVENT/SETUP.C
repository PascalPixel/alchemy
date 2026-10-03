#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "PARTY_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "CALLBACK_SCHEDULER.H"
#include "GAME_FLAGS.H"
#include "SYSTEM.H"

void UiTimedNotice_CloseIfActiveFar(void);
void GameFlag_RefreshLureCapFar(void);
void Object_AttachWorkTargetToObject(s32 object, s32 enabled);

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
    /* FAKEMATCH: eight ordinary forms reordered the zero stores and party-mode address construction; this boundary keeps the 72-byte order. */
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
        /* FAKEMATCH: four ordinary forms loaded this pointer after the destination offset; its handoff preserves the 168-byte order. */
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
        /* FAKEMATCH: four ordinary forms built the override offset before loading this pointer; its handoff preserves the 64-byte order. */
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
