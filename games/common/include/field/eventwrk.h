#ifndef ALCHEMY_EVENTWRK_H
#define ALCHEMY_EVENTWRK_H

#include "types.h"
#include "edition.h"
#include "layout_guard.h"

struct ScenePlacement;
struct SceneEvent;
struct FieldActor;
struct ObjectRuntime;

#if GAME_TLA
struct EventWork {
    u8 unknown_000[0x14];
    struct ObjectRuntime *objects[80];
    s16 exit;
    u8 unknown_156[2];
    s16 countdown;
    u8 unknown_15a[0xa];
    u16 battle_group;
    u8 unknown_166[4];
    u16 unknown_16a;
    u8 unknown_16c[0x18];
    s16 unknown_184;
    u8 unknown_186[4];
    s8 control_mode;
    u8 unknown_18b[0x21];
    s32 screen_effect;
    u8 unknown_1b0[2];
    s16 screen_status;
    s32 delay;
    s32 wait_mode;
    u8 unknown_1bc[8];
    u16 message_id;
    u16 message_actor;
    s16 choice;
    s16 message_state;
    struct ObjectRuntime *view_center;
    u8 unknown_1d0[0x10];
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
    s16 cue_state;
};

LAYOUT_OFFSET_GUARD(EventWork_Objects, struct EventWork, objects, 0x14);
LAYOUT_OFFSET_GUARD(EventWork_Exit, struct EventWork, exit, 0x154);
LAYOUT_OFFSET_GUARD(EventWork_Countdown, struct EventWork, countdown, 0x158);
LAYOUT_OFFSET_GUARD(EventWork_BattleGroup, struct EventWork, battle_group, 0x164);
LAYOUT_OFFSET_GUARD(EventWork_ControlMode, struct EventWork, control_mode, 0x18a);
LAYOUT_OFFSET_GUARD(EventWork_ScreenEffect, struct EventWork, screen_effect, 0x1ac);
LAYOUT_OFFSET_GUARD(EventWork_Delay, struct EventWork, delay, 0x1b4);
LAYOUT_OFFSET_GUARD(EventWork_Message, struct EventWork, message_id, 0x1c4);
LAYOUT_OFFSET_GUARD(EventWork_CurrentOwner, struct EventWork, current_owner, 0x1e0);
LAYOUT_OFFSET_GUARD(EventWork_ActionCounter, struct EventWork, action_counter, 0xca2);
LAYOUT_OFFSET_GUARD(EventWork_PendingSecond, struct EventWork, pending_second, 0xcac);
LAYOUT_OFFSET_GUARD(EventWork_CueState, struct EventWork, cue_state, 0xcb8);
#else
struct EventWork {
    struct ScenePlacement *tables[4];
    struct SceneEvent *events;
    struct FieldActor *actors[8];
    struct FieldActor *placed_actors[58];
    u8 unknown_11c[0x50];
    s16 touched_trigger;
    u8 unknown_16e[2];
    u16 requested_exit;
    u16 unknown_172;
    u8 unknown_174[6];
    s16 facing_trigger;
    u8 unknown_17c[2];
    s16 psynergy_request;
    u8 unknown_180[2];
    s16 raised_trigger;
    u8 unknown_184[0x18];
    s16 blocked_steps;
    s16 mode;
    u8 encounter_groups[8];
    s32 encounter_bias;
    s32 encounter_threshold;
    u8 unknown_1b0[0x10];
    s32 start_transition;
    u8 unknown_1c4[4];
    s32 transition_frames;
    u8 unknown_1cc[0x0c];
    u16 message;
    u8 unknown_1da[6];
    struct FieldActor *view_center;
};

LAYOUT_OFFSET_GUARD(EventWork_PlacedActors, struct EventWork, placed_actors, 0x34);
LAYOUT_OFFSET_GUARD(EventWork_TouchedTrigger, struct EventWork, touched_trigger, 0x16c);
LAYOUT_OFFSET_GUARD(EventWork_RequestedExit, struct EventWork, requested_exit, 0x170);
LAYOUT_OFFSET_GUARD(EventWork_FacingTrigger, struct EventWork, facing_trigger, 0x17a);
LAYOUT_OFFSET_GUARD(EventWork_PsynergyRequest, struct EventWork, psynergy_request, 0x17e);
LAYOUT_OFFSET_GUARD(EventWork_RaisedTrigger, struct EventWork, raised_trigger, 0x182);
LAYOUT_OFFSET_GUARD(EventWork_BlockedSteps, struct EventWork, blocked_steps, 0x19c);
LAYOUT_OFFSET_GUARD(EventWork_Mode, struct EventWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(EventWork_EncounterGroups, struct EventWork, encounter_groups, 0x1a0);
LAYOUT_OFFSET_GUARD(EventWork_EncounterBias, struct EventWork, encounter_bias, 0x1a8);
LAYOUT_OFFSET_GUARD(EventWork_EncounterThreshold, struct EventWork, encounter_threshold, 0x1ac);
LAYOUT_OFFSET_GUARD(EventWork_StartTransition, struct EventWork, start_transition, 0x1c0);
LAYOUT_OFFSET_GUARD(EventWork_TransitionFrames, struct EventWork, transition_frames, 0x1c8);
LAYOUT_OFFSET_GUARD(EventWork_Message, struct EventWork, message, 0x1d8);
LAYOUT_OFFSET_GUARD(EventWork_ViewCenter, struct EventWork, view_center, 0x1e0);
#endif

#endif
