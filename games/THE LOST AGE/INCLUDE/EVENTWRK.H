#ifndef TLA_EVENTWRK_H
#define TLA_EVENTWRK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct ObjectRuntime;

struct EventWork {
    u8 unknown_000[0x14];
    struct ObjectRuntime *objects[80];
    /* The exit the current event leaves by. */
    s16 exit;
    u8 unknown_156[2];
    /* Set to 999 when a destination is chosen. */
    s16 countdown;
    u8 unknown_15a[0x52];
    s32 screen_effect;
    u8 unknown_1b0[2];
    s16 screen_status;
    s32 delay;
    /* The debug shoulder buttons disable event waits with -1. */
    s32 wait_mode;
    u8 unknown_1bc[8];
    /* The dialogue row; message and choice commands advance it. */
    u16 message_id;
    u16 message_actor;
    s16 choice;
    s16 message_state;
    u8 unknown_1cc[0x14];
    s32 current_owner;
    s32 object_status;
    u8 unknown_1e8[0xaba];
    s16 action_counter;
    s8 pending_first;
    u8 unknown_ca5[7];
    s8 pending_second;
    u8 unknown_cad[3];
    s16 unknown_cb0;
    s16 unknown_cb2;
    s16 unknown_cb4;
    u8 unknown_cb6[2];
    /* -1 while the sound test plays its cue pair. */
    s16 cue_state;
};

LAYOUT_OFFSET_GUARD(EventWork_Objects, struct EventWork, objects, 0x14);
LAYOUT_OFFSET_GUARD(EventWork_Exit, struct EventWork, exit, 0x154);
LAYOUT_OFFSET_GUARD(EventWork_Countdown, struct EventWork, countdown, 0x158);
LAYOUT_OFFSET_GUARD(EventWork_ScreenEffect, struct EventWork, screen_effect, 0x1ac);
LAYOUT_OFFSET_GUARD(EventWork_Delay, struct EventWork, delay, 0x1b4);
LAYOUT_OFFSET_GUARD(EventWork_Message, struct EventWork, message_id, 0x1c4);
LAYOUT_OFFSET_GUARD(EventWork_CurrentOwner, struct EventWork, current_owner, 0x1e0);
LAYOUT_OFFSET_GUARD(EventWork_ActionCounter, struct EventWork, action_counter, 0xca2);
LAYOUT_OFFSET_GUARD(EventWork_PendingSecond, struct EventWork, pending_second, 0xcac);
LAYOUT_OFFSET_GUARD(EventWork_CueState, struct EventWork, cue_state, 0xcb8);

void EventRuntime_RunMessage(s32 actor, s32 message);
void EventRuntime_ResolveAllPendingActions(void);
void EventRuntime_ResolvePendingActions(s32 skip);
s32 EventRuntime_ExecutePackedAction(u32 action);
s32 EventRuntime_GetControlledOwner(void);
void EventRuntime_ShowMessageAndWait(s32 actor, s32 flags);
void EventRuntime_UpdateWaitMode(void);
void EventRuntime_Wait(s32 frames);
void EventRuntime_PrepareCurrentObject(void);
void EventRuntime_Begin(void);
void EventRuntime_End(void);
void EventRuntime_EmptyHook(void);
void EventRuntime_StartScreen(void);
void EventRuntime_FinishScreen(void);
void EventRuntime_WaitDelay(void);
void EventRuntime_SetDestination(s32 scene, s32 entrance);
void EventRuntime_SetSavedDestination(s32 scene, s32 entrance);
void EventRuntime_SetCountdown(u16 countdown);
void EventRuntime_SetMessage(s32 message);

#endif
