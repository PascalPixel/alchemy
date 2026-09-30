/* Set the deck's slot actors 8 to 19 out for the crossing: every slot
   value to its top band, the slots active, the modes cleared, the standing
   slots' depths kept, and each slot placed by its value. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

extern u16 FuneKanpan_SlotValue[];
extern s32 FuneKanpan_SlotMode[];
extern s32 FuneKanpan_SlotDepth[];
void OverlayObject_ActivateSlotWithMode3(s32 actor);
void SceneEffect_SelectSlotValueAndPosition(s32 actor, s32 slot, s32 row);
void ObjectVisual_CopyAttributes(s32 actor, s32 source);

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];
s32 BuildMotionCountdown(s32, s16);

extern s32 FuneKanpan_LayerScroll[];
extern s32 FuneKanpan_LayerSpeed[];
extern u8 FuneKanpan_CrewScript[];
extern u8 MsgFuneFinallyReachedTolbi[];
void Event_CallWithLastActiveObjectId(u8 *script);
void FieldScene_RunStepThen10(s32 step);
void FieldScene_CallPairWith10(s32 actor, s32 facing);

extern u8 MsgFuneRobinDontTalkLikeShouldnt[];
extern u8 MsgFuneRobinTalkedPassengersDidntTour[];
extern u8 MsgFuneSeeYoureGoingGoFor[];
#define ACTOR_FLAGS_OFFSET 90
void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, s32 b);

extern u8 FuneKanpan_ClosingCrewScript[];
extern u8 FuneKanpan_LeaveActions[];
extern u8 MsgFuneHowWasRobinDidExplore[];
void FieldScene_RunScene3af_02000bb8(void);

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
    Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
    Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
    Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
    Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Map_CopyCellAttributes(17, 38, 5, 2, x, y);
    }
}

void DialogueLayout_ConfigureTwoRegions(void)
{
    Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
    Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

void FieldScene_RunStepThen10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Actor_FaceDirection(a, b, 10);
}

/* The ship reaches Tolbi: the crew gathers on deck, the captain announces
   the landing and the party walks down the gangway. */
