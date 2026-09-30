#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
#include "CALL.H"
#include "SCENE_IDS.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];
s32 BuildMotionCountdown(s32, s16);

void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);

struct FlyBy {
    u8 unknown_00[0x64];
    s16 step;
};

s32 Engine_RandomNext();

extern u8 FuneKanpan_SceneTableA[];
extern u8 FuneKanpan_SceneTableB[];
extern u8 FuneKanpan_SceneTableC[];

/* The deck's placements, chosen by the story flags the voyage has set. */
extern u8 gFuneKanpanPlacementsFlag93e[];
extern u8 gFuneKanpanPlacementsFlag927[];
extern u8 gFuneKanpanPlacementsFlag928[];
extern u8 gFuneKanpanPlacementsFlag911[];
extern u8 gFuneKanpanPlacements[];

/* The deck's events, chosen by the story flags the voyage has set. */
extern u8 gFuneKanpanEventsFlag93e[];
extern u8 gFuneKanpanEventsFlag8a0[];
extern u8 gFuneKanpanEventsFlag928[];
extern u8 gFuneKanpanEvents[];

extern u8 MsgFuneHaveMakeThemPromiseHelp[];
extern u8 MsgFuneIfWeDontLeaveSoon[];
extern u8 MsgFuneNowWeHaveProtectShip[];
extern u8 MsgFuneSomebodyStopThem[];
extern u8 MsgFuneTheyCantPlanningMutiny[];
extern u8 MsgFuneToldWereLeavingSoonSet[];
extern u8 MsgFuneHeadedColosso[];
extern u8 FuneKanpan_RandomActorActions[];

extern struct EventWork *gEventWork;
void FieldScene_RunScene3af_02000bb8();
void FieldScene_RunScene3af_02000bf0();
void Battle_Reset();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SnapHeadingAndOffset();
void Event_SetValue170();
void AudioCommand_Play();

extern u8 MsgFuneTheresNothingWeCanDo[];
extern u8 MsgFuneWonderCouldHaveHappened[];
extern u8 FuneKanpan_PresentationActionsA[];
extern u8 FuneKanpan_PresentationActionsB[];
void Battle_ResetEffectCounter();
void FieldScene_RunScene3af_02000bf0(void);
void FieldScene_RunStepThen10(s32 a);

extern u8 MsgFuneDidntDoAnything[];

/* The map work pointer heads the field's IWRAM pointer block; the event
   work pointer is its twentieth word. */
extern u8 *gMapWork;
extern s32 FuneKanpan_WaveAngleX;
extern s32 FuneKanpan_WaveAngleY;
extern s32 FuneKanpan_LayerScroll[];
extern s32 FuneKanpan_LayerSpeed[];
void FuneKanpan_ApplyEntryState(void);

void SceneState_ApplyFiveRectsAtColumn78(void);
void DialogueLayout_ConfigureTwoRegions(void);
void SceneEffect_InitSlotsEightToNineteen(void);
void SceneState_InitActorSlots8To19(void);
void SceneState_ConfigureEntries8Through19(void);
void FuneKanpan_RockDeck(void);
void FieldScene_RunScene3af_02001b58(void);
void FieldScene_RunScene3af_02001a98(void);
void FieldScene_RunActorTwentyDialogueSequence(void);
void FieldScene_RunShipDeckEventScript(void);
void FuneKanpan_RunDeckCrewScene(void);
void FieldScene_RunActorSequence(void);
void FuneKanpan_RunJumpScene(void);
void FieldScene_ConfigureLeadActors(void);
void FieldScene_ConfigureThreeActors(void);
void FieldScene_RunPartyRosterScene(void);
void FuneKanpan_ArriveAtTolbi(void);
void FieldScene_RunScene3af_02001920(void);
void FieldScene_RunScene3af_0200185c(void);
void SceneActor_PlaceActors20To27(void);
void FuneKanpan_PlaceRandomDeckActors(void);

/* The saved game as halfwords. */
extern s16 gCell[][1];

void Engine_ActorSetSpritePriority();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
s32 IwramUnsignedRemainder();
void Engine_ActorEnableActionCallback();

extern u8 FuneKanpan_LeadActionsA[];
extern u8 FuneKanpan_LeadActionsB[];
extern u8 FuneKanpan_LeadActionsC[];
extern u8 FuneKanpan_CrewScript[];
void Event_CallWithLastActiveObjectId();
void FieldScene_RunScene3af_02004218(void);

extern u8 FuneKanpan_CrewScriptE[];
void Engine_EventBegin();
void Engine_TaskWait();
void Engine_ActorDestroy();
void FuneKanpan_RunRobinTalk();
void Engine_EventEnd();

extern u8 MsgFuneAyeCaptainSeaMonsters[];
extern u8 MsgFunePreparationsReady[];
void FieldScene_RunScene3af_02001c14();
void FieldScene_CallPairWith10();
void FieldScene_RunScene3af_02000bb8(void);

extern u8 MsgFuneArrgh[];
extern u8 MsgFuneNoUseLate[];
void FieldScene_RunStepThen10();
void WaitFrames();
void ObjectDispatch_SetSingleChildField26(void *, s32);
void ObjectMotion_SetHorizontalPositionWithTerrain();
void ObjectGroup_ConfigureChildValue();
void *ObjectMotion_SetPositionAndReset();
void ObjectMotion_ResetAndSetPosition();
void Motion_SetVarCbAndRefresh();
void Event_SetValue1d8();
void Graphics_EnableObjLayerAndCallbacks();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void *Event_SetStatus1c6();
void Object_RefreshSelectorById();
void ObjectMotion_ResetAndSetPositionInMode2();
void Ui_SetRenderResultFromObject();
void Event_ClearStatus1c6();
void Event_WaitValue1c8Frames();
extern u8 FuneKanpan_DeckEventActions[];
extern u8 FuneKanpan_CrewScriptB[];

extern u8 FuneKanpan_CrewActionsE[];
extern u8 FuneKanpan_CrewScriptC[];
extern u8 FuneKanpan_CrewActionsD[];
extern u8 FuneKanpan_CrewActionsB[];
extern u8 FuneKanpan_CrewActionsA[];
extern u8 FuneKanpan_CrewActionsC[];
extern u8 FuneKanpan_SailorActions[];
void Engine_ActorSetChildValue();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetAnimation();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_AudioPlayCue();
void Object_SetActionCallbackAndRefreshById();
void Engine_EventCloseScreen();
void Engine_ActorStop();
void Engine_EventRequestExit();

extern u8 FuneKanpan_CrewScriptD[];
extern u8 FuneKanpan_DeckActionsA[];
extern u8 FuneKanpan_DeckActionsB[];
extern u8 FuneKanpan_DeckActionsC[];
void ObjectDispatch_SetSingleChildField26Far(struct FieldActor *object, s32 value);
void Event_ClearStatus1c6Far();
void Event_WaitValue1c8FramesFar();

extern struct GameState gGameState;

/* FAKEMATCH: the game state written as rows of halfwords keeps the
 * base-plus-index address form, where its fields fold the offsets into the
 * pool. */
union GameStateRows {
    u8 bytes[512][2];
    u16 halves[512][1];
};

void OverlayObject_DecayFields24And28();
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();
void Engine_CameraFollowActor();
void Engine_ActorWalkToAndWait();
void Engine_ActorJump();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ActorFaceDirection();
void Engine_ActorWalkTo();
void Engine_ActorSetAttachedEffect();

extern u8 MsgFuneCanSeeLand[];
extern u8 MsgFuneThankRobinDidGoodAgainst[];
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

extern u8 MsgFuneDontRowAnymore[];
extern u8 MsgFuneMadeFinallyLucky[];
extern u8 MsgFuneMakeOldMan[];
extern u8 MsgFuneOhhhHaventWorkout[];
extern u8 MsgFuneOtherPassengersAlready[];
extern u8 MsgFuneRowingShipMore[];
extern u8 MsgFuneThanksHardWork[];
extern u8 MsgFuneTotallyLostOcean[];
void FieldScene_RunStepThen10(s32 actor);
void Engine_ActorStartAction(s32 actor);
extern const u8 FuneKanpan_RosterActions[];

