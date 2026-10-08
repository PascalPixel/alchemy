#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "kanpan.h"
#include "text/msg_ids.h"

#define ObjectVisual_CopyAttributes ((void (*)(s32,s32))ObjectVisual_CopyAttributes)

u32 FuneKanpan_SlotMode[8];

union KanpanSlot {
    s32 w;
    s16 h[2];
};



struct SlotSprite {
    s32 unknown_00[7];
    s16 unknown_1c;
    s16 tilt;
};

void Event_CallWithLastActiveObjectId(u8 *script);
void FieldScene_RunStepThen10(s32 step);
void FieldScene_CallPairWith10(s32 actor, s32 facing);
void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, s32 b);
void FuneKanpan_OpenFirstDoor(void);

void FuneKanpan_ChooseSlotModes(void)
{
    u32 *mode = FuneKanpan_SlotMode;
    u32 i;

    for (i = 0; i <= 3; i++) {
        if (SceneState_FindFirstSetFlagOfGroup(i) != 0)
            *mode = SceneData_SelectTableEntryByFlagGroup(i);
        else
            *mode = 3;
        mode++;
    }
    if (SceneState_FindFirstSetFlagOfGroup(0) != 0)
        FuneKanpan_SlotMode[0] = SceneData_SelectTableEntryByFlagGroup(0);
    else
        FuneKanpan_SlotMode[0] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(2) != 0)
        FuneKanpan_SlotMode[1] = SceneData_SelectTableEntryByFlagGroup(2);
    else
        FuneKanpan_SlotMode[1] = 3;
    FuneKanpan_SlotMode[2] = 3;
    FuneKanpan_SlotMode[3] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(1) != 0)
        FuneKanpan_SlotMode[4] = SceneData_SelectTableEntryByFlagGroup(1);
    else
        FuneKanpan_SlotMode[4] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(3) != 0)
        FuneKanpan_SlotMode[5] = SceneData_SelectTableEntryByFlagGroup(3);
    else
        FuneKanpan_SlotMode[5] = 3;
    FuneKanpan_SlotMode[6] = 3;
    FuneKanpan_SlotMode[7] = 3;
}

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Engine_GameFlagIsSet(base + i) != 0) return FuneKanpan_FlagGroupEntries[i];
    }
    return 0;
}

void SceneState_ConfigureEntries8Through19(void)
{
    SceneEffect_AdvanceSlotByValueBand(8, 0);
    FuneKanpan_SlotPhase[0] = 0;
    SceneEffect_AdvanceSlotByValueBand(9, 1);
    FuneKanpan_SlotPhase[1] += 0x80;
    SceneEffect_AdvanceSlotByValueBand(10, 2);
    FuneKanpan_SlotPhase[2] += 0x100;
    SceneEffect_AdvanceSlotByValueBand(11, 3);
    FuneKanpan_SlotPhase[3] += 0x200;
    SceneEffect_SelectSlotValueAndPosition(12, 0, 0);
    SceneEffect_SelectSlotValueAndPosition(13, 1, 0);
    SceneEffect_SelectSlotValueAndPosition(14, 2, 0);
    SceneEffect_SelectSlotValueAndPosition(15, 3, 0);
    SceneEffect_SelectSlotValueAndPosition(16, 4, 1);
    SceneEffect_SelectSlotValueAndPosition(17, 5, 1);
    SceneEffect_SelectSlotValueAndPosition(18, 6, 1);
    SceneEffect_SelectSlotValueAndPosition(19, 7, 1);
}

void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot)
{
    u16 value = FuneKanpan_SlotValue[slot];

    if (value >= 0x6801 && value <= 0x6fff) {
        FuneKanpan_SlotPhase[slot] += 0x70;
        Engine_ActorSetAnimation(actor, 3);
    } else if (value >= 0xe801 && value <= 0xefff) {
        FuneKanpan_SlotPhase[slot] += 0xe0;
        Engine_ActorSetAnimation(actor, 3);
    } else if (value >= 0x7001 && value <= 0xefff) {
        FuneKanpan_SlotPhase[slot] += 0x1c0;
        Engine_ActorSetAnimation(actor, 2);
    } else {
        FuneKanpan_SlotPhase[slot] += 0x300;
        Engine_ActorSetAnimation(actor, 1);
    }
}