void FuneKanpan_ArriveAtTolbi(void)
{
    Event_Begin();
    FuneKanpan_LayerScroll[0] = 0x40000;
    FuneKanpan_LayerSpeed[0] = -0x8000;
    Event_CallWithLastActiveObjectId(FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(21, 0xb60000, 0x26a0000);
    Actor_Get(21)->facing = 0xc000;
    Actor_SetPosition(20, 0xda0000, 0x2040000);
    Actor_Get(20)->facing = 0xb000;
    Actor_SetPosition(22, 0xcc0000, 0x20e0000);
    Actor_Get(22)->facing = 0xb000;
    Actor_SetPosition(23, 0, 0);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Task_Wait(1);
    gEventWork->start_transition = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 182, 0x214);
    Actor_FaceDirection(21, 0xb000, 40);
    Event_SetMessage((s32)MsgFuneFinallyReachedTolbi);
    FieldScene_RunStepThen10(21);
    Actor_RunRepeatedMotion(20, 2);
    Actor_SetAnimation(20, 4);
    FieldScene_RunStepThen10(0x6014);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(22, 0x101, 60);
    Actor_SetAnimation(20, 3);
    FieldScene_RunStepThen10(0x6014);
    Actor_SetAttachedEffect(21, 0x102);
    Actor_SetAttachedEffect(22, 0x102);
    Event_Wait(80);
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
    Actor_RunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(0x5015);
    Actor_SetAnimationAndWait(20, 3);
    FieldScene_CallPairWith10(22, 0xd000);
    FieldScene_RunStepThen10(0x9016);
    Actor_FaceDirection(20, 0x8000, 20);
    Actor_SetAnimation(20, 4);
    FieldScene_RunStepThen10(0xa014);
    Actor_WalkToAndWait(20, 204, 0x218);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_WalkToAndWait(20, 182, 0x224);
    Actor_WalkToAndWait(20, 182, 0x250);
    Actor_WalkTo(20, 182, 0x298);
    Event_Wait(40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(16);
}

/* Sets up actors 1, 2, and 3 from three source records, runs their
 * animations and a wait loop gated on actor 0, then clears a flag byte
 * at +90 on actors 21 and 22 before finishing the scene. */
void FieldScene_RunThreeActorEncounter(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    /* For each of actors 1, 2, and 3: fetch a source record, and if one
     * exists, copy its fields at +8 and +16 into the actor. */
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_GERALD, 194, 0x280);
    Actor_WalkTo(ACTOR_IVAN, 198, 0x28e);
    Actor_WalkToAndWait(ACTOR_MIA, 194, 0x2a0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    FieldScene_CallPairWith10(3, 0x8000);
    FieldScene_CallPairWith10(22, 0);
    Event_SetMessage((s32)MsgFuneRobinTalkedPassengersDidntTour);
    FieldScene_RunStepThen10(22);
    FieldScene_CallPairWith10(21, 0xd000);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_ShowEmote(22, 0x100, 20);
    Actor_RunRepeatedMotion(22, 1);
    Event_OpenMessage(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (Event_ChooseYesNo(0, 0) == 1) {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        FieldScene_RunStepThen10(2);
        FieldScene_CallPairWith10(3, 0xa000);
        Actor_SetAnimation(ACTOR_MIA, 3);
        FieldScene_RunStepThen10(3);
        FieldScene_CallPairWith10(1, 0x6000);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_OpenMessage(ACTOR_GERALD, 0);
        L_02003dfa:;
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
            Event_SetMessage((s32)MsgFuneRobinDontTalkLikeShouldnt);
            Event_OpenMessage(ACTOR_IVAN, 0);
            goto L_02003dfa;
        }
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(22, 3);
    Event_SetMessage((s32)MsgFuneSeeYoureGoingGoFor);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    /* Clear the low bit of the flag byte on actor 22. */
    *(u8 *)((s32)Object_GetById(22) + ACTOR_FLAGS_OFFSET) &= 254;
    Actor_WalkToAndWait(22, 162, 0x27a);
    Event_Wait(1);
    bits = 1;
    {
        /* Set the low bit of the flag byte on actor 22. */
        u8 *record = ((u8 *(*)())Object_GetById)(22);
        u8 value = record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = value | bits;
    }
    /* Clear the low bit of the flag byte on actor 21. */
    *(u8 *)((s32)Object_GetById(21) + ACTOR_FLAGS_OFFSET) &= 254;
    Actor_WalkToAndWait(21, 162, 0x2a4);
    Event_Wait(1);
    {
        /* Set the low bit of the flag byte on actor 21. */
        u8 *record = ((u8 *(*)())Object_GetById)(21);

        bits |= record[ACTOR_FLAGS_OFFSET];
        record[ACTOR_FLAGS_OFFSET] = bits;
    }
    Actor_FaceDirection(22, 0x3000, 0);
    FieldScene_CallPairWith10(21, 0xd000);
    FieldScene_RunStepThen10(22);
    /* Finish actors 1, 2, and 3 with the same target values used earlier. */
    Actor_WalkTo(ACTOR_GERALD, 180, 0x28e);
    Actor_WalkTo(ACTOR_IVAN, 180, 0x28e);
    Actor_WalkToAndWait(ACTOR_MIA, 180, 0x28e);
    Actor_Destroy(ACTOR_GERALD);
    Actor_Destroy(ACTOR_IVAN);
    Actor_Destroy(ACTOR_MIA);
    GameFlag_Set(0x903);
    Event_End();
}

/* On deck a sailor asks how the party found the ship; when Isaac does not
   want to talk, the captain and the crew gather and the scene closes. */
void FuneKanpan_RunRobinTalk(void)
{
    Event_Begin();
    Event_CallWithLastActiveObjectId(FuneKanpan_ClosingCrewScript);
    Task_Wait(1);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
    Actor_ShowEmote(22, 0x100, 0);
    Actor_RunRepeatedMotion(22, 1);
    FieldScene_CallPairWith10(22, 0x5000);
    Event_SetMessage((s32)MsgFuneHowWasRobinDidExplore);
    Event_OpenMessage(0x2016, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        FieldScene_RunStepThen10(0x2016);
        Event_End();
        return;
    }
    gEventWork->message++;
    Event_AskYesNo(0x2016, 0);
    FieldScene_RunScene3af_02000bb8();
    Actor_SetPosition(26, 0xd80000, 0x24c0000);
    Actor_SetSpeed(26, 0x13333, 0x9999);
    Actor_WalkToAndWait(26, 216, 0x254);
    Actor_WalkToAndWait(26, 188, 0x268);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    FieldScene_CallPairWith10(26, 0x5000);
    Actor_Jump(26, 2, 0);
    Actor_SetAnimation(26, 4);
    Event_ShowMessage(26, 0);
    Actor_SetPosition(20, 0xb40000, 0x3090000);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkToAndWait(20, 180, 0x298);
    Actor_FaceDirection(20, 0xd000, 0);
    FieldScene_RunStepThen10(0x2014);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(22, 0x3000, 0);
    Actor_ShowEmote(26, 0x101, 60);
    Actor_RunRepeatedMotion(20, 1);
    FieldScene_RunStepThen10(0x2014);
    Actor_StartRepeatedMotion(21, 2);
    FieldScene_RunStepThen10(21);
    Actor_FaceDirection(20, 0x5000, 20);
    Actor_SetAnimation(20, 3);
    FieldScene_RunStepThen10(0x6014);
    Actor_Jump(26, 2, 20);
    Actor_SetAnimation(26, 4);
    FieldScene_RunStepThen10(26);
    Actor_WalkToAndWait(20, 182, 0x280);
    Actor_FaceDirection(20, 0xd000, 0);
    FieldScene_RunStepThen10(0x8014);
    Actor_ShowEmote(26, 0x100, 20);
    Actor_StartRepeatedMotion(26, 2);
    FieldScene_RunStepThen10(26);
    Actor_SetAnimationAndWait(20, 3);
    FieldScene_CallPairWith10(22, 0);
    Actor_RunRepeatedMotion(22, 1);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x19999, 0xcccc);
    Actor_EnableActionCallback(22, FuneKanpan_LeaveActions);
    Actor_SetSpeed(21, 0x19999, 0xcccc);
    Actor_WalkToAndWait(21, 168, 0x278);
    Actor_EnableActionCallback(21, FuneKanpan_LeaveActions);
    Event_Wait(80);
    Actor_EnableActionCallback(26, FuneKanpan_LeaveActions);
    Event_Wait(40);
    FieldScene_CallPairWith10(20, 0x8000);
    FieldScene_RunStepThen10(0x2014);
    FieldScene_CallPairWith10(ACTOR_PARTY_LEADER, 0xe000);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(20, 3);
    gEventWork->start_transition = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(17);
}

void FieldScene_RunScene3af_02004218(void)
{
    s32 record;

    Camera_MoveTo(0xe80000, -1, 0x2a40000, 0);
    Map_Redraw();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x2a40000);
    record = (s32)Object_GetById(0);
    {
        s32 shown = 0x4000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
}