struct MapLayer {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    s32 *camera;
    u8 unknown_04[16];
    struct MapLayer layers[8];
};

/* The deck's work, laid out in order just past the overlay's image: the
 * phases of the four drifting slots, the slot work the deck scenes keep and
 * the eight slot values whose bands pace them, the two wave angles, the
 * scroll of layer 5 and its speed. */
u16 FuneKanpan_SlotPhase[4] = { 0 };

s32 FuneKanpan_WaveAngleY = 0;
s32 FuneKanpan_SlotWork = 0;

u16 FuneKanpan_SlotValue[8] = { 0 };

s32 FuneKanpan_LayerScroll[2] = { 0 };

s32 FuneKanpan_WaveAngleX = 0;
s32 FuneKanpan_DeckSpare = 0;

s32 FuneKanpan_LayerSpeed[2] = { 0 };

void FuneKanpan_PlaceDeckActors(s32 a0, s32 a1);
s32 SceneState_FindFirstSetFlagOfGroup(u32 sel);

s32 SceneActor_OscillateHeightBetweenLimits(struct FieldActor *actor)
{
    s16 *flag = (s16 *)&actor->unknown_66;
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = actor->y.fixed - (((u32)(Random_Next() << 15)) >> 16) - 0x8000;
        actor->y.fixed = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = actor->y.fixed + (((u32)(Random_Next() << 15)) >> 16) + 0x8000;
        actor->y.fixed = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 SceneActor_SetFacingFromSample(struct FieldActor *actor)
{
    u32 v = ((u32)(Random_Next() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        actor->facing = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        actor->facing = t << 8;
    }
    return 1;
}

/* A deck object's update: wait a random while, fly to its mark, make actor 21 turn and react, then fly off and start over. */
s32 FuneKanpan_UpdateFlyByForActor21(struct FieldActor *obj)
{
    struct FlyBy *gull;

    gull = (struct FlyBy *)obj;
    switch (gull->step) {
    case 0:
        if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
            gull->step++;
        }
        break;
    case 1:
        gull->step++;
        break;
    case 2:
        obj->velocity_y = 0x40000;
        obj->speed = 0x40000;
        obj->acceleration = 0x20000;
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1080000, 0, 0x2960000);
        gull->step++;
        break;
    case 3:
        gull->step++;
        break;
    case 4:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
            Engine_AudioPlayCue(152);
            if (obj->rise_enabled != 0) {
                Call3(Engine_ActorFaceDirection, 21, 0xb000, 0);
            } else {
                Call3(Engine_ActorFaceDirection, 21, 0x5000, 0);
            }
            if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                Object_GetById(21)->velocity_y = 0x20000;
            } else {
                Call3(Engine_ActorShowEmote, 21, 0x103, 0);
                Object_GetById(21)->velocity_y = 0x60000;
            }
        }
        break;
    case 5:
        gull->step++;
        break;
    case 6:
        gull->step++;
        obj->velocity_y = 0x40000;
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        if (obj->rise_enabled != 0) {
            Call4(Engine_ObjectSetPosition, (s32)obj, 0xfc0000, 0, 0x2860000);
        } else {
            Call4(Engine_ObjectSetPosition, (s32)obj, 0x1000000, 0, 0x2ae0000);
        }
        break;
    case 7:
        gull->step++;
        break;
    case 8:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
        }
        break;
    case 9:
        gull->step = 0;
        break;
    }
    return 1;
}

/* A deck object's update: wait a random while, fly to its mark, make actor 22 turn and react, then fly off and start over. */
s32 FuneKanpan_UpdateFlyByForActor22(struct FieldActor *obj)
{
    struct FlyBy *gull;

    gull = (struct FlyBy *)obj;
    switch (gull->step) {
    case 0:
        if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
            gull->step++;
        }
        break;
    case 1:
        gull->step++;
        break;
    case 2:
        obj->velocity_y = 0x40000;
        obj->speed = 0x40000;
        obj->acceleration = 0x20000;
        Call4(Engine_ObjectSetPosition, (s32)obj, 0xb00000, 0, 0x2b80000);
        gull->step++;
        break;
    case 3:
        gull->step++;
        break;
    case 4:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
            Engine_AudioPlayCue(152);
            if (obj->rise_enabled != 0) {
                Call3(Engine_ActorFaceDirection, 22, 0xd000, 0);
            } else {
                Engine_ActorFaceDirection(22, 0, 0);
            }
            if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                Object_GetById(22)->velocity_y = 0x20000;
            } else {
                Call3(Engine_ActorShowEmote, 22, 0x103, 0);
                Object_GetById(22)->velocity_y = 0x60000;
            }
        }
        break;
    case 5:
        gull->step++;
        break;
    case 6:
        gull->step++;
        obj->velocity_y = 0x40000;
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        if (obj->rise_enabled != 0) {
            Call4(Engine_ObjectSetPosition, (s32)obj, 0xb80000, 0, 0x2a00000);
        } else {
            Call4(Engine_ObjectSetPosition, (s32)obj, 0xca0000, 0, 0x2b40000);
        }
        break;
    case 7:
        gull->step++;
        break;
    case 8:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
        }
        break;
    case 9:
        gull->step = 0;
        break;
    }
    return 1;
}

s32 SceneActor_SetWord28RandomlyOneIn40(struct FieldActor *actor)
{
    if ((((u32)(Random_Next() * 40)) >> 16) == 0)
        actor->velocity_y = 0x40000;
    return 1;
}

void OverlayObject_DecayFields24And28(struct FieldActor *actor)
{
    if (actor->scale_x > 0x10000) {
        actor->scale_x += 0xFFFFF800;
        actor->scale_y += 0xFFFFF800;
    }
}

/* Counts down the wait timer; when it runs out, turns to a random heading and waits again. */
s32 FuneKanpan_IdleTurn(u8 *actor)
{
    if (actor[98] != 0) {
        actor[98]--;
    } else {
        u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

        if (roll > 200) {
            s32 dir = 0xd000;

            *(u16 *)(actor + 6) = dir;
        } else if (roll > 100) {
            s32 dir = 0x5000;

            *(u16 *)(actor + 6) = dir;
        } else {
            *(u16 *)(actor + 6) = 0;
        }
        actor[98] = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
    }
    return 1;
}

/* Contiguous unnamed leaf-owner run for resource_3af. */
u8 *SceneData_GetTablec994(void)
{
    return FuneKanpan_SceneTableA;
}

u8 *SceneData_GetTablecb44(void)
{
    return FuneKanpan_SceneTableB;
}

u8 *SceneData_GetTablecb64(void)
{
    return FuneKanpan_SceneTableC;
}

/* The actors placed on the deck. Once flag 0x911 is set the table adjusts
 * three of its entries by flags 0x925 and 0x922 before it is used. */
u8 *FuneKanpan_GetPlacements(void)
{
    s32 v;

    if (GameFlag_IsSet(0x93e))
        return gFuneKanpanPlacementsFlag93e;
    if (GameFlag_IsSet(0x927))
        return gFuneKanpanPlacementsFlag927;
    v = GameFlag_IsSet(0x928);
    if (v != 0)
        return gFuneKanpanPlacementsFlag928;
    if (GameFlag_IsSet(0x911)) {
        if (GameFlag_IsSet(0x925)) {
            gFuneKanpanPlacementsFlag911[0x14E] = v;
            gFuneKanpanPlacementsFlag911[0x1AE] = 2;
            gFuneKanpanPlacementsFlag911[0x1C6] = 2;
        } else if (GameFlag_IsSet(0x922)) {
            gFuneKanpanPlacementsFlag911[0x1AE] = 1;
            gFuneKanpanPlacementsFlag911[0x1C6] = 1;
        }
        return gFuneKanpanPlacementsFlag911;
    }
    return gFuneKanpanPlacements;
}