void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags)
{
    struct FieldActor *actor = Object_GetById(id);
    struct SlotSprite *sprite = (struct SlotSprite *)actor->sprite;
    s32 sine;

    if ((flags & 2) == 0) {
        switch (FuneKanpan_SlotMode[slot]) {
        case 1:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[0];
            ObjectVisual_CopyAttributes(id, 8);
            break;
        case 2:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[1];
            ObjectVisual_CopyAttributes(id, 9);
            break;
        case 3:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[2];
            ObjectVisual_CopyAttributes(id, 10);
            break;
        case 4:
            FuneKanpan_SlotValue[slot] = FuneKanpan_SlotPhase[3];
            ObjectVisual_CopyAttributes(id, 11);
            break;
        }
    }
    if (flags & 1) {
        sine = Engine_MathSin(FuneKanpan_SlotValue[slot]);
        sprite->tilt = Engine_MathSin(FuneKanpan_SlotValue[slot] + 0x8000) >> 5;
        actor->z.fixed = FuneKanpan_SlotDepth[slot] - (sine << 2) - (sine << 1);
    } else {
        sine = Engine_MathSin(FuneKanpan_SlotValue[slot] + 0x8000);
        sprite->tilt = Engine_MathSin(FuneKanpan_SlotValue[slot]) >> 5;
        actor->z.fixed = FuneKanpan_SlotDepth[slot] + (sine << 2) + (sine << 1);
    }
}

void SceneState_InitActorSlots8To19(void)
{
    struct FieldActor *actor;
    u32 i;

    for (i = 0; i <= 7; i++)
        FuneKanpan_SlotValue[i] = 0xc000;
    OverlayObject_ActivateSlotWithMode3(8);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    OverlayObject_ActivateSlotWithMode3(13);
    OverlayObject_ActivateSlotWithMode3(14);
    OverlayObject_ActivateSlotWithMode3(15);
    FuneKanpan_SlotMode[0] = 0;
    FuneKanpan_SlotMode[1] = 0;
    FuneKanpan_SlotMode[2] = 0;
    FuneKanpan_SlotMode[3] = 0;
    FuneKanpan_SlotDepth[0] = Object_GetById(8)->z.fixed;
    FuneKanpan_SlotDepth[1] = Object_GetById(13)->z.fixed;
    FuneKanpan_SlotDepth[2] = Object_GetById(14)->z.fixed;
    FuneKanpan_SlotDepth[3] = Object_GetById(15)->z.fixed;
    OverlayObject_ActivateSlotWithMode3(16);
    OverlayObject_ActivateSlotWithMode3(17);
    OverlayObject_ActivateSlotWithMode3(18);
    OverlayObject_ActivateSlotWithMode3(19);
    Object_GetById(16)->scale_x = 0xffff0000;
    Object_GetById(17)->scale_x = 0xffff0000;
    Object_GetById(18)->scale_x = 0xffff0000;
    Object_GetById(19)->scale_x = 0xffff0000;
    FuneKanpan_SlotMode[4] = 0;
    FuneKanpan_SlotMode[5] = 0;
    FuneKanpan_SlotMode[6] = 0;
    FuneKanpan_SlotMode[7] = 0;
    FuneKanpan_SlotDepth[4] = Object_GetById(16)->z.fixed;
    FuneKanpan_SlotDepth[5] = Object_GetById(17)->z.fixed;
    FuneKanpan_SlotDepth[6] = Object_GetById(18)->z.fixed;
    FuneKanpan_SlotDepth[7] = Object_GetById(19)->z.fixed;
    actor = Object_GetById(0);
    if (actor != NULL)
        Engine_ActorSetPosition(8, actor->x.fixed, actor->z.fixed);
    Engine_TaskWait(1);
    ObjectVisual_CopyAttributes(13, 8);
    ObjectVisual_CopyAttributes(14, 8);
    ObjectVisual_CopyAttributes(15, 8);
    ObjectVisual_CopyAttributes(16, 8);
    ObjectVisual_CopyAttributes(17, 8);
    ObjectVisual_CopyAttributes(18, 8);
    ObjectVisual_CopyAttributes(19, 8);
    Object_GetById(8)->unknown_5c = 1;
    Object_GetById(13)->unknown_5c = 1;
    Object_GetById(14)->unknown_5c = 1;
    Object_GetById(15)->unknown_5c = 1;
    Object_GetById(16)->unknown_5c = 1;
    Object_GetById(17)->unknown_5c = 1;
    Object_GetById(18)->unknown_5c = 1;
    Object_GetById(19)->unknown_5c = 1;
    Engine_TaskWait(1);
    Engine_ActorSetPosition(8, 0x840000, 0x2780000);
    Engine_TaskWait(1);
    SceneEffect_SelectSlotValueAndPosition(8, 0, 2);
    SceneEffect_SelectSlotValueAndPosition(13, 1, 2);
    SceneEffect_SelectSlotValueAndPosition(14, 2, 2);
    SceneEffect_SelectSlotValueAndPosition(15, 3, 2);
    SceneEffect_SelectSlotValueAndPosition(16, 4, 3);
    SceneEffect_SelectSlotValueAndPosition(17, 5, 3);
    SceneEffect_SelectSlotValueAndPosition(18, 6, 3);
    SceneEffect_SelectSlotValueAndPosition(19, 7, 3);
}

