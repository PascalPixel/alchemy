#ifndef ALCHEMY_EVENT_WORK_H
#define ALCHEMY_EVENT_WORK_H

#include "types.h"
#include "edition.h"
#include "layout_guard.h"

struct ScenePlacement;
struct SceneEvent;
struct FieldActor;
struct ObjectRuntime;

#if GAME_TLA
struct EventWork
{
    u8                    unk000[0x14];
    struct ObjectRuntime *objects[80];
    s16                   exit;
    u8                    unk156[2];
    s16                   countdown;
    u16                   unk15a;
    u8                    unk15c[8];
    u16                   battleGroup;
    u8                    unk166[4];
    u16                   unk16a;
    u8                    unk16c[0x18];
    s16                   unk184;
    u8                    unk186[4];
    s8                    controlMode;
    u8                    unk18b[0x21];
    s32                   screenEffect;
    u8                    unk1b0[2];
    s16                   screenStatus;
    s32                   delay;
    s32                   waitMode;
    u8                    unk1bc[8];
    u16                   messageId;
    u16                   messageActor;
    s16                   choice;
    s16                   messageState;
    struct ObjectRuntime *viewCenter;
    u8                    unk1d0[0x10];
    s32                   currentOwner;
    s32                   objectStatus;
    u8                    unk1e8[0xaba];
    s16                   actionCounter;
    s8                    pendingFirst;
    u8                    unkca5[7];
    s8                    pendingSecond;
    u8                    unkcad[3];
    s16                   unkcb0;
    s16                   unkcb2;
    s16                   unkcb4;
    u8                    unkcb6[2];
    s16                   cueState;
};

LAYOUT_OFFSET_GUARD(EventWork_Objects, struct EventWork, objects, 0x14);
LAYOUT_OFFSET_GUARD(EventWork_Exit, struct EventWork, exit, 0x154);
LAYOUT_OFFSET_GUARD(EventWork_Countdown, struct EventWork, countdown, 0x158);
LAYOUT_OFFSET_GUARD(EventWork_BattleGroup, struct EventWork, battleGroup, 0x164);
LAYOUT_OFFSET_GUARD(EventWork_ControlMode, struct EventWork, controlMode, 0x18a);
LAYOUT_OFFSET_GUARD(EventWork_ScreenEffect, struct EventWork, screenEffect, 0x1ac);
LAYOUT_OFFSET_GUARD(EventWork_Delay, struct EventWork, delay, 0x1b4);
LAYOUT_OFFSET_GUARD(EventWork_Message, struct EventWork, messageId, 0x1c4);
LAYOUT_OFFSET_GUARD(EventWork_CurrentOwner, struct EventWork, currentOwner, 0x1e0);
LAYOUT_OFFSET_GUARD(EventWork_ActionCounter, struct EventWork, actionCounter, 0xca2);
LAYOUT_OFFSET_GUARD(EventWork_PendingSecond, struct EventWork, pendingSecond, 0xcac);
LAYOUT_OFFSET_GUARD(EventWork_CueState, struct EventWork, cueState, 0xcb8);
#else
struct EventWork
{
    struct ScenePlacement *tables[4];
    struct SceneEvent     *events;
    struct FieldActor     *actors[8];
    struct FieldActor     *placedActors[58];
    u8                     unk11c[0x50];
    s16                    touchedTrigger;
    u8                     unk16e[2];
    u16                    requestedExit;
    u16                    unk172;
    u8                     unk174[6];
    s16                    facingTrigger;
    u8                     unk17c[2];
    s16                    psynergyRequest;
    u8                     unk180[2];
    s16                    raisedTrigger;
    u8                     unk184[0x18];
    s16                    blockedSteps;
    s16                    mode;
    u8                     encounterGroups[8];
    s32                    encounterBias;
    s32                    encounterThreshold;
    u8                     unk1b0[0x10];
    s32                    startTransition;
    u8                     unk1c4[4];
    s32                    transitionFrames;
    u8                     unk1cc[0x0c];
    u16                    message;
    u8                     unk1da[6];
    struct FieldActor     *viewCenter;
    u8                     unk1e4[0xade];
    u16                    choiceMessage; // 選択肢のメッセージ
    u16                    unkcc4;
};

LAYOUT_OFFSET_GUARD(EventWork_PlacedActors, struct EventWork, placedActors, 0x34);
LAYOUT_OFFSET_GUARD(EventWork_TouchedTrigger, struct EventWork, touchedTrigger, 0x16c);
LAYOUT_OFFSET_GUARD(EventWork_RequestedExit, struct EventWork, requestedExit, 0x170);
LAYOUT_OFFSET_GUARD(EventWork_FacingTrigger, struct EventWork, facingTrigger, 0x17a);
LAYOUT_OFFSET_GUARD(EventWork_PsynergyRequest, struct EventWork, psynergyRequest, 0x17e);
LAYOUT_OFFSET_GUARD(EventWork_RaisedTrigger, struct EventWork, raisedTrigger, 0x182);
LAYOUT_OFFSET_GUARD(EventWork_BlockedSteps, struct EventWork, blockedSteps, 0x19c);
LAYOUT_OFFSET_GUARD(EventWork_Mode, struct EventWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(EventWork_EncounterGroups, struct EventWork, encounterGroups, 0x1a0);
LAYOUT_OFFSET_GUARD(EventWork_EncounterBias, struct EventWork, encounterBias, 0x1a8);
LAYOUT_OFFSET_GUARD(EventWork_EncounterThreshold, struct EventWork, encounterThreshold, 0x1ac);
LAYOUT_OFFSET_GUARD(EventWork_StartTransition, struct EventWork, startTransition, 0x1c0);
LAYOUT_OFFSET_GUARD(EventWork_TransitionFrames, struct EventWork, transitionFrames, 0x1c8);
LAYOUT_OFFSET_GUARD(EventWork_Message, struct EventWork, message, 0x1d8);
LAYOUT_OFFSET_GUARD(EventWork_ViewCenter, struct EventWork, viewCenter, 0x1e0);
LAYOUT_OFFSET_GUARD(EventWork_ChoiceMessage, struct EventWork, choiceMessage, 0xcc2);
#endif

#endif