/* What the deck answers. */
u8 *FuneKanpan_GetEvents(void)
{
    if (GameFlag_IsSet(0x93e))
        return gFuneKanpanEventsFlag93e;
    if (GameFlag_IsSet(0x8A0))
        return gFuneKanpanEventsFlag8a0;
    if (GameFlag_IsSet(0x928))
        return gFuneKanpanEventsFlag928;
    return gFuneKanpanEvents;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneNowWeHaveProtectShip);
        Event_ShowMessage(21, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(21, 2);
            Event_SetMessage((s32)MsgFuneSomebodyStopThem);
            Event_ShowMessage(21, 0);
            rec7 = (s32)Object_GetById(21);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(21, (u32)FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(21, 0x103, 0);
            Actor_StartRepeatedMotion(21, 3);
            Event_SetMessage((s32)MsgFuneToldWereLeavingSoonSet);
            Event_ShowMessage(21, 0);
        }
    }
    Event_End();
}

void FieldScene_RunScene3afSequenceA(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneHaveMakeThemPromiseHelp);
        Event_ShowMessage(24, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(24, 2);
            Event_SetMessage((s32)MsgFuneTheyCantPlanningMutiny);
            Event_ShowMessage(24, 0);
            rec7 = (s32)Object_GetById(24);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(24, (u32)FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(24, 0x103, 0);
            Actor_StartRepeatedMotion(24, 3);
            Event_SetMessage((s32)MsgFuneIfWeDontLeaveSoon);
            Event_ShowMessage(24, 0);
        }
    }
    Event_End();
}

void SceneDialogue_RunActor21Line(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgFuneHeadedColosso);
    Event_AskYesNo(21, 0);
    Event_End();
}

void FieldScene_RunScene3af_02000bb8(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x271) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
        GameFlag_Set(0x271);
    }
}

void FieldScene_RunScene3af_02000bf0(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x272) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
        GameFlag_Set(0x272);
    }
}

void FuneKanpan_RunDeckStateEvent(void)
{

    s32 v5;
    u8 *p6;

    p6 = *(s32 *)&gEventWork;
    Battle_Reset();
    v5 = 0;
    switch (*(s16 *)(((s32)p6 + 0x16c))) {
    case 1:
        v5 = 1;
        FieldScene_RunScene3af_02000bb8();
        break;
    case 3:
        v5 = 1;
        FieldScene_RunScene3af_02000bf0();
        break;
    }
    if (v5 != 0) {
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x9999, 0x4ccc);
        Call3(ObjectMotion_SnapHeadingAndOffset, 0, 1, -10);
        Battle_WaitMode0(10);
    } else {
        AudioCommand_Play(123);
    }
    Event_SetValue170(*(s16 *)(((s32)p6 + 0x16c)));
}

/* Gated on scene condition 0x911; when set, configures actors 20, 22 and
 * 23 (position, pose, movement and sprite flags) and their attached
 * effects, then advances the shared scene phase. */
void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    u8 *record;

    if (GameFlag_IsSet(0x911) != 0) {
        Event_Begin();
        Battle_ResetEffectCounter();
        Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
        Camera_SetSpeed(0x19999, 0x3333);
        Camera_MoveTo(0xbe0000, -1, 0x2c40000, 1);
        Camera_WaitForMove();
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage((s32)MsgFuneWonderCouldHaveHappened);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x102, 60);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_RunRepeatedMotion(22, 1);
        Actor_FaceDirection(22, 0x5000, 0);
        FieldScene_RunStepThen10(0x4016);
        Actor_RunRepeatedMotion(20, 1);
        FieldScene_CallPairWith10(20, 0xb000);
        FieldScene_RunStepThen10(20);
        FieldScene_CallPairWith10(23, 0x3000);
        Actor_SetAnimation(23, 3);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(22, 0x101, 40);
        Actor_FaceDirection(22, 0x8000, 20);
        FieldScene_RunStepThen10(0x4016);
        FieldScene_CallPairWith10(23, 0);
        Actor_SetAnimationAndWait(23, 4);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(20, 0x100, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_SetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        FieldScene_CallPairWith10(20, 0xd000);
        Actor_SetAnimation(23, 3);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(60);
        Actor_ShowEmote(22, 0x106, 40);
        FieldScene_CallPairWith10(22, 0x5000);
        Event_SetMessage((s32)MsgFuneTheresNothingWeCanDo);
        Actor_StartRepeatedMotion(22, 1);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x101, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_ShowEmote(22, 0x108, 20);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(23, 0x102, 60);
        FieldScene_RunStepThen10(0x4017);
        FieldScene_CallPairWith10(22, 0x8000);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(20, 0x102, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        FieldScene_CallPairWith10(22, 0x5000);
        Actor_SetAnimation(22, 4);
        FieldScene_RunStepThen10(22);
        Actor_FaceDirection(20, 0xb000, 0);
        Actor_FaceDirection(23, 0x3000, 40);
        Actor_FaceDirection(23, 0, 0);
        Actor_FaceDirection(20, 0xd000, 20);
        Actor_RunRepeatedMotion(22, 2);
        Event_Wait(20);
        FieldScene_RunStepThen10(0x4016);
        Actor_SetAttachedEffect(23, 0x102);
        Actor_SetAttachedEffect(20, 0x102);
        Event_Wait(40);
        Actor_SetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xb60000, -1, 0x2f80000, 1);
        Actor_SetSpeed(23, 0xcccc, 0x6666);
        ((s32 (*)())Engine_ActorEnableActionCallback)(23, (s32)FuneKanpan_PresentationActionsA);
        Actor_SetSpeed(22, 0xcccc, 0x6666);
        ((s32 (*)())Engine_ActorEnableActionCallback)(22, (s32)FuneKanpan_PresentationActionsB);
        Actor_SetSpeed(20, 0xcccc, 0x6666);
        Actor_WalkToAndWait(20, 182, 0x2f8);
        Actor_StartRepeatedMotion(20, 2);
        Actor_ShowEmote(20, 0x100, 60);
        FieldScene_CallPairWith10(20, 0xd000);
        Event_ShowMessageAndWait(20, 0, 20);
        Actor_SetAnimationAndWait(20, 3);
        Actor_Jump(20, 4, 0);
        Actor_FaceDirection(20, 0x3000, 40);
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_MoveTo(0xd80000, -1, 0x3160000, 1);
        {
            /* Set the low bit of the flag byte at +35 of actor 20's record. */
            u8 *record = (u8 *)Object_GetById(20);
            /* FAKEMATCH: a result temporary, not a compound or-assign: the
             * reference merges the byte into the mask's register, which the
             * two-address ORR does only when the result is its own object. */
            u8 bits = 1;

            bits |= record[35];
            record[35] = bits;
        }
        Actor_SetSpeed(20, 0x13333, 0x9999);
        Actor_WalkToAndWait(20, 182, 0x30e);
        Actor_WalkToAndWait(20, 192, 0x328);
        Actor_WalkToAndWait(20, 216, 0x328);
        FieldScene_CallPairWith10(20, 0xd000);
        Actor_RunRepeatedMotion(20, 2);
        FieldScene_RunScene3af_02000bf0();
        Actor_WalkToAndWait(20, 216, 0x31e);
        Actor_SetPosition(20, 0, 0);
        /* Set the fixed-point word at +24 of actor 20's record to 1.0. */
        record = (u8 *)Object_GetById(20);
        *(s32 *)(record + 24) = 0x10000;
        /* Set the fixed-point word at +28 of actor 20's record to 1.0. */
        record = (u8 *)Object_GetById(20);
        *(s32 *)(record + 28) = 0x10000;
        GameFlag_Set(0x920);
        Event_End();
    }
}

void FieldScene_RunScene3af_020010a0(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Event_Begin();
            Battle_ResetEffectCounter();
            FieldScene_RunScene3af_020012f0();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            *(u8 *)((s32)Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 232, 0x330);
            Event_Wait(1);
            bits = 1;
            {
                u8 *record = (s32)Object_GetById(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Event_Wait(20);
            Actor_StartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            *(u8 *)((s32)Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 244, 0x324);
            Event_Wait(1);
            {
                u8 *record = ((u8 *(*)())Object_GetById)(20);

                bits |= record[90];
                record[90] = bits;
            }
            Event_Wait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 248, 0x30a);
            Actor_WalkToAndWait(20, 248, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
            Event_End();
        }
    }
}