void SceneState_ApplyFiveRectsAtColumn78(void)
{
    Engine_MapCopyCellsTo(78, 39, 78, 40, 5, 1);
    Engine_MapCopyCellsTo(78, 39, 78, 41, 5, 1);
    Engine_MapCopyCellsTo(78, 39, 79, 42, 4, 1);
    Engine_MapCopyCellsTo(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Engine_MapCopyCellAttributes(17, 38, 5, 2, x, y);
    }
}

void DialogueLayout_ConfigureTwoRegions(void)
{
    Engine_MapCopyCellsTo(66, 61, 64, 40, 5, 4);
    Engine_MapCopyCellAttributes(0, 0, 5, 4, 5, 39);
}

void FieldScene_RunStepThen10(s32 a)
{
    Engine_EventShowMessage(a, 0);
    Engine_EventWait(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Engine_ActorFaceDirection(a, b, 10);
}

void FuneKanpan_ArriveAtTolbi(void)
{
    Engine_EventBegin();
    FuneKanpan_LayerScroll[0] = 0x40000;
    FuneKanpan_LayerSpeed[0] = -0x8000;
    Event_CallWithLastActiveObjectId(FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Actor_SetPosition(21, 0xb60000, 0x26a0000);
    Actor_Get(21)->facing = 0xc000;
    Actor_SetPosition(20, 0xda0000, 0x2040000);
    Actor_Get(20)->facing = 0xb000;
    Actor_SetPosition(22, 0xcc0000, 0x20e0000);
    Actor_Get(22)->facing = 0xb000;
    Actor_SetPosition(23, 0, 0);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 182, 0x214);
    Actor_FaceDirection(21, 0xb000, 40);
    Engine_EventSetMessage((s32)MsgFuneFinallyReachedTolbi);
    FieldScene_RunStepThen10(21);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_ActorSetAnimation(20, 4);
    FieldScene_RunStepThen10(0x6014);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(22, 0x101, 60);
    Engine_ActorSetAnimation(20, 3);
    FieldScene_RunStepThen10(0x6014);
    Actor_SetAttachedEffect(21, 0x102);
    Actor_SetAttachedEffect(22, 0x102);
    Engine_EventWait(80);
    Actor_ShowEmote(21, 0x100, 20);
    Actor_SetSpeed(21, 0x19999, 0xcccc);
    Actor_WalkToAndWait(21, 194, 0x1f4);
    Actor_FaceDirection(21, 0xb000, 20);
    Actor_SetSpeed(22, 0xcccc, 0x6666);
    Actor_WalkToAndWait(22, 192, 0x206);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkToAndWait(20, 210, 0x1fc);
    FieldScene_CallPairWith10(20, 0xb000);
    Engine_ActorRunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(0x5015);
    Engine_ActorSetAnimationAndWait(20, 3);
    FieldScene_CallPairWith10(22, 0xd000);
    FieldScene_RunStepThen10(0x9016);
    Actor_FaceDirection(20, 0x8000, 20);
    Engine_ActorSetAnimation(20, 4);
    FieldScene_RunStepThen10(0xa014);
    Actor_WalkToAndWait(20, 204, 0x218);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_WalkToAndWait(20, 182, 0x224);
    Actor_WalkToAndWait(20, 182, 0x250);
    Actor_WalkTo(20, 182, 0x298);
    Engine_EventWait(40);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(16);
}

void FieldScene_RunThreeActorEncounter(void)
{
    struct FieldActor *leader;
    u8 bits;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
        Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
        Actor_SetPosition(IWAN, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
        Actor_SetPosition(MEARI, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x13333, 0x9999);
    Actor_WalkTo(GERALD, 194, 0x280);
    Actor_WalkTo(IWAN, 198, 0x28e);
    Actor_WalkToAndWait(MEARI, 194, 0x2a0);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(GERALD, 0x8000, 0);
    Actor_FaceDirection(IWAN, 0x8000, 0);
    FieldScene_CallPairWith10(3, 0x8000);
    FieldScene_CallPairWith10(22, 0);
    Engine_EventSetMessage((s32)MsgFuneRobinTalkedPassengersDidntTour);
    FieldScene_RunStepThen10(22);
    FieldScene_CallPairWith10(21, 0xd000);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_ShowEmote(22, 0x100, 20);
    Engine_ActorRunRepeatedMotion(22, 1);
    Event_OpenMessage(22, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        FieldScene_RunStepThen10(2);
        FieldScene_CallPairWith10(3, 0xa000);
        Engine_ActorSetAnimation(MEARI, 3);
        FieldScene_RunStepThen10(3);
        FieldScene_CallPairWith10(1, 0x6000);
        Engine_ActorRunRepeatedMotion(GERALD, 1);
        Event_OpenMessage(GERALD, 0);
    ask_again:
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Engine_ActorRunRepeatedMotion(IWAN, 1);
            Engine_EventSetMessage((s32)MsgFuneRobinDontTalkLikeShouldnt);
            Event_OpenMessage(IWAN, 0);
            goto ask_again;
        }
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventSetMessage((s32)MsgFuneSeeYoureGoingGoFor);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    Object_GetById(22)->steering_flags &= 254;
    Actor_WalkToAndWait(22, 162, 0x27a);
    Engine_EventWait(1);
    bits = ACTOR_FACES_MOTION;
    {
        struct FieldActor *actor = Object_GetById(22);
        u8 value = actor->steering_flags;

        actor->steering_flags = value | bits;
    }
    Object_GetById(21)->steering_flags &= 254;
    Actor_WalkToAndWait(21, 162, 0x2a4);
    Engine_EventWait(1);
    {
        struct FieldActor *actor = Object_GetById(21);

        bits |= actor->steering_flags;
        actor->steering_flags = bits;
    }
    Actor_FaceDirection(22, 0x3000, 0);
    FieldScene_CallPairWith10(21, 0xd000);
    FieldScene_RunStepThen10(22);
    Actor_WalkTo(GERALD, 180, 0x28e);
    Actor_WalkTo(IWAN, 180, 0x28e);
    Actor_WalkToAndWait(MEARI, 180, 0x28e);
    Engine_ActorDestroy(GERALD);
    Engine_ActorDestroy(IWAN);
    Engine_ActorDestroy(MEARI);
    GameFlag_Set(0x903);
    Engine_EventEnd();
}

void FuneKanpan_RunRobinTalk(void)
{
    Engine_EventBegin();
    Event_CallWithLastActiveObjectId(FuneKanpan_ClosingCrewScript);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
    Actor_ShowEmote(22, 0x100, 0);
    Engine_ActorRunRepeatedMotion(22, 1);
    FieldScene_CallPairWith10(22, 0x5000);
    Engine_EventSetMessage((s32)MsgFuneHowWasRobinDidExplore);
    Event_OpenMessage(0x2016, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        FieldScene_RunStepThen10(0x2016);
        Engine_EventEnd();
        return;
    }
    gEventWork->message++;
    Event_AskYesNo(0x2016, 0);
    FuneKanpan_OpenFirstDoor();
    Actor_SetPosition(26, 0xd80000, 0x24c0000);
    Actor_SetSpeed(26, 0x13333, 0x9999);
    Actor_WalkToAndWait(26, 216, 0x254);
    Actor_WalkToAndWait(26, 188, 0x268);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    FieldScene_CallPairWith10(26, 0x5000);
    Engine_ActorJump(26, 2, 0);
    Engine_ActorSetAnimation(26, 4);
    Event_ShowMessage(26, 0);
    Actor_SetPosition(20, 0xb40000, 0x3090000);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkToAndWait(20, 180, 0x298);
    Actor_FaceDirection(20, 0xd000, 0);
    FieldScene_RunStepThen10(0x2014);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(22, 0x3000, 0);
    Actor_ShowEmote(26, 0x101, 60);
    Engine_ActorRunRepeatedMotion(20, 1);
    FieldScene_RunStepThen10(0x2014);
    Engine_ActorStartRepeatedMotion(21, 2);
    FieldScene_RunStepThen10(21);
    Actor_FaceDirection(20, 0x5000, 20);
    Engine_ActorSetAnimation(20, 3);
    FieldScene_RunStepThen10(0x6014);
    Engine_ActorJump(26, 2, 20);
    Engine_ActorSetAnimation(26, 4);
    FieldScene_RunStepThen10(26);
    Actor_WalkToAndWait(20, 182, 0x280);
    Actor_FaceDirection(20, 0xd000, 0);
    FieldScene_RunStepThen10(0x8014);
    Actor_ShowEmote(26, 0x100, 20);
    Engine_ActorStartRepeatedMotion(26, 2);
    FieldScene_RunStepThen10(26);
    Engine_ActorSetAnimationAndWait(20, 3);
    FieldScene_CallPairWith10(22, 0);
    Engine_ActorRunRepeatedMotion(22, 1);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x19999, 0xcccc);
    Engine_ActorEnableActionCallback(22, FuneKanpan_LeaveActions);
    Actor_SetSpeed(21, 0x19999, 0xcccc);
    Actor_WalkToAndWait(21, 168, 0x278);
    Engine_ActorEnableActionCallback(21, FuneKanpan_LeaveActions);
    Engine_EventWait(80);
    Engine_ActorEnableActionCallback(26, FuneKanpan_LeaveActions);
    Engine_EventWait(40);
    FieldScene_CallPairWith10(20, 0x8000);
    FieldScene_RunStepThen10(0x2014);
    FieldScene_CallPairWith10(ACTOR_PARTY_LEADER, 0xe000);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(17);
}

void FuneKanpan_PlaceLeaderOnDeck(void)
{
    struct FieldActor *record;

    Camera_MoveTo(0xe80000, -1, 0x2a40000, 0);
    Engine_MapRedraw();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x2a40000);
    record = Object_GetById(0);
    {
        u16 shown = 0x4000;

        record->facing = shown;
    }
    Engine_TaskWait(1);
}