void FieldScene_RunScene3af_020011c8(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Event_Begin();
            Battle_ResetEffectCounter();
            FieldScene_RunScene3af_020012f0();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            *(u8 *)((s32)Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 202, 0x330);
            Event_Wait(1);
            bits = 1;
            {
                u8 *record = (s32)Object_GetById(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Event_Wait(20);
            Actor_StartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            *(u8 *)((s32)Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 192, 0x324);
            Event_Wait(1);
            {
                u8 *record = ((u8 *(*)())Object_GetById)(20);

                bits |= record[90];
                record[90] = bits;
            }
            Event_Wait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 180, 0x30a);
            Actor_WalkToAndWait(20, 180, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
            Event_End();
        }
    }
}

void FieldScene_RunScene3af_020012f0(void)
{
    u32 i;
    s32 record;

    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0xd80000, -1, 0x3380000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    FieldScene_RunScene3af_02000bf0();
    Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
    Event_Wait(10);
    Actor_SetPosition(20, 0xd80000, 0x3200000);
    Actor_SetSpeed(20, 0x13333, 0x9999);
    Actor_WalkToAndWait(20, 216, 0x32e);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
    Actor_SetAnimationAndWait(20, 4);
    Actor_StartRepeatedMotion(20, 2);
    Actor_ShowEmote(20, 0x100, 20);
    Actor_FaceActor(20, ACTOR_PARTY_LEADER, 20);
    Actor_StartRepeatedMotion(20, 2);
    Event_SetMessage((s32)MsgFuneDidntDoAnything);
    Event_ShowMessageAndWait(20, 0, 20);
    Engine_ActorShowEmote(20, 0x102, 0);
    GameFlag_Set(0x923);
}

/* Restart the deck: reseed the two wave angles, stop layer 5's scroll and
   apply the entry state again. */
s32 FuneKanpan_ResetDeck(void)
{
    u8 **base = &gMapWork;
    u8 *map = base[0] + 0x104;

    Engine_GameFlagClear(0x11c);
    *(s32 *)(base[19] + 0x1c0) = 0x209;
    *(s32 *)(map + 0x1c) = 0;
    FuneKanpan_WaveAngleX = (u16)Engine_RandomNext();
    FuneKanpan_WaveAngleY = (u16)Engine_RandomNext();
    FuneKanpan_LayerScroll[0] = 0;
    FuneKanpan_LayerScroll[1] = 0;
    FuneKanpan_LayerSpeed[0] = 0;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    FuneKanpan_ApplyEntryState();
    return 0;
}

/* The scroll of map layer 5 and its speed (ROCK.C). */

/* Ship deck entry: record the arrival, set the deck by the voyage flags, then run the entrance's scene or place the deck crew. */
void FuneKanpan_ApplyEntryState(void)
{
    s32 flag;

    Engine_GameFlagSet(0x144);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x271);
        Engine_GameFlagClear(0x272);
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        DialogueLayout_ConfigureTwoRegions();
        Engine_ActorSetChildValue(24, 2);
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        DialogueLayout_ConfigureTwoRegions();
    } else if ((flag = Engine_GameFlagIsSet(0x927)) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        SceneEffect_InitSlotsEightToNineteen();
        Value2(Engine_TaskAddCallback, (s32)SceneState_ConfigureEntries8Through19, 0xc80);
        FuneKanpan_LayerScroll[1] = 0x200000;
        FuneKanpan_LayerSpeed[1] = 0x13333;
        Engine_TaskAddCallback((s32)FuneKanpan_RockDeck, 0xc80);
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        FuneKanpan_LayerScroll[1] = flag;
        FuneKanpan_LayerSpeed[1] = flag;
        Engine_TaskAddCallback((s32)FuneKanpan_RockDeck, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x927) == 0) {
        SceneState_InitActorSlots8To19();
    }
    switch (gCell[225][0]) {
    case 4:
        Object_GetById(0)->sprite->priority = 1;
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3af_02001b58();
        } else {
            FieldScene_RunScene3af_02001a98();
        }
        return;
    case 11:
        FieldScene_RunActorTwentyDialogueSequence();
        return;
    case 12:
        FieldScene_RunShipDeckEventScript();
        return;
    case 13:
        FuneKanpan_RunDeckCrewScene();
        return;
    case 14:
        FieldScene_RunActorSequence();
        return;
    case 15:
        FuneKanpan_RunJumpScene();
        return;
    case 16:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            FieldScene_RunScene3af_02001920();
        } else {
            FieldScene_ConfigureLeadActors();
        }
        return;
    case 17:
        FieldScene_ConfigureThreeActors();
        return;
    case 18:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunPartyRosterScene();
        }
        return;
    case 19:
        FuneKanpan_ArriveAtTolbi();
        return;
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        *(s32 *)(gMapWork + 236) = 0x410000;
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
        ((void (*)(void))FuneKanpan_PlaceDeckActors)();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunScene3af_02001920();
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        FieldScene_RunScene3af_0200185c();
    } else if (Engine_GameFlagIsSet(0x925) != 0) {
        SceneActor_PlaceActors20To27();
    } else if (Engine_GameFlagIsSet(0x911) != 0) {
        FuneKanpan_PlaceRandomDeckActors();
    }
}

void FuneKanpan_PlaceRandomDeckActors(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_200c4d8;
    s32 a;

    Engine_ActorSetSpritePriority(27, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(26, 1);
    Engine_ActorSetSpritePriority(24, 1);
    if (Engine_GameFlagIsSet(0x920) != 0) {
        Engine_ActorSetPosition(22, 0xa20000, 0x29a0000);
        record = (s32)Object_GetById(22);
        {
            s32 shown = 0x8000;
        
            *(u16 *)(record + 6) = shown;
        }
        Engine_ActorSetPosition(23, 0, 0);
        Engine_ActorSetPosition(20, 0, 0);
    }
    rec7 = Engine_GameFlagIsSet(0x922);
    if (rec7 != 0) {
        Engine_ActorSetPosition(21, 0x1080000, 0x2be0000);
        record = (s32)Object_GetById(21);
        {
            s32 shown = 0x5000;
        
            *(u16 *)(record + 6) = shown;
        }
        a = (s32)Object_GetById(21);
        record = Engine_RandomNext();
        {
            /* FAKEMATCH: the temporary makes the +60 add come before the +100. */
            s32 t = IwramUnsignedRemainder(record, 90) + 60;

            a += 100;
            *(u16 *)a = t;
        }
        Engine_ActorEnableActionCallback(21, FuneKanpan_RandomActorActions);
        Engine_ActorSetPosition(24, 0xf80000, 0x2a80000);
        a = Value1(Object_GetById, 24);
        record = Engine_RandomNext();
        a += 100;
        *(u16 *)a = (IwramUnsignedRemainder(record, 90) + 60);
        Engine_ActorEnableActionCallback(24, FuneKanpan_RandomActorActions);
        Engine_ActorSetPosition(22, 0, 0);
    } else {
        if (Engine_GameFlagIsSet(0x923) != 0) {
            Call3(Engine_ActorSetPosition, 20, 0xf60000, 0x2000000);
            record = (s32)Object_GetById(20);
            *(u16 *)(record + 6) = rec7;
        }
    }
}

void SceneActor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Actor_SetPosition(21, 0x1060000, 0x2C20000);
    *(u16 *)((u8 *)Object_GetById(21) + 6) = m;
    Actor_SetPosition(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)((u8 *)Object_GetById(24) + 6) = z;
    }
    Actor_SetSpritePriority(24, 1);
    Actor_SetPosition(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)((u8 *)Object_GetById(25) + 6) = x << 8;
    }
    Actor_SetSpritePriority(25, 1);
    Actor_SetPosition(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)((u8 *)Object_GetById(26) + 6) = x << 8;
    }
    Actor_SetPosition(27, 0xBA0000, 0x27B0000);
    *(u16 *)((u8 *)Object_GetById(27) + 6) = m;
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(20, 0, 0);
}

void FieldScene_RunScene3af_0200185c(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0xcc0000, 0x2090000);
    record = (u8 *)Object_GetById(22);
    *(s32 *)(record + 12) = 0x100000;
    bits = 128;
    {
        u8 *record = Object_GetById(22);
        u8 value = record[89];

        record[89] = value | bits;
    }
    Actor_SetSpeed(22, 0x9999, 0x4ccc);
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsA);
    {
        u8 *record = (u8 *)Object_GetById(21);

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}

void FieldScene_RunScene3af_02001920(void)
{
    u8 *record;

    Event_Begin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = (u8 *)Object_GetById(22);
    {
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        u8 *record = (u8 *)Object_GetById(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}

/* Ship deck: place actors 21 to 23 for the crossing, facing them by flag
 * 0x903, and close the encounter when the state is 6. */
void FuneKanpan_PlaceDeckActors(s32 a0, s32 a1)
{
    s32 record;
    s32 v5;

    *(s32 *)(*(s32 *)&gMapWork + 236) = 0x410000;
    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScriptE);
    Engine_TaskWait(1);
    Engine_ActorDestroy(24);
    Call3(Engine_ActorSetPosition, 23, 0xee0000, 0x2720000);
    v5 = 192;
    record = (s32)Object_GetById(23);
    *(u16 *)(record + 6) = (v5 << 6);
    if (Engine_GameFlagIsSet(0x903) != 0) {
        Call3(Engine_ActorSetPosition, 22, 0xa20000, 0x27a0000);
        record = (s32)Object_GetById(22);
        *(u16 *)(record + 6) = (v5 << 6);
        Call3(Engine_ActorSetPosition, 21, 0xa20000, 0x2a40000);
        record = (s32)Object_GetById(21);
        {
            s32 facing = 0xd000; /* FAKEMATCH: word temporary keeps the facing as movs+lsls */

            *(u16 *)(record + 6) = facing;
        }
    } else {
        Engine_ActorSetPosition(22, 0xa00000, 0x28c0000);
        record = (s32)Object_GetById(22);
        *(u16 *)(record + 6) = (v5 << 6);
        Call3(Engine_ActorSetPosition, 21, 0xa60000, 0x29c0000);
        record = (s32)Object_GetById(21);
        {
            s32 facing = 0xb000; /* FAKEMATCH: word temporary keeps the facing as movs+lsls */

            *(u16 *)(record + 6) = facing;
        }
    }
    if (gGameState.entrance == 6) {
        FuneKanpan_RunRobinTalk();
    }
    Engine_EventEnd();
}

void FieldScene_RunScene3af_02001a98(void)
{
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
    Actor_SetPosition(26, 0, 0);
    Actor_SetPosition(27, 0, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(23, 0, 0);
    record = (s32)Object_GetById(23);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(21, 0xe80000, 0x28a0000);
    record = (s32)Object_GetById(21);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Camera_MoveTo(0xe80000, -1, 0x27c0000, 0);
    Map_Redraw();
    Task_Wait(1);
    FieldScene_RunScene3af_02001c14(23, 21);
}

void FieldScene_RunScene3af_02001b58(void)
{
    s32 record;

    Event_Begin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x27c0000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (s32)Object_GetById(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_Stop(22);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(20, 0, 0);
    record = (s32)Object_GetById(20);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(23, 0xe80000, 0x28a0000);
    record = (s32)Object_GetById(23);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    FieldScene_RunScene3af_02001c14(20, 23);
}

void FieldScene_RunScene3af_02001c14(s32 a0, s32 a1)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    FieldScene_RunScene3af_02000bb8();
    Actor_SetPosition(a0, 0xd80000, 0x24c0000);
    Actor_SetSpeed(a0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(a0, 216, 0x258);
    Actor_WalkToAndWait(a0, 218, 0x25c);
    Actor_WalkToAndWait(a0, 234, 0x25c);
    Actor_WalkToAndWait(a0, 236, 0x26a);
    Actor_FaceDirection(a0, 0x5000, 20);
    Actor_SetAnimationAndWait(a0, 3);
    Event_Wait(20);
    FieldScene_CallPairWith10(a1, 0x5000);
    Actor_Jump(a1, 4, 40);
    Actor_StartRepeatedMotion(a1, 2);
    Event_SetMessage((s32)MsgFunePreparationsReady);
    Event_ShowMessageAndWait(a1, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
}

void FieldScene_RunActorTwentyDialogueSequence(void)
{
    extern s32 *Data_03001ebc;

    Event_Begin();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags((s32)Object_GetById(0), 0);
    Data_03001ebc[0x70] = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_SetMessage((s32)MsgFuneAyeCaptainSeaMonsters);
    Event_ShowMessageAndWait(20, 0, 10);
    FieldScene_CallPairWith10(22, 0x5000);
    Actor_Jump(22, 4, 20);
    Actor_StartRepeatedMotion(22, 2);
    Event_ShowMessageAndWait(0x6016, 0, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(11);
}

void FieldScene_RunShipDeckEventScript(void)
{
    void *p8;
    void *p10;
    void *p21;
    void *p22;
    void *p23;
    s32 v;

    Battle_Reset();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScript);
    WaitFrames(1);
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScriptB);
    WaitFrames(1);
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 21, 16252928, 47710208);
    ObjectGroup_ConfigureChildValue(0, 15);
    p8 = (void *)Object_GetById(0);
    ObjectDispatch_SetSingleChildField26(p8, 0);
    *(s32 *)(*(u8 **)&gEventWork + 448) = 514;
    Event_SetStatus1c6();
    Call3(ObjectMotion_SetSpeedParameters, 21, 104857, 52428);
    Call3(ObjectMotion_SetPositionAndReset, 21, 242, 692);
    Call3(ObjectMotion_SetPositionAndReset, 21, 196, 678);
    Call3(ObjectMotion_SetPositionAndReset, 21, 182, 654);
    Motion_SetVarCbAndRefresh(21, 2);
    Event_SetValue1d8((s32)MsgFuneNoUseLate);
    FieldScene_RunStepThen10(40981);
    Call3(ObjectMotion_SetSpeedParameters, 0, 157286, 78643);
    Call3(ObjectMotion_ResetAndSetPosition, 0, 154, 609);
    AudioCommand_Play(146);
    v = 0;
    p21 = (void *)Object_GetById(24);
    *(u16 *)((u8 *)(p21) + 100) = v;
    p22 = (void *)Object_GetById(25);
    *(u16 *)((u8 *)(p22) + 100) = v;
    p23 = (void *)Object_GetById(26);
    *(u16 *)((u8 *)(p23) + 100) = v;
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 24, 2097152, 31719424);
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 25, 5505024, 32505856);
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 26, 1048576, 39059456);
    Call3(ObjectMotion_SetSpeedParameters, 24, 157286, 78643);
    Call3(ObjectMotion_SetSpeedParameters, 25, 157286, 78643);
    Call3(ObjectMotion_SetSpeedParameters, 26, 157286, 78643);
    v = (s32)FuneKanpan_DeckEventActions;
    ObjectMotion_EnableActionAndSetCallback(24, v);
    ObjectMotion_EnableActionAndSetCallback(25, v);
    ObjectMotion_EnableActionAndSetCallback(26, v);
    ObjectGroup_ConfigureChildValue(24, 3);
    ObjectGroup_ConfigureChildValue(25, 3);
    ObjectGroup_ConfigureChildValue(26, 3);
    do {
        WaitFrames(1);
        p10 = (void *)Object_GetById(24);
    } while (*(s16 *)(p10 + 100) == 0);
    FieldScene_RunScene3af_02000bb8();
    /* FAKEMATCH: the do/while sets r0 = 21 first, straight after the call. */
    do {
        Call3(ObjectMotion_ResetAndSetPositionInMode2, 21, 196, 612);
    } while (0);
    Object_RefreshSelectorById(24);
    Battle_WaitMode0(10);
    Event_ClearStatus1c6();
    Event_WaitValue1c8Frames();
    Battle_WaitMode0(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    UiText_ShowCenteredMessage((s32)MsgFuneArrgh, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Event_SetValue170(12);
}

/* Ship deck: place the crew actors, run their action scripts and walk actor
 * 30 and 31 through the deck sequence. */
void FuneKanpan_RunDeckCrewScene(void)
{
    u32 i;
    s32 record;
    s32 action_c80c;
    s32 v6;
    s32 v5;
    s32 action_c7a8;
    s32 action_c764;
    s32 action_c7ec;
    s32 action_c888;

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    record = (s32)Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScriptC);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(31, 0);
    record = (s32)Object_GetById(24);
    Engine_ActorSetSpriteFlags(record, 1);
    record = (s32)Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 1);
    record = (s32)Object_GetById(26);
    Engine_ActorSetSpriteFlags(record, 1);
    record = (s32)Object_GetById(27);
    Engine_ActorSetSpriteFlags(record, 1);
    record = (s32)Object_GetById(28);
    Engine_ActorSetSpriteFlags(record, 1);
    record = (s32)Object_GetById(29);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetPosition, 22, 0x1000000, 0x2800000);
    action_c80c = (s32)FuneKanpan_CrewActionsD;
    Engine_ActorEnableActionCallback(22, action_c80c);
    Call3(Engine_ActorSetPosition, 21, 0x10c0000, 0x2b40000);
    Engine_ActorEnableActionCallback(22, action_c80c);
    Call3(Engine_ActorSetPosition, 24, 0xf20000, 0x25c0000);
    Call3(Engine_ActorSetPosition, 25, 0x1080000, 0x2580000);
    Call3(Engine_ActorSetPosition, 26, 0xfe0000, 0x29c0000);
    Engine_ActorSetPosition(27, 0x11a0000, 0x2920000);
    v6 = 0;
    *(u8 *)((s32)Object_GetById(24) + 99) = v6;
    v5 = 1;
    *(u8 *)((s32)Object_GetById(25) + 99) = v5;
    *(u8 *)((s32)Object_GetById(26) + 99) = v6;
    *(u8 *)((s32)Object_GetById(27) + 99) = v5;
    action_c7a8 = (s32)FuneKanpan_CrewActionsB;
    Engine_ActorEnableActionCallback(24, action_c7a8);
    Engine_ActorEnableActionCallback(25, action_c7a8);
    action_c764 = (s32)FuneKanpan_CrewActionsA;
    Engine_ActorEnableActionCallback(26, action_c764);
    Engine_ActorEnableActionCallback(27, action_c764);
    Engine_ActorSetPosition(20, 0, 0);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(0x190);
    Call3(Engine_ActorSetPosition, 28, 0xfe0000, 0x2e40000);
    Call3(Engine_ActorSetPosition, 29, 0x180000, 0x24a0000);
    Call3(Engine_ActorSetSpeed, 28, 0x19999, 0xcccc);
    Call3(Engine_ActorSetSpeed, 29, 0x19999, 0xcccc);
    Call3(Engine_ActorSetDestination, 29, 172, 0x284);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 28, 200, 0x294);
    Call3(Engine_ActorSetSpeed, 0, 0x40000, 0x20000);
    Call3(Engine_ActorSetDestination, 0, 174, 0x26c);
    Engine_ObjectMotionSetPositionAndCommit(28, 180, 0x244);
    Engine_AudioPlayCue(146);
    action_c7ec = (s32)FuneKanpan_CrewActionsC;
    Engine_ActorEnableActionCallback(28, action_c7ec);
    Engine_ActorEnableActionCallback(29, action_c7ec);
    Engine_AudioPlayCue(240);
    Call3(Engine_ActorSetPosition, 31, 0x860000, 0x2520000);
    Engine_ActorEnableActionCallback(31, (s32)FuneKanpan_CrewActionsE);
    Engine_EventWait(10);
    Call3(Engine_ActorSetPosition, 30, 0x860000, 0x2480000);
    Call3(Engine_ActorSetSpeed, 30, 0x40000, 0x20000);
    record = (s32)Object_GetById(30);
    *(s32 *)(record + 40) = 0x80000;
    Engine_ObjectMotionSetPositionAndCommit(30, 186, 0x264);
    record = (s32)Object_GetById(30);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 30, 0x20000, 0x10000);
    Engine_ObjectMotionSetPositionAndCommit(30, 216, 0x258);
    FieldScene_CallPairWith10(30, 0xc000);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    action_c888 = (s32)FuneKanpan_SailorActions;
    Engine_ActorEnableActionCallback(30, action_c888);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(28, action_c888);
    Engine_EventWait(10);
    Object_SetActionCallbackAndRefreshById(29, action_c888);
    Engine_EventWait(20);
    Engine_AudioPlayCue(147);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_EventWait(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    UiText_ShowCenteredMessage((s32)MsgFuneArrgh, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventRequestExit(13);
}

/* EXACT: 856 bytes, candidate 856, 0 differing halfwords, 0 halfword
 * edits (2026-09-27). FieldScene_RunActorSequence in
 * FIELD/FUNE_KANPAN/DECK_SEQ.C is a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Complete extent 020022c0..02002618: first zero/pool 23b8..23d4,
 * second zero/pool 252c..2554, return 2606 and final pool 2608..2614.
 * The 24fc call is ActorSetDestination (runtime veneer 0200c344), not
 * ActorWalkToAndWait (0200c35c); retained this independently verified fix.
 * Three structural trials: sharing the early actor-result scalar with the
 * loop counter produced 860 bytes / 361 differing halfwords / 135 edits,
 * adding unwanted saved-register copies to the initial facing stores.
 * One shared halfword-zero record across both phases gave 856 / 85 / 57:
 * second zero uses saved r6 and the store order matches, but actor stays r5,
 * first zero also moves to r6, and the second pool remains four bytes early.
 * Phase-scoped actor pointers gave 848 / 211 / 94, removing the reference's
 * saved-pointer copies for actors 30 and 0; the single shared actor survives.
 * Retained the original lifetimes plus the destination-call correction.
 * Previous remaining: actor/counter r5/r6 versus r6/r5, second zero in r2 versus r5,
 * second pool four bytes early, and callback address hoisted before its
 * speed call. Those lifetime-only trials are closed.
 * New interface model: exact FIELD_EVENT.H FieldActor accesses, canonical
 * void Engine_ActorEnableActionCallback and named FuneKanpan_SailorActions, supported by
 * OBJECT/BY_ID.C and exact FUNE_KANPAN deck scenes. Keep all original local
 * lifetimes and calls; this reduces 83 halfwords/55 edits to 3/2. Complete
 * 856-byte owner, saved registers and all three pools now agree. Only the
 * loop increment at 247c is early: candidate adds r5 before the scale_y
 * store and movs r0,#1; reference adds after both, immediately before wait.
 * Follow-up: compiler dumps locate that independent increment before
 * NOTE_INSN_LOOP_CONT. Make it the natural for-loop continuation after
 * the wait instead of a pre-wait body statement. The scheduler then emits
 * the reference store/movs/increment/call order: all 856 bytes and pools
 * exact. No counter type, declaration-order or fixed-register changes. */
void FieldScene_RunActorSequence(void)
{
    u32 i;
    struct FieldActor *rec8;
    struct FieldActor *record;
    s32 base5_200c8c4;
    s32 base5_200c8b0;
    s32 base5_200c8d8;
    s32 base5_0;
    const u8 *base5_200c888;

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    record = Object_GetById(0);
    ObjectDispatch_SetSingleChildField26Far(record, 0);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScriptD);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(22, 0xb00000, 0x2b80000);
    record = Object_GetById(22);
    {
        s32 shown = 0xd000;

        record->facing = shown;
    }
    Engine_ActorSetPosition(21, 0x1080000, 0x2960000);
    record = Object_GetById(21);
    {
        s32 shown = 0xb000;

        record->facing = shown;
    }
    Call3(Engine_ActorSetPosition, 24, 0xb80000, 0x2a00000);
    Call3(Engine_ActorSetPosition, 25, 0xca0000, 0x2b40000);
    Call3(Engine_ActorSetPosition, 26, 0xfc0000, 0x2860000);
    Call3(Engine_ActorSetPosition, 27, 0x1000000, 0x2ae0000);
    Call3(Engine_ActorSetPosition, 28, 0xac0000, 0x2780000);
    Engine_ActorSetPosition(29, 0x1000000, 0x26e0000);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        Object_GetById(24)->rise_enabled = zero.v;
        Object_GetById(25)->rise_enabled = 1;
        Object_GetById(26)->rise_enabled = zero.v;
        Object_GetById(27)->rise_enabled = 2;
    }
    Engine_ActorSetPosition(20, 0, 0);
    Engine_ActorEnableActionCallback(24, FuneKanpan_DeckActionsA);
    Engine_ActorEnableActionCallback(25, (s32)FuneKanpan_DeckActionsA);
    Engine_ActorEnableActionCallback(26, FuneKanpan_DeckActionsB);
    Engine_ActorEnableActionCallback(27, (s32)FuneKanpan_DeckActionsB);
    Engine_ActorEnableActionCallback(28, FuneKanpan_DeckActionsC);
    Engine_ActorEnableActionCallback(29, (s32)FuneKanpan_DeckActionsC);
    Engine_ActorSetChildValue(24, 3);
    Engine_ActorSetChildValue(25, 3);
    Engine_ActorSetChildValue(26, 3);
    Engine_ActorSetChildValue(27, 3);
    Engine_ActorSetChildValue(28, 3);
    Engine_ActorSetChildValue(29, 3);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    rec8 = Object_GetById(31);
    rec8->scale_x = 0x1999;
    rec8->scale_y = 0x1999;
    rec8->x.fixed = 0xc20000;
    rec8->z.fixed = 0x2820000;
    for (base5_0 = 0; (u32)base5_0 < 16; base5_0++) {
        rec8->scale_x += 0xf5c;
        rec8->scale_y += 0xf5c;
        Engine_TaskWait(1);
    }
    rec8 = Object_GetById(30);
    rec8->scale_x = 0x11999;
    rec8->scale_y = 0x11999;
    rec8->x.fixed = 0xc20000;
    rec8->y.fixed = 0x500000;
    rec8->z.fixed = 0x2820000;
    {
        s32 shown = 0x5000;

        rec8->facing = shown;
    }
    *(s32 *)((s32)rec8 + 68) = 0x6666;
    *(s32 *)((s32)rec8 + 72) = 0x20000;
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    Engine_ActorSetPosition(31, 0, 0);
    record = Object_GetById(30);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetSpeed, 0, 0x19999, 0xcccc);
    rec8 = Object_GetById(0);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        rec8->motion_flags = zero.v;
    }
    Call3(Engine_ActorSetDestination, 0, 216, 0x264);
    Call3(Engine_ActorSetSpeed, 30, 0x19999, 0xcccc);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 196, 0x258);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 216, 0x258);
    Engine_ActorStop(28);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 28, 0x19999, 0xcccc);
    base5_200c888 = FuneKanpan_SailorActions;
    Engine_ActorEnableActionCallback(28, base5_200c888);
    FieldScene_CallPairWith10(30, 0xd000);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(30, base5_200c888);
    Engine_ActorStop(29);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 29, 0x19999, 0xcccc);
    Object_SetActionCallbackAndRefreshById(29, base5_200c888);
    Engine_EventWait(20);
    Event_ClearStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_EventWait(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    UiText_ShowCenteredMessage((s32)MsgFuneArrgh, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventRequestExit(14);
}

/* The ship's deck: the leader jumps to the deck below and the party follows,
 * then the scene sets where the party returns. The engine's calls are
 * declared here without prototypes, as the call sites pass them. */

/* Runs the deck scene: the leader jumps and walks, actors 22 and 25 move
 * into place, and the scene sets where the party returns. The game state's
 * rows are written through a halfword row view (FAKEMATCH: it keeps the
 * base-plus-index address form the fields would fold away). */
void FuneKanpan_RunJumpScene(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScriptE);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(25, 1);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(21, 5);
    record = (s32)Object_GetById(21);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetPosition(0, 0, 0);
    record = (s32)Object_GetById(0);
    {
        /* FAKEMATCH: the facing held in a forced temporary is formed in
         * the game's register order. */
        s32 shown = 0x4000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    Call3((void (*)())Engine_ActorSetPosition, 0, 0xd80000, 0x24a0000);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 216, 0x256);
    Engine_EventWait(20);
    FieldScene_CallPairWith10(0, 0x6000);
    Engine_ActorJump(0, 2, 10);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x19999, 0xcccc);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 194, 0x270);
    Engine_AudioPlayCue(181);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_EventWait(20);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(10);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0xc000, 20);
    {
        /* FAKEMATCH: a one-halfword struct zero is loaded from a halfword pool entry with the reach the reference pool placement needs */
        struct { u16 v; } zero;

        zero.v = 0;
        *((u8 *)Object_GetById(25) + 85) = zero.v;
    }
    Call3((void (*)())Engine_ActorSetSpeed, 25, 0x20000, 0x10000);
    Engine_ActorSetDestination(25, 216, 0x264);
    Engine_AudioPlayCue(149);
    Engine_ActorSetSpritePriority(22, 2);
    Engine_ActorSetAnimation(22, 5);
    record = (s32)Object_GetById(22);
    *(s32 *)(record + 40) = 0x80000;
    *(s32 *)(record + 72) = 0xb333;
    *(s32 *)(record + 24) = 0x1a000;
    *(s32 *)(record + 28) = 0x1a000;
    *(s32 *)(record + 108) = (s32)OverlayObject_DecayFields24And28;
    *(s32 *)(record + 68) = 0x8000;
    Call3((void (*)())Engine_ActorSetSpeed, 22, 0x60000, 0x30000);
    Engine_ObjectMotionSetPositionAndCommit(22, 182, 0x26a);
    record = (s32)Object_GetById(22);
    Engine_ActorSetSpriteFlags(record, 0);
    FieldScene_CallPairWith10(0, 0xa000);
    Engine_ActorJump(0, 6, 80);
    Call3((void (*)())Engine_ActorSetDestination, 25, 232, 0x234);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 204, 0x262);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 208, 0x256);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 248, 0x256);
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = (s32)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3((void (*)())Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkTo, 0, 250, 0x248);
    Call3((void (*)())Engine_ActorWalkTo, 1, 240, 0x258);
    Call3((void (*)())Engine_ActorWalkTo, 2, 254, 0x258);
    Engine_ActorWalkToAndWait(3, 248, 0x268);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_ActorFaceDirection(3, 0xc000, 20);
    Engine_AudioPlayCue(149);
    Engine_EventWait(40);
    Call2((void (*)())Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2((void (*)())Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2((void (*)())Engine_ActorSetAttachedEffect, 2, 0x102);
    Engine_ActorSetAttachedEffect(3, 0x102);
    Engine_EventWait(60);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3((void (*)())Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3((void (*)())Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
    Call3((void (*)())Engine_ActorSetSpeed, 3, 0xcccc, 0x6666);
    Call3((void (*)())Engine_ActorWalkTo, 0, 248, 0x234);
    Call3((void (*)())Engine_ActorWalkTo, 1, 248, 0x234);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorWalkTo, 2, 248, 0x234);
    Call3((void (*)())Engine_ActorWalkTo, 3, 248, 0x234);
    Engine_EventWait(20);
    ((union GameStateRows *)&gGameState)->halves[226][0] = (s32)&SceneId_FuneHeya;
    do { ((union GameStateRows *)&gGameState)->halves[227][0] = 30; } while (0); /* FAKEMATCH: keeps row 227 from being derived from row 226 */
    ((u8 *)&gGameState)[0x22b] = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_FuneKanpan, 16);
    BattleFx_SetWeightedResult(62, 3);
    Engine_EventEnd();
}

/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
/* Configures actors 20, 21, 22 and 23 (position, pose, and movement/sprite
 * flags) and advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureLeadActors(void)
{
    u8 *record;

    Event_Begin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0xb60000, 0x26a0000);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = ((u8 *(*)())Object_GetById)(22);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        /* Set the high bit of the flag byte at +89. */
        u8 *record = ((u8 *(*)())Object_GetById)(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Actor_WalkToAndWait(20, 182, 0x224);
    FieldScene_CallPairWith10(20, 0);
    FieldScene_CallPairWith10(0, 0x8000);
    Actor_RunRepeatedMotion(20, 1);
    Event_SetMessage((s32)MsgFuneThankRobinDidGoodAgainst);
    FieldScene_RunStepThen10(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(40);
    Actor_FaceDirection(20, 0x5000, 20);
    Actor_ShowEmote(20, 0x105, 60);
    Event_ShowMessageAndWait(20, 0, 40);
    FieldScene_CallPairWith10(20, 0);
    FieldScene_RunStepThen10(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(20, 3);
    Actor_WalkToAndWait(20, 182, 0x258);
    Actor_WalkToAndWait(20, 216, 0x258);
    FieldScene_CallPairWith10(20, 0xc000);
    FieldScene_RunScene3af_02000bb8();
    Event_Wait(10);
    Actor_WalkToAndWait(20, 216, 0x244);
    Actor_SetPosition(20, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x92b);
    GameFlag_Clear(0x302);
    Event_End();
}

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureThreeActors(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (s32)Object_GetById(0);
    Actor_SetSpriteFlags(record, 0);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0xc40000, 0x1f60000);
    record = (s32)Object_GetById(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(22, 0xb80000, 0x20c0000);
    record = (s32)Object_GetById(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Engine_ActorSetSpritePriority(21, 1);
    Actor_SetPosition(21, 0xb80000, 0x2780000);
    record = (s32)Object_GetById(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_Jump(22, 4, 10);
    Actor_Jump(22, 6, 20);
    Event_SetMessage((s32)MsgFuneCanSeeLand);
    FieldScene_RunStepThen10(22);
    Actor_SetAnimationAndWait(20, 3);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Actor_WalkToAndWait(21, 180, 0x222);
    Engine_ActorFaceDirection(21, 0xb000, 40);
    Actor_RunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(21);
    Event_RequestExit(15);
}

/* Walks up to four present party members in one at a time, each with a line chosen by who they are, then clears story flag 0x12f. */
void FieldScene_RunPartyRosterScene(void)
{
    s32 roster[4];
    s32 matched[4];
    s32 category[4];
    u32 index;
    s32 found;
    s32 actor;
    s32 member;
    struct FieldActor *record;

    found = 0;
    for (index = 0; index <= 3; index++) {
        roster[index] = SceneState_FindFirstSetFlagOfGroup((s32)index);
        category[index] = 0;
    }

    /* Phase 2 -- nine search blocks, in source order. */
    member = 23;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 24;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 25;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 27;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 28;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 29;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 26;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 10;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 30;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 11;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 31;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 20;
            break;
        }
    }

scene:
    /* Phase 3 -- open the scene. */
    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(32, record->x.fixed, record->z.fixed);
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_CameraFollowActor(32, 1);

    FieldScene_RunScene3af_02000bb8();

    Engine_EventWait(10);

    /* Phase 4 -- one beat per matched member. */
    for (index = 0; index <= 3; index++) {
        actor = matched[index];
        Call3(Engine_ActorSetPosition, actor, 216 << 16, 146 << 18);
        if (category[index] == 20) {
            Call3(Engine_ActorSetSpeed, actor, 0xcccc, 0x6666);
        } else {
            Call3(Engine_ActorSetSpeed, actor, 128 << 9, 128 << 8);
        }
        Call3(Engine_ActorWalkToAndWait, actor, 216, 150 << 2);
        Call3(Engine_ActorWalkToAndWait, actor, 192, 0x26a);
        Engine_ActorWalkToAndWait(actor, 192, 164 << 2);

        switch (category[index]) {
        case 0:
            Engine_ActorShowEmote(actor, 129 << 1, 60);
            Engine_EventSetMessage((s32)MsgFuneMadeFinallyLucky);
            break;
        case 1:
            FieldScene_CallPairWith10(actor, 208 << 8);
            Engine_ActorShowEmote(actor, 129 << 1, 60);
            Engine_EventSetMessage((s32)MsgFuneOtherPassengersAlready);
            break;
        case 2:
            Engine_ActorShowEmote(actor, 0x105, 60);
            Engine_EventSetMessage((s32)MsgFuneDontRowAnymore);
            break;
        case 3:
            Engine_ActorRunRepeatedMotion(actor, 1);
            Engine_EventSetMessage((s32)MsgFuneOhhhHaventWorkout);
            break;
        case 10:
            Engine_ActorSetAnimationAndWait(actor, 3);
            Engine_EventSetMessage((s32)MsgFuneRowingShipMore);
            break;
        case 11:
            Engine_ActorSetAnimation(actor, 4);
            Engine_EventSetMessage((s32)MsgFuneTotallyLostOcean);
            break;
        case 20:
            Engine_ActorSetAnimationAndWait(actor, 4);
            Engine_ActorShowEmote(actor, 0x107, 40);
            Engine_EventSetMessage((s32)MsgFuneMakeOldMan);
            break;
        default:
            break;
        }

        FieldScene_RunStepThen10(actor);
        Engine_ActorEnableActionCallback(actor, FuneKanpan_RosterActions);
    }

    /* Phase 5 -- teardown. */
    Engine_ActorStartAction(actor);
    Engine_EventWait(40);
    Engine_ActorSetPosition(0, 216 << 16, 146 << 18);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 0, 216, 150 << 2);
    Call3(Engine_ActorWalkToAndWait, 0, 190, 153 << 2);
    Engine_EventSetMessage((s32)MsgFuneThanksHardWork);
    FieldScene_RunStepThen10(20);
    Engine_ActorFaceDirection(0, 192 << 8, 0);

    Object_GetById(32)->motion_flags = 0;
    Call3(Engine_ActorSetSpeed, 32, 128 << 10, 128 << 9);
    Call3(Engine_ActorSetDestination, 32, 196, 141 << 2);

    Call3(Engine_ActorSetSpeed, 20, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 20, 182, 0x22b);
    Call3(Engine_ActorFaceDirection, 20, 192 << 6, 20);
    Engine_ActorRunRepeatedMotion(20, 1);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(20, 3);
    FieldScene_RunStepThen10(20);
    Engine_ActorFaceDirection(20, 128 << 8, 40);
    FieldScene_RunStepThen10(20);
    FieldScene_CallPairWith10(20, 192 << 6);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_CameraFollowActor(0, 1);
    Call3(Engine_ActorWalkToAndWait, 20, 188, 128 << 2);
    Engine_ActorFaceDirection(20, 192 << 6, 0);
    Engine_ActorSetPosition(32, 0, 0);

    Engine_GameFlagClear(0x12f);
    Engine_EventEnd();
}

s32 SceneState_FindFirstSetFlagOfGroup(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (GameFlag_IsSet(v)!= 0) return id;
        v++;
        id++;
    }
    return 0;
}

/* Rocks the ship's deck: sways the camera by the cosine and sine of two
 * slowly, randomly advancing angles and scrolls map layer 5 by its speed,
 * wrapping the scroll within two cells. */
void FuneKanpan_RockDeck(void)
{
    struct MapWork *map = ((struct MapWork *)gMapWork);
    s32 *camera = map->camera;
    s32 dx = Engine_MathCos(FuneKanpan_WaveAngleX);
    s32 dy = Engine_MathSin(FuneKanpan_WaveAngleY);
    struct MapLayer *layer;

    *camera++ += dx >> 1;
    *camera += dy;
    FuneKanpan_WaveAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
    {
        s32 turn = FuneKanpan_WaveAngleY + ((u32)(Engine_RandomNext() << 9) >> 16);

        FuneKanpan_WaveAngleX &= 0xffff;
        FuneKanpan_WaveAngleY = turn & 0xffff;
    }
    layer = &map->layers[5];
    layer->x = FuneKanpan_LayerScroll[0];
    FuneKanpan_LayerScroll[0] -= FuneKanpan_LayerSpeed[0];
    if (FuneKanpan_LayerScroll[0] < 0) {
        FuneKanpan_LayerScroll[0] += 0x200000;
    }
    if (FuneKanpan_LayerScroll[0] > 0x200000) {
        FuneKanpan_LayerScroll[0] -= 0x200000;
    }
    layer->y = FuneKanpan_LayerScroll[1];
    FuneKanpan_LayerScroll[1] -= FuneKanpan_LayerSpeed[1];
    if (FuneKanpan_LayerScroll[1] < 0) {
        FuneKanpan_LayerScroll[1] += 0x200000;
    }
}
